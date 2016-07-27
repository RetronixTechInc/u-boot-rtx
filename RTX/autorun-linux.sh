#! /bin/sh

IPLOG="false"
CLEANSET="true"

MYPATH="/data"
DEVNODE="/dev/mmcblk0"

# ==================================
MBR_SIZE=64
ROOTFS_SIZE=2048
CONFIG_SIZE=64
LOG_SIZE=128
REV_SIZE=16
# =================================

print() {
	echo $1 > /dev/ttymxc0
	echo $1 > /dev/tty1
}

run_end() {
	cd ${MYPATH}
	print "update Fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
	exit 0
}

umount_sd()
{
	mounted=$(mount | grep $DEVNODE | awk '{print $1}' | wc -l)
    if [ $mounted -gt 0 ]; then
		todo=$(mount | grep $DEVNODE | awk '{print $1}')
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
/data/efm32cmd 2 12 -sg 0
cd $MYPATH
RTXBOOT=`ls /data/flash/rtx_boot*`
UBOOT=`ls /data/flash/u-boot*`
KERNEL=`ls /data/flash/uImage*`
FSKERNEL=`ls /data/flash/enguImage*`
FSRAMDISK=`ls /data/flash/engramdisk*`
ROOTFS=`ls /data/flash/rootfs*`
MODULE=`ls /data/flash/module*`
HOME=`ls /data/flash/home*`

# check file is exist
if [ "$UBOOT"x = "x" ] ;    then run_end ; fi
if [ "$KERNEL"x = "x" ] ;   then run_end ; fi
if [ "$FSKERNEL"x = "x" ] ;   then run_end ; fi
if [ "$FSRAMDISK"x = "x" ] ; then run_end ; fi

#umount $DEVNODE
umount_sd

# cal size
TOTAL_SIZE=`fdisk -l /dev/mmcblk0 | grep 'Disk /dev/mmcblk0:' | awk '{print $5}'`
UNITS_SIZE=`fdisk -l /dev/mmcblk0 | grep 'Units = cylinders' | awk '{print $9}'`

MBR_SIZE=`expr $MBR_SIZE \* 1024 \* 1024`
ROOTFS_SIZE=`expr $ROOTFS_SIZE \* 1024 \* 1024`
CONFIG_SIZE=`expr $CONFIG_SIZE \* 1024 \* 1024`
LOG_SIZE=`expr $LOG_SIZE \* 1024 \* 1024`
REV_SIZE=`expr $REV_SIZE \* 1024 \* 1024`

if [ $TOTAL_SIZE -gt 3000000000 ] ; then
	DATA_SIZE=`expr $TOTAL_SIZE - $MBR_SIZE - $ROOTFS_SIZE - $CONFIG_SIZE - $LOG_SIZE - $REV_SIZE`
else
	run_end
fi

print "clean 0K~512K"
# clean MBR
dd if=/dev/zero of=$DEVNODE bs=1024 count=512
print "clean 1M~32M"
# clean PARTITION 0
dd if=/dev/zero of=$DEVNODE bs=1M seek=1 count=30

if [ $CLEANSET == "true" ] ;  then
	dd if=/dev/zero of=$DEVNODE bs=1024 seek=768 count=1
fi

# bootloader: uboot
dd if=$UBOOT of=$DEVNODE bs=1024 seek=1 || run_end
sync

print "fdisk $DEVNODE"
rm -f $MYPATH/.SD_PARTITION
touch $MYPATH/.SD_PARTITION
# - P1 ----------------------------------
MBR_SIZE=`expr $MBR_SIZE / $UNITS_SIZE`
ROOTFS_SIZE=`expr $MBR_SIZE + $ROOTFS_SIZE / $UNITS_SIZE`

echo "		n" >> $MYPATH/.SD_PARTITION
echo "		p" >> $MYPATH/.SD_PARTITION
echo "		1" >> $MYPATH/.SD_PARTITION
echo "		$MBR_SIZE" >> $MYPATH/.SD_PARTITION
echo "		$ROOTFS_SIZE" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

# - P2 ----------------------------------
ROOTFS_SIZE=`expr $ROOTFS_SIZE + 1`
CONFIG_SIZE=`expr $ROOTFS_SIZE + $CONFIG_SIZE / $UNITS_SIZE`

echo "		n" >> $MYPATH/.SD_PARTITION
echo "		p" >> $MYPATH/.SD_PARTITION
echo "		2" >> $MYPATH/.SD_PARTITION
echo "		$ROOTFS_SIZE" >> $MYPATH/.SD_PARTITION
echo "		$CONFIG_SIZE" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

# - E3 ----------------------------------
CONFIG_SIZE=`expr $CONFIG_SIZE + 2`
DATA_SIZE=`expr $CONFIG_SIZE + 6 + $DATA_SIZE / $UNITS_SIZE`

echo "		n" >> $MYPATH/.SD_PARTITION
echo "		e" >> $MYPATH/.SD_PARTITION
echo "		3" >> $MYPATH/.SD_PARTITION
echo "		$CONFIG_SIZE" >> $MYPATH/.SD_PARTITION
echo "		$DATA_SIZE" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

# - P4 ----------------------------------
DATA_SIZE=`expr $DATA_SIZE + 1`
LOG_SIZE=`expr $DATA_SIZE + $LOG_SIZE / $UNITS_SIZE`

echo "		n" >> $MYPATH/.SD_PARTITION
echo "		p" >> $MYPATH/.SD_PARTITION
echo "		$DATA_SIZE" >> $MYPATH/.SD_PARTITION
echo "		$LOG_SIZE" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

# - P5 ----------------------------------
echo "		n" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

# - Finish write-------------------------
echo "		w" >> $MYPATH/.SD_PARTITION
echo "" >> $MYPATH/.SD_PARTITION

fdisk $DEVNODE < $MYPATH/.SD_PARTITION
sync
sleep 5

print "format ${DEVNODE}p?"
mke2fs -t ext4 -L root ${DEVNODE}p1
mke2fs -t ext4 -L config ${DEVNODE}p2
mke2fs -t ext4 -L log ${DEVNODE}p4
mke2fs -t ext4 -O ^extent -L home ${DEVNODE}p5
sync

# kernel image: uImage for recovery
dd if=$FSKERNEL of=${DEVNODE} bs=1M seek=1 || run_end
sync

# engramdisk image: engramdisk support uboot Hot key 'u'
dd if=$FSRAMDISK of=${DEVNODE} bs=1M seek=30 || run_end
sync

# kernel image: uImage for normal
dd if=$KERNEL of=${DEVNODE} bs=1M seek=13 || run_end
sync

if [ "$ROOTFS"x != "x" ] ;  then
	print "  mount /dev/mmcblk0p1"
	mount /dev/mmcblk0p1 /mnt
	rm -rf /mnt/*
	
	print "  tar -xpvf ${ROOTFS}"
	tar -xpf ${ROOTFS} -C /mnt/
	sync
	tar -xpvf ${MODULE} -C /mnt/lib/modules/
	sync
	if [ $IPLOG == "true" ] ;  then
		cp /data/flash/70-syslog-ip.conf /mnt/etc/rsyslog.d/
		sync
	fi
	umount /mnt
	sync
else
	run_end
fi

if [ "$HOME"x != "x" ] ;  then
	print "  mount /dev/mmcblk0p5"
	mount /dev/mmcblk0p5 /mnt
	rm -rf /mnt/*
	
	print "  tar -xpvf ${HOME}"
	tar -xpvf ${HOME} -C /mnt/
	sync
	cp /data/flash/update.sh /mnt/
	cp -rf /data/flash/update /mnt/
	sync
	umount /mnt
	sync
else
	run_end ;
fi

sleep 3
sync
sync
sync

} > /dev/tty1

run
#mv /data/check_code /data/check_code-finish
sync
print "update system finished......"
sleep 3
print "Please Reboot System......"
sync
#reboot

