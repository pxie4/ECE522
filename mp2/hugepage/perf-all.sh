#!/bin/bash

if [ $# -ne 1 ]; then
	echo "./perf-all.sh tested-system"
	exit
fi

home_dir=`echo $HOME/ECE522/mp2`

for bench in canneal streamcluster
do
	TS=`date +%Y-%m-%d_%H-%M-%S`

	mkdir -p $home_dir/hugepage/results/$bench
	sudo perf stat -x, -o $home_dir/hugepage/results/$bench/$1.$bench.$TS.csv --cpu=0 --append -e dTLB-load-misses,iTLB-load-misses,cache-misses $home_dir/hugepage/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 1 -c gcc-pthreads &>> $home_dir/hugepage/results/$bench/$1.$bench.$TS.log
	if [ $? -ne 0 ]; then
    	echo "Error running benchmark $bench"
	fi	
done

for bench in fmm
do
	TS=`date +%Y-%m-%d_%H-%M-%S`
	mkdir -p $home_dir/hugepage/results/$bench
	sudo perf stat -x, -o $home_dir/hugepage/results/$bench/$1.$bench.$TS.csv --cpu=0 --append -e dTLB-load-misses,iTLB-load-misses,cache-misses $home_dir/hugepage/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 1 -c gcc-pthreads &>> $home_dir/hugepage/results/$bench/$1.$bench.$TS.log 
	if [ $? -ne 0 ]; then
    	echo "Error running benchmark $bench"
	fi	
done

