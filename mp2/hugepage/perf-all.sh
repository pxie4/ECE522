#!/bin/bash

if [ $# -ne 1 ]; then
	echo "./perf-all.sh tested-system"
	exit
fi

home_dir=`echo $HOME`

for bench in canneal streamcluster
do
	TS=`date +%Y-%m-%d_%H-%M-%S`
	sudo perf stat -x, -o $home_dir/ece522-mp/hugepage/results/$1.$bench.$TS.csv --cpu=0 --append -e dTLB-load-misses,iTLB-load-misses,cache-misses $home_dir/ece522-mp/hugepage/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 1 -c gcc-pthreads &>> $home_dir/ece522-mp/hugepage/logs/$1.$bench.$TS.log
done

for bench in fmm
do
	TS=`date +%Y-%m-%d_%H-%M-%S`
	sudo perf stat -x, -o $home_dir/ece522-mp/hugepage/results/$1.$bench.$TS.csv --cpu=0 --append -e dTLB-load-misses,iTLB-load-misses,cache-misses $home_dir/ece522-mp/hugepage/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 1 -c gcc-pthreads &>> $home_dir/ece522-mp/hugepage/logs/$1.$bench.$TS.log 
done
