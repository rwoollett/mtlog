#!/bin/bash

TSAN_OPTIONS="halt_on_error=0 second_deadlock_stack=1" \
setarch x86_64 -R \
./build/tsan/tsantest/TSANMTLog "$@"
