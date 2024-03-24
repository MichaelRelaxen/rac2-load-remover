EE_OBJS_DIR = obj/
EE_BIN_DIR = bin/
EE_SRC_DIR = src/
EE_INC_DIR = include/

EE_ELF = $(EE_BIN_DIR)test.elf
EE_BIN = $(EE_BIN_DIR)38996035
EE_OBJS = main.o timer.o
#EE_LIBS = -luyantsc
#EE_LIBS = -luyapal

ADDRESS = 000F0000
# This is where the code will be written into memory.

EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%)
EE_INCS := -I../../include -I$(PS2SDK)/ports/include
EE_LDFLAGS = -fno-builtin -nostdlib -nostartfiles -L. -L../../lib -L$(PS2SDK)/ports/lib
EE_CFLAGS := -D_PROGRAM_ENTRY=0x$(ADDRESS)

ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
    EE_CFLAGS := $(EE_CFLAGS) -DDEBUG
endif

all: linker $(EE_OBJS_DIR) $(EE_BIN_DIR) $(EE_BIN) code
	
debug: all

$(EE_OBJS_DIR):
	mkdir -p $(EE_OBJS_DIR)

$(EE_BIN_DIR):
	mkdir -p $(EE_BIN_DIR)

$(EE_OBJS_DIR)%.o : $(EE_SRC_DIR)%.c
	$(EE_C_COMPILE) -c $< -o $@

$(EE_OBJS_DIR)%.o : $(EE_SRC_DIR)%.S
	$(EE_C_COMPILE) -c $< -o $@

linker:
	$(EE_CC) -E -x c $(EE_CFLAGS) linkfile.template | grep -v '^#' > linkfile

code:
	bin2code $(EE_BIN) $(ADDRESS) $(EE_BIN).raw raw-writeonce
	bin2code $(EE_BIN) $(ADDRESS) $(EE_BIN).pnach pnach-writeonce
	cat template/footer.raw >> $(EE_BIN).raw
	cat template/footer.pnach >> $(EE_BIN).pnach

clean:
	rm -rf linkfile
	rm -rf crt0.o
	rm -f -r $(EE_OBJS_DIR)
	rm -f -r $(EE_BIN_DIR)

include $(CURDIR)/Makefile.pref
include $(CURDIR)/Makefile.eeglobal
