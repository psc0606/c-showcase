#!/bin/bash
cd src_init_app
make clean
make
cd ../src_debuger_app
make clean
make
cd ../src_ko
make clean
make
cd ../rw9361viaspi
make clean
make
