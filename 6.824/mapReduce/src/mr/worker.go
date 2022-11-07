package mr

import (
	"encoding/json"
	"fmt"
	"hash/fnv"
	"io/ioutil"
	//"log"
	"net/rpc"
	"os"
	"strings"
)

//
// Map functions return a slice of KeyValue.
//
type KeyValue struct {
	Key   string
	Value string
}

//
// use ihash(key) % NReduce to choose the reduce
// Task number for each KeyValue emitted by Map.
//
func ihash(key string) int {
	h := fnv.New32a()
	h.Write([]byte(key))
	return int(h.Sum32() & 0x7fffffff)
}

func Worker(mapf func(string, string) []KeyValue,
	reducef func(string, []string) string) {
	
	logDebug("[Worker::Worker] begin")

	w := worker{}
	w.mapf = mapf
	w.reducef = reducef
	w.register()
	w.run()

	logDebug("[Worker::Worker] end")
	
}

type worker struct {
	id      int
	mapf    func(string, string) []KeyValue
	reducef func(string, []string) string
}

func (w *worker) run() {
	logDebug("[Worker::run] in")
	for {
		t := w.reqTask()
		if !t.Alive {
			logDebug("[Worker::run] worker get task not alive, exit")
			return
		}
		w.doTask(t)
	}
}

func (w *worker) reqTask() Task {
	args := TaskArgs{}
	args.WorkerId = w.id
	reply := TaskReply{}

	if ok := call("Master.GetOneTask", &args, &reply); !ok {
		logError("[Worker::reqTask] worker get task fail,exit")
		os.Exit(1)
	}

	logInfo("[Worker::reqTask] %+v worker get task:%+v", w.id , reply.Task)

	return *reply.Task
}

func (w *worker) doTask(t Task) {
	logDebug("[Worker::doTask] in do Task")

	switch t.Phase {
	case MapPhase:
		w.doMapTask(t)
	case ReducePhase:
		w.doReduceTask(t)
	default:
		panic(fmt.Sprintf("task phase err: %v", t.Phase))
	}

}

//每个key-value的分配规则是，将其分配给第ihash(key) % nReduce 个task。
//生成NReduce个中间文件
func (w *worker) doMapTask(t Task) {
	logDebug("[Worker::doMapTask] WorkerId:%+v begin", w.id)

	contents, err := ioutil.ReadFile(t.FileName)
	if err != nil {
		w.reportTask(t, false, err)
		return
	}

	kvs := w.mapf(t.FileName, string(contents))
	reduces := make([][]KeyValue, t.NReduce)
	for _, kv := range kvs {
		idx := ihash(kv.Key) % t.NReduce
		reduces[idx] = append(reduces[idx], kv)
	}

	for idx, l := range reduces {
		fileName := reduceName(t.Seq, idx)
		f, err := os.Create(fileName)
		if err != nil {
			w.reportTask(t, false, err)
			return
		}
		enc := json.NewEncoder(f)
		for _, kv := range l {
			if err := enc.Encode(&kv); err != nil {
				w.reportTask(t, false, err)
			}

		}
		if err := f.Close(); err != nil {
			w.reportTask(t, false, err)
		}
	}
	w.reportTask(t, true, nil)
	logDebug("[Worker::doMapTask] WorkerId:%+v end", w.id)
}

func (w *worker) doReduceTask(t Task) {
	logDebug("[Worker::doReduceTask] WorkerId:%+v begin", w.id)

	maps := make(map[string][]string)
	for idx := 0; idx < t.NMaps; idx++ {
		fileName := reduceName(idx, t.Seq)
		file, err := os.Open(fileName)
		if err != nil {
			w.reportTask(t, false, err)
			return
		}
		dec := json.NewDecoder(file)
		for {
			var kv KeyValue
			if err := dec.Decode(&kv); err != nil {
				break
			}
			if _, ok := maps[kv.Key]; !ok {
				maps[kv.Key] = make([]string, 0, 100)
			}
			maps[kv.Key] = append(maps[kv.Key], kv.Value)
		}
	}

	res := make([]string, 0, 100)
	for k, v := range maps {
		res = append(res, fmt.Sprintf("%v %v\n", k, w.reducef(k, v)))
	}

	if err := ioutil.WriteFile(mergeName(t.Seq), []byte(strings.Join(res, "")), 0600); err != nil {
		w.reportTask(t, false, err)
	}

	w.reportTask(t, true, nil)

	logDebug("[Worker::doReduceTask] WorkerId:%+v end", w.id)
}

func (w *worker) reportTask(t Task, done bool, err error) {
	if err != nil {
		logError("[Worker::reportTask] %v", err)
	}
	args := ReportTaskArgs{}
	args.Done = done
	args.Seq = t.Seq
	args.Phase = t.Phase
	args.WorkerId = w.id
	reply := ReportTaskReply{}

	if ok := call("Master.ReportTask", &args, &reply); !ok {
		logError("[Worker::reportTask] report task fail:%+v", args)
	}

	logDebug("[Worker::reportTask] info args:%+v,reply:%+v", args,reply)
}

//通过rpc调用，向master注册并获取WorkerId
func (w *worker) register() {
	args := &RegisterArgs{}
	reply := &RegisterReply{}

	if ok := call("Master.RegWorker", args, reply); !ok {
		logError("[Worker::register] reg fail")
	}
	w.id = reply.WorkerId

	logDebug("[Worker::register] args:%+v,reply:%+v", args,reply)
}

//
// send an RPC request to the master, wait for the response.
// usually returns true.
// returns false if something goes wrong.
//
func call(rpcname string, args interface{}, reply interface{}) bool {
	// c, err := rpc.DialHTTP("tcp", "127.0.0.1"+":1234")
	sockname := masterSock()
	c, err := rpc.DialHTTP("unix", sockname)
	if err != nil {
		logError("[Worker::call] dialing:", err)
		return false
	}
	defer c.Close()

	err = c.Call(rpcname, args, reply)
	if err == nil {
		return true
	}

	logDebug("[Worker::call] %+v", err)
	return false
}
