#! /bin/sh

set -e

TOP=`pwd`
export ARCH=arm64

# =====================================
# Cross compile tools path define 
# default path is /opt/cross if not define CROSS_COMPILE_PATH
# =====================================

if [ -z $CROSS_COMPILE_PATH ];then
CROSS_COMPILE_PATH=/opt/cross
fi

# =====================================
# Cross compile tools version
# =====================================
export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-


make clean
make


