#!/bin/bash
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 3000000 > /proc/sys/kernel/sched_min_granularity_ns'"
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 4000000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 2000000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 1000000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 500000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 250000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 125000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 62500 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 0 > /proc/sys/kernel/sched_wakeup_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads

echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 2000000 > /proc/sys/kernel/sched_min_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 1000000 > /proc/sys/kernel/sched_min_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 500000 > /proc/sys/kernel/sched_min_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 250000 > /proc/sys/kernel/sched_min_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 125000 > /proc/sys/kernel/sched_min_granularity_ns'"
./parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads

echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 3000000 > /proc/sys/kernel/sched_min_granularity_ns'"
echo $password | ssh dingxn@phoenix "sudo -S sh -c 'echo 4000000 > /proc/sys/kernel/sched_wakeup_granularity_ns'"

exec 3<>/dev/tcp/vm1/8888
echo "FINISH" >&3
