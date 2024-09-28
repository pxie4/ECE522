#!/bin/bash

for bench in bodytrack dedup facesim streamcluster vips 
do
	./parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads
done

for bench in cholesky ocean_cp ocean_ncp volrend 
do
	./parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads
done
