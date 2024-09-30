#!/bin/bash

for bench in facesim streamcluster
do
	for i in `seq 1 1`
	do
		./parsecmgmt -a run -p $bench -i native -n 8 -c gcc-pthreads
        done
done


for bench in barnes ocean_cp ocean_ncp volrend
do
	for i in `seq 1 1`
       do
		./parsecmgmt -a run -p splash2x.$bench -i native -n 8 -c gcc-pthreads
       done
done


./parsecmgmt -a run -p dedup -i native -n 8 -c gcc-pthreads
