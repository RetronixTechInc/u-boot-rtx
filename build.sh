#! /bin/sh

set -e

TOP=`pwd`

export ARCH=arm
#export CROSS_COMPILE=/opt/cross/rtx-gcc-4.9.3-glibc-2.19-hf-64bits/arm-rtx-linux-gnueabihf/bin/arm-rtx-linux-gnueabihf-
export CROSS_COMPILE=/opt/freescale/usr/local/gcc-4.8.5-glibc-2.23/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_android_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_mfg_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_defconfig
#U_BOOT_DEFAULT_CONFIG=rtx_pitx_mx6q_2g_defconfig
U_BOOT_DEFAULT_CONFIG=rtx_a6_mx6q_defconfig

if [ ! -d out ] ; then
	mkdir -p out
fi

case "${1}" in
	"all")
		rm -rf out
		mkdir -p out
		make ${U_BOOT_DEFAULT_CONFIG}
		make -j8
		cp -f u-boot.imx out/.
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
		exit 1
		;;
esac
