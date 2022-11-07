package mr

import (
	//"log"
	"net"
	"net/http"
	"net/rpc"
	"os"
	"sync"
	"time"
)

//任务状态
const (
	TaskStatusReady   = 0
	TaskStatusQueue   = 1
	TaskStatusRunning = 2
	TaskStatusFinish  = 3
	TaskStatusErr     = 4
)

//最大任务运行时间，超时即fail
//间隔时间
const (
	MaxTaskRunTime   = time.Second * 5
	ScheduleInterval = time.Millisecond * 500
)

type TaskStat struct {
	Status    int
	WorkerId  int
	StartTime time.Time
}

//Master 结构体，taskCh  任务通道，用于goroutine之间通信
type Master struct {
	files     []string
	nReduce   int
	taskPhase TaskPhase
	taskStats []TaskStat
	mu        sync.Mutex
	done      bool
	workerSeq int
	taskCh    chan Task
}

//获取任务，Master内部调用
func (m *Master) getTask(taskSeq int) Task {
	task := Task{
		FileName: "",
		NReduce:  m.nReduce,
		NMaps:    len(m.files),
		Seq:      taskSeq,
		Phase:    m.taskPhase,
		Alive:    true,
	}
	//logDebug("[Master::getTask] m:%+v, taskseq:%d, lenfiles:%d, lents:%d", m, taskSeq, len(m.files), len(m.taskStats))
	if task.Phase == MapPhase {
		task.FileName = m.files[taskSeq]
	}

	logDebug("[Master::getTask] taskSeq:%+v, taskPhase:%+v, taskFileName:%+v", task.Seq, task.Phase, task.FileName)
	
	return task
}

//调度worker和task
/*
1.Master结构体初始化
2.initMapTask完成taskPhase，taskStats的创建(MapPhase,taskStats数组)
3.schedule中m.taskStats初始值为TaskStatusReady,调用getTask创建任务，并传递给通道m.taskCh
4.worker通过rpc调用GetOneTask,触发task将任务状态转变成TaskStatusRunning，且通过rpc将任务分发给worker
5.worker通过ReportTask，将任务完成情况上报给master,所有任务的mapTask都是TaskStatusFinish，则进入initReduceTask
6.initReduceTask完成taskPhase，taskStats的创建(ReducePhase,taskStats数组)
7.ReduceTask的处理过程类似MapTask
*/
func (m *Master) schedule() {
	m.mu.Lock()
	defer m.mu.Unlock() //用 defer 来确保在函数内正确释放了锁

	logDebug("[Master::schedule] in")

	if m.done {
		return
	}
	allFinish := true
	for index, t := range m.taskStats {
		switch t.Status {
		case TaskStatusReady:
			allFinish = false
			m.taskCh <- m.getTask(index)
			m.taskStats[index].Status = TaskStatusQueue
		case TaskStatusQueue:
			allFinish = false
		case TaskStatusRunning:
			allFinish = false
			if time.Now().Sub(t.StartTime) > MaxTaskRunTime {
				m.taskStats[index].Status = TaskStatusQueue
				m.taskCh <- m.getTask(index)
			}
		case TaskStatusFinish:
		case TaskStatusErr:
			allFinish = false
			m.taskStats[index].Status = TaskStatusQueue
			m.taskCh <- m.getTask(index)
		default:
			panic("t.status err") //程序终止运行
		}
	}
	if allFinish {
		if m.taskPhase == MapPhase {
			m.initReduceTask()
		} else {
			m.done = true
		}
	}
}

//初始化MapTask
func (m *Master) initMapTask() {
	logDebug("[Master::initMapTask] in")
	m.taskPhase = MapPhase
	m.taskStats = make([]TaskStat, len(m.files))
	logDebug("[Master::initMapTask] m.taskStats:%+v",m.taskStats)
}

//初始化ReduceTask
func (m *Master) initReduceTask() {
	logDebug("[Master::initReduceTask] in")
	m.taskPhase = ReducePhase
	m.taskStats = make([]TaskStat, m.nReduce)
	logDebug("[Master::initReduceTask] m.taskStats:%+v",m.taskStats)
}

func (m *Master) regTask(args *TaskArgs, task *Task) {
	m.mu.Lock()
	defer m.mu.Unlock()

	if task.Phase != m.taskPhase {
		panic("req Task phase neq")
	}

	m.taskStats[task.Seq].Status = TaskStatusRunning
	m.taskStats[task.Seq].WorkerId = args.WorkerId
	m.taskStats[task.Seq].StartTime = time.Now()

	logDebug("[Master::regTask]: m.taskStats[task.Seq]:%+v",m.taskStats[task.Seq])
}

// Your code here -- RPC handlers for the worker to call.
func (m *Master) GetOneTask(args *TaskArgs, reply *TaskReply) error {
	task := <-m.taskCh
	reply.Task = &task

	if task.Alive {
		m.regTask(args, &task)
	}

	logDebug("[Master::GetOneTask] in get one Task, args:%+v, reply:%+v", args, reply)

	return nil
}

func (m *Master) ReportTask(args *ReportTaskArgs, reply *ReportTaskReply) error {
	m.mu.Lock()
	defer m.mu.Unlock()

	logDebug("[Master::ReportTask] get report task: %+v, taskPhase: %+v", args, m.taskPhase)

	if m.taskPhase != args.Phase || args.WorkerId != m.taskStats[args.Seq].WorkerId {
		return nil
	}

	if args.Done {
		m.taskStats[args.Seq].Status = TaskStatusFinish
	} else {
		m.taskStats[args.Seq].Status = TaskStatusErr
	}

	go m.schedule()
	return nil
}

func (m *Master) RegWorker(args *RegisterArgs, reply *RegisterReply) error {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.workerSeq += 1
	reply.WorkerId = m.workerSeq

	logDebug("[Master::RegWorker] reply.WorkerId = m.workerSeq = %+v", reply.WorkerId )

	return nil
}

//
// start a thread that listens for RPCs from worker.go
//
func (m *Master) server() {

	logDebug("[Master::server] start a thread that listens for RPCs from worker")

	rpc.Register(m)
	rpc.HandleHTTP()
	//l, e := net.Listen("tcp", ":1234")
	sockname := masterSock()
	os.Remove(sockname)
	l, e := net.Listen("unix", sockname)
	if e != nil {
		logError("[Master::server] listen error:", e)
	}

	logInfo("[Master::server] socket:%+v listen %+v",l,sockname)

	go http.Serve(l, nil)
}

//
// main/mrmaster.go calls Done() periodically to find out
// if the entire job has finished.
//
func (m *Master) Done() bool {
	m.mu.Lock()
	defer m.mu.Unlock()
	return m.done
}

func (m *Master) tickSchedule() {
	// 按说应该是每个 task 一个 timer，此处简单处理
	logDebug("[Master::tickSchedule] in")
	for !m.Done() {
		go m.schedule()
		time.Sleep(ScheduleInterval)
	}
}

//
// create a Master.
//
func MakeMaster(files []string, nReduce int) *Master {
	logDebug("[Master::MakeMaster] begin")
	m := Master{}
	m.mu = sync.Mutex{}
	m.nReduce = nReduce
	m.files = files
	if nReduce > len(files) {
		m.taskCh = make(chan Task, nReduce)
	} else {
		m.taskCh = make(chan Task, len(m.files))
	}

	logInfo("[Master::MakeMaster] master info:%+v",m)

	m.initMapTask()
	go m.tickSchedule()
	m.server()
	logDebug("[Master::MakeMaster] end")
	// Your code here.
	return &m
}
