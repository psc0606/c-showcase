#!/bin/bash
#测试大量I/O操作
dd if=/dev/zero of=/tmp/data bs=1M count=1000
