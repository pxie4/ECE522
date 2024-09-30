#!/bin/bash



echo 'always' | sudo tee /sys/kernel/mm/transparent_hugepage/enabled
