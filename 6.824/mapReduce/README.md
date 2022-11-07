# 6.824 分布式系统学习

用于记录6.824分布式系统学习的内容。

## 参考资料

* [参考mapreduce项目](https://github.com/yzongyue/6.824-golabs-2020)

## 1. mapReduce程序思路

```
1.Master结构体初始化，注册rpc服务供worker调用
2.initMapTask完成taskPhase，taskStats的创建(MapPhase,taskStats数组)
3.schedule中m.taskStats初始值为TaskStatusReady,调用getTask创建任务，并传递给通道m.taskCh
4.worker通过rpc调用GetOneTask,触发task将任务状态转变成TaskStatusRunning，且通过rpc将任务分发给worker
5.worker通过ReportTask，将任务完成情况上报给master,所有任务的mapTask都是TaskStatusFinish，则进入initReduceTask
6.initReduceTask完成taskPhase，taskStats的创建(ReducePhase,taskStats数组)
7.ReduceTask的处理过程类似MapTask
```

## 目录结构

```
images ———— 项目图片
log    ———— 项目程序执行日志
source ———— 项目原始文件和论文
src    ———— 项目源码
study  ———— go语言的学习记录
```





