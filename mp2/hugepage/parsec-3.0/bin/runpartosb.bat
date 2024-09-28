ssh kvm@kvm1 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads" &
ssh kvm@kvm2 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads" &
ssh kvm@kvm3 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads" &
ssh kvm@kvm4 "/home/kvm/parsec-3.0/bin/parsecmgmt -a run -p $1 -i native -n 16 -c gcc-pthreads" &

