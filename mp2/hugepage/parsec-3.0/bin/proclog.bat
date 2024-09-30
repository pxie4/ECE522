#!/bin/bash
grep -h -R real ~/parsec-3.0/log/ > /tmp/time
grep -h -R '========== Running' ~/parsec-3.0/log/ > /tmp/bench
awk -F '[ms\t]' '{print $2*60+$3}' /tmp/time > /tmp/realtime
awk -F '[. ]' {'print $6}' /tmp/bench > /tmp/benchnames
paste /tmp/benchnames /tmp/realtime > /tmp/benchtime
sort -d  /tmp/benchtime > /tmp/benchtime.sorted
awk 'BEGIN{total=0;line=0}{total=total+$2; line=line+1; if(line==5) {print $1, total, total/line; line=0;total=0}}' /tmp/benchtime.sorted
