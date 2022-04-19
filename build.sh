#! /bin/sh

set -e

#TARGET_SOC=imx8mm
#TARGET_SOC=imx6q
TARGET_SOC=pico
TOP=`pwd`
# =====================================
# Cross compile tools path define 
# default path is /opt/cross if not define CROSS_COMPILE_PATH
# =====================================
if [ -z $CROSS_COMPILE_PATH ];then
    [ -d /media/tom/ext2t/freescale/cross-compile ] && CROSS_COMPILE_PATH=/media/tom/ext2t/freescale/cross-compile
fi

if [ -z $CROSS_COMPILE_PATH ];then
	[ -d /opt/cross ] && CROSS_COMPILE_PATH=/opt/cross
fi
# =====================================
# Cross compile tools version
# Configue file select
# =====================================
case "${TARGET_SOC}" in
	"imx8mm")
		export ARCH=arm64
		export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
		#U_BOOT_DEFAULT_CONFIG=imx8mm_cse_defconfig
		#DEVICE_TREE=rtx-imx8mm-cse
		U_BOOT_DEFAULT_CONFIG=imx8mm_evk_defconfig
		DEVICE_TREE=fsl-imx8mm-evk
	;;
	"imx6q")
		export ARCH=arm
		export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
		U_BOOT_DEFAULT_CONFIG=rtx_pico_b21_imx6q_1g_defconfig
		#U_BOOT_DEFAULT_CONFIG=mx6ul_14x14_evk_spl_defconfig
	;;
	"pico")  #DDR 1G or 2G need modify include/configs/retronix/rtx_pitxP_mx6q.h #define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024) for 1G ; #define PHYS_SDRAM_SIZE		(2u * 1024 * 1024 * 1024) for 2G
		export ARCH=arm
		export CROSS_COMPILE=${CROSS_COMPILE_PATH}/gcc-linaro-7.4.1-2019.02-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
		U_BOOT_DEFAULT_CONFIG=rtx_pitxP_mx6q_nanya1g_dtb_rtx_all_linux_defconfig
		#~ U_BOOT_DEFAULT_CONFIG=rtx_pitxP_mx6q_micro2g_dtb_rtx_all_linux_defconfig
		#U_BOOT_DEFAULT_CONFIG=mx6qsabresd_optee_defconfig
	;;
	*)
		echo "${TARGET_SOC} not support."
		exit 1
	;;
esac

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

		case "${TARGET_SOC}" in
			"imx8mm")
				if [ ! -f .config ] ; then
					echo " make ${U_BOOT_DEFAULT_CONFIG} "
					make ${U_BOOT_DEFAULT_CONFIG}
				else
					echo " make .config "
				fi
				make -j${CPUS}


				./install_uboot_imx8.sh ${DEVICE_TREE}
				if [ -f imx-mkimage/iMX8M/flash.bin ] ; then
					cp imx-mkimage/iMX8M/flash.bin out/.
					sync
				fi
				;;
			"imx6q")
				if [ ! -f .config ] ; then
					echo " make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG} "
					make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG}
					#make ${U_BOOT_DEFAULT_CONFIG}
				else
					echo " make .config "
				fi

				#make ${U_BOOT_DEFAULT_CONFIG}
				make -j${CPUS}
				dd if=/dev/zero of=u-boot.imx bs=1K count=640
				sync
				dd if=SPL of=u-boot.imx bs=1K conv=notrunc
				sync
				dd if=u-boot.img of=u-boot.imx bs=1K seek=68 conv=notrunc
				sync
				;;
			"pico")
				if [ ! -f .config ] ; then
					echo " make ${U_BOOT_DEFAULT_CONFIG} "
					make ${U_BOOT_DEFAULT_CONFIG}
				else
					echo " make .config "
				fi
				make -j${CPUS}
				;;
			*) 
				echo "${TARGET_SOC} not support."
				exit 1
				;;
		esac
		;;
	"config")
		make ${U_BOOT_DEFAULT_CONFIG}
		;;
	"menuconfig")
		case "${TARGET_SOC}" in
			"imx8mm" | "pico")
				if [ ! -f .config ] ; then
					make ${U_BOOT_DEFAULT_CONFIG}
				fi
				;;
			"imx6q")
				if [ ! -f .config ] ; then
					make SRCARCH=../Retronix ${U_BOOT_DEFAULT_CONFIG}
				fi
				;;
			*) 
				echo "${TARGET_SOC} not support."
				exit 1
				;;
		esac

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
		echo "${0} [all/config/menuconfig/clean/distclean/tools]"
		#exit 1
		;;
esac
