#! /bin/sh

set -e

TOP=`pwd`

CROSS_COMPILE_PATH=/media/tom/ext1t/freescale/cross-compile
export ARCH=arm
export CROSS_COMPILE=${CROSS_COMPILE_PATH}/rtx-gcc-4.9.3-glibc-2.23-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
#export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-4.8.5-glibc-2.23/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-

# A6 PLUS board
#U_BOOT_DEFAULT_CONFIG=rtx_a6plus_mx6q_micro1g_n_roymark_tv_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_a6plus_mx6q_micro1g_n_roymark_box_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_a6plus_mx6q_micro1g_y_rtx_box_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_a6plus_mx6q_micro1g_y_rtx_all_linux_defconfig

#A6 Board
#U_BOOT_DEFAULT_CONFIG=rtx_a6_mx6q_micro1g_y_rtx_all_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_a6_mx6q_micro1g_y_rtx_all_linux_defconfig

#PITX Board
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_nanya1g_mfg_defconfig
U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_nanya1g_y_rtx_all_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_nanya1g_y_rtx_all_linux_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_nanya2g_y_rtx_all_android_defconfig

#Q7 Board


# Get Host Number of CPUs
CPUS=`cat /proc/cpuinfo | grep processor | wc -l`

if [ ! -d out ] ; then
	mkdir -p out
fi

case "${1}" in
	"all")
		#rm -rf out
		mkdir -p out
		make ${U_BOOT_DEFAULT_CONFIG}
		make -j${CPUS}
		cp -f u-boot.imx out/.
		;;
	"config")
		make ${U_BOOT_DEFAULT_CONFIG}
		;;
	"menuconfig")
		make menuconfig
		;;
	"clean")
		make clean
		;;
	"distclean")
		make distclean
		rm -rf out
		;;

	*) 
		echo "${0} [all/clean/disclean]"
		#exit 1
		;;
esac
