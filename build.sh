#! /bin/sh

set -e

TOP=`pwd`
export ARCH=arm

# =====================================
# Cross compile tools path define 
# default path is /opt/cross if not define CROSS_COMPILE_PATH
# =====================================
#CROSS_COMPILE_PATH=/home/artie/JOB-Area/Android
#CROSS_COMPILE_PATH=/opt/freescale/usr/local
#CROSS_COMPILE_PATH=/media/tom/ext2t/freescale/cross-compile
if [ -z $CROSS_COMPILE_PATH ];then
CROSS_COMPILE_PATH=/opt/cross
fi

# =====================================
# Cross compile tools version
# =====================================
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-4.8.5-glibc-2.23/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.19-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.23-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.23-hf/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf- 
export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-5.3.0-glibc-2.23-hf/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/prebuilts/gcc/linux-x86/arm/arm-eabi-4.6/bin/arm-eabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/android/arm-eabi-4.8/bin/arm-eabi-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/android/arm-linux-androideabi-4.9/bin/arm-linux-androideabi-

# =====================================
# Configue file select
# =====================================
#U_BOOT_DEFAULT_CONFIG=rtx_pico_b10_imx6q_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pico_b21_imx6q_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_q7_b10_imx6q_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_a6plus_b10_imx6q_2g_defconfig

# =====================================
# Run
# =====================================
# Get Host Number of CPUs
CPUS=`cat /proc/cpuinfo | grep processor | wc -l`

case "${1}" in
	"all")
		make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG}
		make -j${CPUS}
		dd if=/dev/zero of=u-boot.imx bs=1K count=512
		sync
		dd if=SPL of=u-boot.imx bs=1K conv=notrunc
		sync
		dd if=u-boot.img of=u-boot.imx bs=1K seek=68 conv=notrunc
		sync
		;;
	"config")
		make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG}
		;;
	"menuconfig")
		make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG}
		make menuconfig
		;;
	"clean")
		make clean
		;;
	"distclean")
		make distclean
		;;
	"tools")
		cd RTX/tools
		make clean
		make
		cd -
		;;

	*) 
		echo "${0} [all/config/menuconfig/clean/disclean/tools]"
		#exit 1
		;;
esac
