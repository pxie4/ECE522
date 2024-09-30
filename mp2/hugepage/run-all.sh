#!/bin/bash

if [ $# -ne 1 ]; then
	echo "./perf-all.sh tested-system"
	exit
fi

#kvm1=192.168.122.109

home_dir=`echo $HOME`

for bench in canneal streamcluster
do
	TS=`date +%Y-%m-%d_%H-%M-%S`

	mkdir -p $home_dir/ECE522/mp2/hugepage/logs/$bench
	{ sudo $home_dir/ECE522/mp2/hugepage/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 1 -c gcc-pthreads ; } &>> $home_dir/ECE522/mp2/hugepage/logs/$bench/$1.$bench.$TS.log

	if [ $? -ne 0 ]; then
    	echo "Error running benchmark $bench"
	fi	
done

for bench in fmm
do
	TS=`date +%Y-%m-%d_%H-%M-%S`

	mkdir -p $home_dir/ECE522/mp2/hugepage/logs/$bench
	{ sudo $home_dir/ECE522/mp2/hugepage/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 1 -c gcc-pthreads ; } &>> $home_dir/ECE522/mp2/hugepage/logs/$bench/$1.$bench.$TS.log
	
	if [ $? -ne 0 ]; then
    	echo "Error running benchmark $bench"
	fi	
done
