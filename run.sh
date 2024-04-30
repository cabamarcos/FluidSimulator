#!/bin/bash

if [ ! -d ./outputs ]; then
    mkdir outputs
fi

perf stat ./build/fluidapp 1000 ./inputs/large.fld ./outputs/large.fld
echo "Finished execution"

