EE_BIN = gctimer.elf
IRX = $(PS2SDK)/iop/irx
IP_ADDR = 10.0.1.100
GUI = gui

EE_OBJS = src/main.o src/timer.o

EE_OBJS += poweroff.o
EE_OBJS += elf_loader.o

EE_SRC += poweroff.s
EE_SRC += elf_loader.s

EE_INCS = -I$(PS2DEV)/gsKit/include -I$(PS2SDK)/ports/include -I$(PS2SDK)/sbv/include
EE_LDFLAGS = -nostartfiles -L$(PS2DEV)/gsKit/lib -L$(PS2SDK)/ports/lib \
	-L$(PS2SDK)/sbv/lib -L. -s
EE_LIBS = -Xlinker --start-group -lps2ip -lpatches -lgskit -ldmakit -lm -ldebug -lkernel -Xlinker --end-group

all: $(EE_BIN)
	make clean
 
clean:
	rm -f $(EE_OBJS) $(EE_SRC)

poweroff.s:
	bin2s $(IRX)/poweroff.irx poweroff.s poweroff_irx

elf_loader.s:
	bin2s elf_loader/elf_loader.elf elf_loader.s elf_loader

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
