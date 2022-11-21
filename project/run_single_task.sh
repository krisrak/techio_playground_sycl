#!/bin/bash
source /opt/intel/inteloneapi/setvars.sh > /dev/null 2>&1
dpcpp single_task.cpp
if [ $? -eq 0 ]; then ./a.out; fi

