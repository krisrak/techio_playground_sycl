#!/bin/bash
source /opt/intel/inteloneapi/setvars.sh > /dev/null 2>&1
dpcpp buffer.cpp
if [ $? -eq 0 ]; then ./a.out; fi

