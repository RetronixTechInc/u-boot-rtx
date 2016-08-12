CC      = ${CROSS_COMPILE}gcc
AS      = ${CROSS_COMPILE}gcc
LD      = ${CROSS_COMPILE}gcc
SIZE    = ${CROSS_COMPILE}size
NM      = ${CROSS_COMPILE}nm
OBJDUMP = ${CROSS_COMPILE}objdump
OBJCOPY = ${CROSS_COMPILE}objcopy
RM      = rm

#compile include file
CFLAGS = -static -O2 -Wall -Ih
#CFLAGS += -DKERNEL_ANDROID

LDFLAGS = -static

OUTPUT   = ./out

#The mcu.h file from xxd -i mcu.bin mcu.h, that mcu.bin is mcu update file. 
EXE = $(OUTPUT)/rtx_setting

#depence name for $(OBJS) is rtx_setenv.o datalist.o crc32.o
o/%.o : %.c
	$(CC) -c $(CFLAGS)  $< -o $@
	
#first is default build target
all:makepath $(EXE)

#makepath
makepath:
	mkdir -p $(OUTPUT)

#depence name for $(EXE) are EXE += $(OUTPUT)/mp EXE += $(OUTPUT)/efm32fn EXE += $(OUTPUT)/ASIXeeprom EXE += $(OUTPUT)/mcuupdate
$(OUTPUT)/% : %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean: distclean
distclean:
	rm -rf $(OUTPUT)




