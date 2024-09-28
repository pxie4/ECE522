#!/bin/bash
exec 3<>/dev/tcp/vm1/8888
echo "BEGIN" >&3
sleep 30
echo "ending..."
echo "END" >&3

