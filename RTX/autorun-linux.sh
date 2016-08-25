#! /bin/sh

#===================================
#MYPATH : storage mount path
#MYPATHSD : partition file temp path
#===================================
MYPATH="/data"
MYPATHSD="/tmp"

#===================================
#DEVNUM : defaut storage
#EMMC_PATH : mmc cotroller path
#EMMC_VALUE : where is boot part.
#	0 : normal user data.
#	8 : bootpart1
#	16 : bootpart2
#===================================
DEVNUM=0
DEVNODE="/dev/mmcblk${DEVNUM}"
EMMC_PATH="/sys/class/mmc_host"
EMMC_VALUE=8

#===================================
#efm32cmd : mcu command
#CHECK_MCU : update mcu if version un-correct.
#MCU_VER : correct mcu version
#===================================
efm32cmd=`ls /data/iMX6-A6*`
CHECK_MCU="TRUE"
MCU_VER="160308"

#===================================
#CLEANSET : Errase Setting info at 768KB, Len 1KB
#===================================
CLEANSET="TRUE"

#===================================
#Storage Parttion define
#MBR_SIZE : P0
#ROOTFS_SIZE : P1
#REV_SIZE : Reserve
# ==================================
MBR_SIZE=64
ROOTFS_SIZE=2048
REV_SIZE=64

# =================================
#MESSAGE_PORT : ttymxc message port
# ==================================
MESSAGE_PORT="/dev/ttymxc0"

print() {
	echo $1 > ${MESSAGE_PORT}
	echo $1 > /dev/tty1
}

run_end() {
	cd ${MYPATH}
	print "update Fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	exit 0
}

umount_sd()
{
	mounted=$(mount | grep ${DEVNODE} | awk '{print $1}' | wc -l)
    if [ $mounted -gt 0 ]; then
		todo=$(mount | grep ${DEVNODE} | awk '{print $1}')
        for i in $todo
			do
                echo $i
				umount $i
                sleep 1
			done
	fi
}

run() {
print "update system start........."
if [ "$efm32cmd"x != x ]; then
print "efm32 command is $efm32cmd"
$efm32cmd 2 12 -sg 0
efmstr=`$efm32cmd 2 12 -v`
efmlen=${#efmstr}
efmstart=$(expr $efmlen - 6)
efmver=${efmstr:$efmstart:6}
print "efm32 version is $efmver"
if [ "$CHECK_MCU"x = TRUEx ]; then
if [ "$efmver"x = x ]; then
	print "efm32 get version command fail."
else
if [ "$efmver" != ${MCU_VER} ]; then
	print "efm32 updating......"
	$efm32cmd 2 12 -u
	sleep 1
	$efm32cmd 2 12 -u
	sleep 1
fi
fi
fi
fi
sync

cd $MYPATH

UBOOT=`ls /data/flash/u-boot*`
KERNEL=`ls /data/flash/uImage*`
DTB=`ls /data/flash/imx*.dtb`
ENGKERNEL=`ls /data/flash/enguImage*`
ENGDTB=`ls /data/flash/engdtb.dtb`
ENGRAMDISK=`ls /data/flash/engramdisk*`
ENGRECOVERY=`ls /data/flash/engrecovery*`
ROOTFS=`ls /data/flash/debian-*`

# check file is exist
if [ "$UBOOT"x = "x" ] ;    then run_end ; fi
if [ "$KERNEL"x = "x" ] ;   then run_end ; fi

# check emmc_path if EMMC_PATH have define
if [ ! -z $EMMC_PATH ] ;  then
	
	for i in 0 3 2 1 ; do
		if [ -z ${emmc_path} ] ; then
			emmc_path=`ls ${EMMC_PATH}/mmc${i}/mmc*/boot_info`
			if [ ! -z ${emmc_path} ] ; then
				DEVNUM=${i}
				DEVNODE="/dev/mmcblk${i}"
				emmc_conf=`ls ${EMMC_PATH}/mmc${i}/mmc*/boot_config`
				emmc_bus=`ls ${EMMC_PATH}/mmc${i}/mmc*/boot_bus_config`
				break ;
			fi
		fi
	done
fi

#umount $DEVNODE
umount_sd

# cal size
TOTAL_SIZE=`fdisk -l ${DEVNODE} | grep "Disk ${DEVNODE}:" | awk '{print $5}'`
UNITS_SIZE=`fdisk -l ${DEVNODE} | grep 'Units = cylinders' | awk '{print $9}'`

MBR_SIZE=`expr $MBR_SIZE \* 1024 \* 1024`
ROOTFS_SIZE=`expr $ROOTFS_SIZE \* 1024 \* 1024`
REV_SIZE=`expr $REV_SIZE \* 1024 \* 1024`

if [ $TOTAL_SIZE -gt 3000000000 ] ; then
	DATA_SIZE=`expr $TOTAL_SIZE - $MBR_SIZE - $ROOTFS_SIZE - $REV_SIZE`
else
	run_end
fi

print "clean MBR"
# clean MBR
dd if=/dev/zero of=${DEVNODE} bs=1024 count=512
print "clean 1M~64M"

if [ $CLEANSET == "TRUE" ] ;  then
	dd if=/dev/zero of=${DEVNODE} bs=1024 seek=768 count=1
fi

# clean clean 1M~64M
dd if=/dev/zero of=${DEVNODE} bs=1M seek=1 count=60

# u-boot write to eMMC
if [ ! -z ${emmc_conf} ] ; then
#boot_info=`cat ${emmc_path}`
echo "${EMMC_VALUE}" > ${emmc_conf}
	if [ ${EMMC_VALUE} -eq 8 ] ; then
		echo 0 > /sys/class/block/mmcblk${DEVNUM}boot0/force_ro
		if [ "$RTXBOOT"x == "x" ] ;  then
			dd if=${UBOOT} of=${DEVNODE}boot0 bs=1024 seek=1 || run_end
		else
			dd if=${RTXBOOT} of=${DEVNODE}boot0 bs=1024 skip=1 seek=1 || run_end
			dd if=${UBOOT} of=${DEVNODE}boot0 bs=1024 seek=256 || run_end	
		fi
		echo 1 > /sys/class/block/mmcblk${DEVNUM}boot0/force_ro
	elif [ ${EMMC_VALUE} -eq 16 ] ; then
		echo 1 > /sys/class/block/mmcblk${DEVNUM}boot1/force_ro
		if [ "$RTXBOOT"x == "x" ] ;  then
			dd if=${UBOOT} of=${DEVNODE}boot1 bs=1024 seek=1 || run_end
		else
			dd if=${RTXBOOT} of=${DEVNODE}boot1 bs=1024 skip=1 seek=1 || run_end
			dd if=${UBOOT} of=${DEVNODE}boot1 bs=1024 seek=256 || run_end	
		fi	
		echo 1 > /sys/class/block/mmcblk${DEVNUM}boot1/force_ro
	else
		if [ "$RTXBOOT"x == "x" ] ;  then
			dd if=${UBOOT} of=${DEVNODE} bs=1024 seek=1 || run_end
		else
			dd if=${RTXBOOT} of=${DEVNODE} bs=1024 skip=1 seek=1 || run_end
			dd if=${UBOOT} of=${DEVNODE} bs=1024 seek=256 || run_end	
		fi
	fi
else
	if [ "$RTXBOOT"x == "x" ] ;  then
		dd if=${UBOOT} of=${DEVNODE} bs=1024 seek=1 || run_end
	else
		dd if=${RTXBOOT} of=${DEVNODE} bs=1024 skip=1 seek=1 || run_end
		dd if=${UBOOT} of=${DEVNODE} bs=1024 seek=256 || run_end	
	fi
fi

sync

print "fdisk ${DEVNODE}"
rm -f ${MYPATHSD}/.SD_PARTITION
touch ${MYPATHSD}/.SD_PARTITION
# - P1 ----------------------------------
MBR_SIZE=`expr $MBR_SIZE / $UNITS_SIZE`
ROOTFS_SIZE=`expr $MBR_SIZE + $ROOTFS_SIZE / $UNITS_SIZE`

echo "		n" >> ${MYPATHSD}/.SD_PARTITION
echo "		p" >> ${MYPATHSD}/.SD_PARTITION
echo "		1" >> ${MYPATHSD}/.SD_PARTITION
echo "		$MBR_SIZE" >> ${MYPATHSD}/.SD_PARTITION
echo "		$ROOTFS_SIZE" >> ${MYPATHSD}/.SD_PARTITION
echo "" >> ${MYPATHSD}/.SD_PARTITION

# - P2 ----------------------------------
ROOTFS_SIZE=`expr $ROOTFS_SIZE + 1`
DATA_SIZE=`expr $ROOTFS_SIZE + $DATA_SIZE / $UNITS_SIZE`

echo "		n" >> ${MYPATHSD}/.SD_PARTITION
echo "		p" >> ${MYPATHSD}/.SD_PARTITION
echo "		2" >> ${MYPATHSD}/.SD_PARTITION
echo "		$ROOTFS_SIZE" >> ${MYPATHSD}/.SD_PARTITION
echo "		$DATA_SIZE" >> ${MYPATHSD}/.SD_PARTITION
echo "" >> ${MYPATHSD}/.SD_PARTITION

# - Finish write-------------------------
echo "		w" >> ${MYPATHSD}/.SD_PARTITION
echo "" >> /.SD_PARTITION

fdisk ${DEVNODE} < ${MYPATHSD}/.SD_PARTITION
sync
sleep 5

print "format ${DEVNODE}p?"
mke2fs -t ext4 -L rootfs ${DEVNODE}p1
mke2fs -t ext4 -L home ${DEVNODE}p2

sync

# engkernel image at 1MB
if [ "$ENGKERNEL"x != "x" ] ;  then
dd if=$ENGKERNEL of=${DEVNODE} bs=1M seek=1 || run_end
sync
fi
# engdtb.dtb at 11.5MB
if [ "$ENGDTB"x != "x" ] ;  then
dd if=$ENGDTB of=${DEVNODE} bs=1024 seek=11776 || run_end
sync
fi

# kernel.dtb at 12.5MB
if [ "$DTB"x != "x" ] ;  then
dd if=$DTB of=${DEVNODE} bs=1024 seek=12800 || run_end
sync
fi

# kernel image at 13MB
if [ "$KERNEL"x != "x" ] ;  then
dd if=$KERNEL of=${DEVNODE} bs=1024 seek=13312 || run_end
sync
fi

# uramdisk image at 24MB
if [ "$URAMDISK"x != "x" ] ;  then
dd if=$URAMDISK of=${DEVNODE} bs=1M seek=24 || run_end
sync
fi

# engrecovery image at 26MB
if [ "$ENGRECOVERY"x != "x" ] ;  then
dd if=$ENGRECOVERY of=${DEVNODE} bs=1M seek=26 || run_end
sync
fi

# engramdisk image at 30MB
if [ "$ENGRAMDISK"x != "x" ] ;  then
dd if=$ENGRAMDISK of=${DEVNODE} bs=1M seek=30 || run_end
sync
fi

# boot image: inclue uImage and ramdisk filesystem
if [ "$BOOT"x != "x" ] ;  then
dd if=$BOOT of=${DEVNODE}p1 bs=1M || run_end
sync
fi

# recovery image: are inclue uImage and recovery filesystem
if [ "$RECOVERY"x != "x" ] ;  then
dd if=$RECOVERY of=${DEVNODE}p2 bs=1M || run_end
sync
fi

# rootfs: It will be mounted as "/"
if [ "$ROOTFS"x != "x" ] ;  then
	print "  mount ${DEVNODE}p1"
	mount ${DEVNODE}p1 /mnt
	rm -rf /mnt/*
	
	print "  tar ${ROOTFS}"
	tar -xpvf ${ROOTFS} -C /mnt/
	sync
	sync
	sync
	umount /mnt
	sync
	sync
	sync
else
	echo "can't find any file system !!!!!" 
	run_end
fi

} > /dev/tty1

run
#mv /data/check_code /data/check_code-finish
sync
print "update system finished......"
sleep 3
print "Please Reboot System......"

#reboot

