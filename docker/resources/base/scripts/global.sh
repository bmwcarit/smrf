#!/bin/bash

log () {
    echo ""
    echo "========================================"
    echo "= $1"
    echo "========================================"
    echo ""
}

if hash nproc 2>/dev/null; then
    JOBS=$(nproc)
else
    JOBS=1
fi

ulimit -c unlimited
