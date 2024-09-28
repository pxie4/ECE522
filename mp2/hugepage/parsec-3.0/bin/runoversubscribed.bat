virsh shutdown kvm1
virsh shutdown kvm2
virsh shutdown kvm3
virsh shutdown kvm4
virsh shutdown kvm5
virsh shutdown kvm6
virsh shutdown kvm7
virsh shutdown kvm8

sleep 100

virsh start kvm1

sleep 150 

for bench in facesim streamcluster fluidanimate
do
        for i in `seq 1 3`
        do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads"
        done
done

for bench in barnes ocean_cp ocean_ncp volrend
do
        for i in `seq 1 3`
       do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads"
       done
done

for i in `seq 1 3`
do
        ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads"
done

sleep 600

virsh shutdown kvm1
virsh shutdown kvm2
virsh shutdown kvm3
virsh shutdown kvm4
virsh shutdown kvm5
virsh shutdown kvm6
virsh shutdown kvm7
virsh shutdown kvm8

sleep 150

virsh start kvm1
virsh start kvm2

sleep 300

for bench in facesim streamcluster fluidanimate
do
        for i in `seq 1 3`
        do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" 
				sleep 100
        done
done

for bench in barnes ocean_cp ocean_ncp volrend
do
        for i in `seq 1 3`
       do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads"
				sleep 100
       done
done

for i in `seq 1 3`
do
        ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads"
		sleep 300
done

sleep 600


virsh shutdown kvm1
virsh shutdown kvm2
virsh shutdown kvm3
virsh shutdown kvm4
virsh shutdown kvm5
virsh shutdown kvm6
virsh shutdown kvm7
virsh shutdown kvm8

sleep 300

virsh start kvm1
virsh start kvm2
virsh start kvm3
virsh start kvm4

sleep 500

for bench in facesim streamcluster fluidanimate
do
        for i in `seq 1 3`
        do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" 
                sleep 300
        done
done

for bench in barnes ocean_cp ocean_ncp volrend
do
        for i in `seq 1 3`
       do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads"
                sleep 300
       done
done

for i in `seq 1 3`
do
        ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads"
        sleep 600
done

sleep 600


virsh shutdown kvm1
virsh shutdown kvm2
virsh shutdown kvm3
virsh shutdown kvm4
virsh shutdown kvm5
virsh shutdown kvm6
virsh shutdown kvm7
virsh shutdown kvm8

sleep 300

virsh start kvm1
virsh start kvm2
virsh start kvm3
virsh start kvm4
virsh start kvm5
virsh start kvm6
virsh start kvm7
virsh start kvm8

sleep 600

for bench in facesim streamcluster fluidanimate
do
        for i in `seq 1 3`
        do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm5 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm6 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm7 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm8 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $bench -i native -n 16 -c gcc-pthreads"
                sleep 500
        done
done

for bench in barnes ocean_cp ocean_ncp volrend
do
        for i in `seq 1 3`
       do
                ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
		    	ssh kvm@kvm5 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm6 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm7 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads" &
                ssh kvm@kvm8 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p splash2x.$bench -i native -n 16 -c gcc-pthreads"
                sleep 500
       done
done

for i in `seq 1 3`
do
        ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm5 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm6 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm7 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads" &
        ssh kvm@kvm8 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p dedup -i native -n 16 -c gcc-pthreads"
        sleep 800
done

