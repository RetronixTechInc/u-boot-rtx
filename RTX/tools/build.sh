#! /bin/sh

set -e

TOP=`pwd`
export ARCH=arm

# =====================================
# Cross compile tools path define 
# default path is /opt/cross if not define CROSS_COMPILE_PATH
# =====================================
#[ -d /opt/freescale/usr/local ] && CROSS_COMPILE_PATH=/opt/freescale/usr/local
#[ -d /home/artie/JOB-Area/Android ] && CROSS_COMPILE_PATH=/home/artie/JOB-Area/Android
#[ -d /media/tom/ext2t/freescale/cross-compile ] && CROSS_COMPILE_PATH=/media/tom/ext2t/freescale/cross-compile
#if [ -z $CROSS_COMPILE_PATH ];then
#CROSS_COMPILE_PATH=/opt/cross
#fi
#MY_ANDROID=/media/alston/HDD_IN/Android/android_build

# =====================================
# Cross compile tools version
# =====================================
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-4.8.5-glibc-2.23/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.19-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.23-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.23-hf/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf- 
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-5.3.0-glibc-2.23-hf/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/android/arm-eabi-4.8/bin/arm-eabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/android/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-
export CROSS_COMPILE=${MY_ANDROID}/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-

make clean
make


