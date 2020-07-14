#! /bin/sh

set -e

TOP=`pwd`
export ARCH=arm

# =====================================
# Cross compile tools path define 
# default path is /opt/cross if not define CROSS_COMPILE_PATH
# =====================================
CROSS_COMPILE_PATH=/media/tom/ext2t/freescale/cross-compile/arm-linux-androideabi-4.9/bin
if [ ! -d $CROSS_COMPILE_PATH ];then
echo "The CROSS_COMPILE_PATH is not exist!!!"
fi

# =====================================
# Cross compile tools version
# =====================================
export CROSS_COMPILE=${CROSS_COMPILE_PATH}/arm-linux-androideabi-


# =====================================
# Configue file select
# =====================================
#TreadMill Board
U_BOOT_DEFAULT_CONFIG=rtx_treadmill_mx6q_samsung2g_dtb_rtx_all_android_defconfig

# Get Host Number of CPUs
CPUS=`cat /proc/cpuinfo | grep processor | wc -l`

if [ ! -d out ] ; then
	mkdir -p out
fi

case "${1}" in
	"all")
		#rm -rf out
		#mkdir -p out
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
		#rm -rf out/u*
		;;
	"tools")
		cd RTX/tools
		make clean
		make
		cd -
		;;

	*) 
		echo "${0} [all/clean/distclean]"
		#exit 1
		;;
esac
