#!/bin/bash


#echo 1000 | sudo tee /proc/sys/vm/extfrag_threshold

echo 1 | sudo tee /proc/sys/vm/compact_memory
