#!/bin/bash
awk 'BEGIN{line=0; sum=0;}{line=line+1; sum=sum+$2;}END{print line/10, sum/(line/10)}' /tmp/output
