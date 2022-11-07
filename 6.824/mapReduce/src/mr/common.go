package mr

import (
	"fmt"
	"log"
)

//任务阶段: 分Map和Reduce
type TaskPhase int

const (
	MapPhase    TaskPhase = 0
	ReducePhase TaskPhase = 1
)

//任务结构体
type Task struct {
	FileName      string
	NReduce       int
	NMaps         int
	Seq           int
	Phase         TaskPhase
	Alive         bool	// worker should exit when alive is false
}

//reduce和merge的名称
func reduceName(mapIdx, reduceIdx int) string {
	return fmt.Sprintf("mr-%d-%d", mapIdx, reduceIdx)
}

func mergeName(reduceIdx int) string {
	return fmt.Sprintf("mr-out-%d", reduceIdx)
}

//日志打印
const Debug = false

func DPrintf(format string, v ...interface{}) {
	if Debug {
		log.Printf(format+"\n", v...)
	}
}

//自定义日志打印函数
var logLevel int = 0

const (
	DEBUG  int = 0
	INFO   int = 1
	WARN   int = 2
	ERROR  int = 3
)

func logDebug(format string, v ...interface{}) {
	if  DEBUG>=logLevel {
		log.Printf("DEBUG:"+format+"\n", v...)
	}
}

func logInfo(format string, v ...interface{}) {
	if INFO>=logLevel {
		log.Printf("INFO:"+format+"\n", v...)
	}
}

func logWarn(format string, v ...interface{}) {
	if WARN>=logLevel {
		log.Printf("WARN:"+format+"\n", v...)
	}
}

func logError(format string, v ...interface{}) {
	if ERROR>=logLevel {
		log.Printf("ERROR:"+format+"\n", v...)
	}
}
