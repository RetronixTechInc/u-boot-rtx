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

# =====================================
# Configue file select
# =====================================
U_BOOT_DEFAULT_CONFIG=imx8mm_evk_defconfig

# =====================================
# Run
# =====================================
# Get Host Number of CPUs
CPUS=`cat /proc/cpuinfo | grep processor | wc -l`

case "${1}" in
	"all")
		if [ ! -d out ] ; then
			mkdir -p out
		fi
		rm -rf out/*; sync

		if [ ! -f .config ] ; then
			make ${U_BOOT_DEFAULT_CONFIG}
		fi
		make -j${CPUS}
		./install_uboot_imx8.sh
		if [ -f imx-mkimage/iMX8M/flash.bin ] ; then
			cp imx-mkimage/iMX8M/flash.bin out/.
			sync
		fi
		;;
	"config")
		make ${U_BOOT_DEFAULT_CONFIG}
		;;
	"menuconfig")
		if [ ! -f .config ] ; then
			make ${U_BOOT_DEFAULT_CONFIG}
		fi
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
