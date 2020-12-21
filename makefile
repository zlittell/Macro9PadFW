# describe project
PROJECT = Macro9PadFW
MCU = SAMD11D14AM
BUILDBASEDIR = _build
DEBUG := 0

# commands setup
CC = arm-none-eabi-gcc # C Compiler
LD = arm-none-eabi-ld #  Linker
OBJCOPY = arm-none-eabi-objcopy # Final Binary Builder
SIZE = arm-none-eabi-size # Size tool
RM      = rm -rf # Remove recursively command

# BUILD TYPE DIFFERENTIATION
ifeq ($(DEBUG), 1)
BUILDDIR = $(BUILDBASEDIR)/DEBUG
COMPILEFLAGS += \
  -DDEBUG \
  -Og \
  -g3 
else
BUILDDIR = $(BUILDBASEDIR)/RELEASE
COMPILEFLAGS += \
  -DNDEBUG \
  -Os \
  -Werror
endif

# list of source files
C_SRCS +=  \
src/startup/startup_samd11.c \
src/startup/system_samd11.c \
src/drivers/MSF_I2C/MSF_SAMD11_I2C.c \
src/drivers/MSF_NVM_FLASH/MSF_NVM_FLASH_SAMD11.c \
src/drivers/MSF_RGB/MSF_RGB_PCA9632.c \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hal/src/hal_atomic.c \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hpl/gclk/hpl_gclk.c \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hpl/pm/hpl_pm.c \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hpl/sysctrl/hpl_sysctrl.c \
src/drivers/tinyusb/src/class/dfu/dfu_rt_device.c \
src/drivers/tinyusb/src/class/hid/hid_device.c \
src/drivers/tinyusb/src/class/vendor/vendor_device.c \
src/drivers/tinyusb/src/common/tusb_fifo.c \
src/drivers/tinyusb/src/device/usbd.c \
src/drivers/tinyusb/src/device/usbd_control.c \
src/drivers/tinyusb/src/portable/microchip/samd/dcd_samd.c \
src/drivers/tinyusb/src/tusb.c \
src/macropad.c \
src/main.c \
src/serialnumber.c \
src/sys_samd11.c \
src/USBCallbacks.c \
src/usb_descriptors.c \
src/version.c

# convert c files to list of .o files
OBJ += $(C_SRCS:.c=.o)
OBJASPATH = $(addprefix $(BUILDDIR)/obj/, $(OBJ))

# folders to include for finding header files
INC += \
src/include \
src/drivers/tinyusb/src \
src/drivers/tinyusb/hw  \
src/drivers/tinyusb/hw/mcu/microchip/samd11 \
src/drivers/tinyusb/hw/mcu/microchip/samd11/config \
src/drivers/tinyusb/hw/mcu/microchip/samd11/include \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hal/include \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hal/utils/include \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hpl/pm \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hpl/port \
src/drivers/tinyusb/hw/mcu/microchip/samd11/hri \
src/drivers/tinyusb/hw/mcu/microchip/samd11/CMSIS/Core/Include \
src/drivers

#<------------------FILE COMPILATION FLAGS--------------------->
COMPILEFLAGS += \
	-x c \
	-mthumb \
	-D__SAMD11D14AM__ \
	-ffunction-sections \
	-mlong-calls \
	-mcpu=cortex-m0plus \
	-std=gnu99

COMPILEFLAGS += \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
  	-Wstrict-prototypes \
	-Wfatal-errors \
	-Wfloat-equal \
	-Wshadow \
	-Wwrite-strings \
	-Wmissing-format-attribute \
	-Wunreachable-code \
	-Wcast-align

# add all include folders to cflags with -I prefix
COMPILEFLAGS += $(addprefix -I,$(INC))

LDFLAGS += \
  -mthumb \
	-Wl,-Map="$(BUILDDIR)/bin/Macro9Pad.map" \
	--specs=nano.specs \
	--specs=nosys.specs \
	-L"./src/startup" \
	-Wl,--gc-sections \
	-mcpu=cortex-m0plus \
	-T"./src/startup/samd11d14am_flash.ld"

.DEFAULT_GOAL := all
all: $(BUILDDIR)/bin/$(PROJECT).bin size

#Compile individual object files without linking
%.o:%.c
	$(info Building $<)
	mkdir -p $(BUILDDIR)/obj/$(@D)
	$(CC) -c $(COMPILEFLAGS) -o "$(BUILDDIR)/obj/$@" "$<"

$(BUILDDIR)/bin/$(PROJECT).elf: $(OBJ)
	$(info Building Elf File)
	mkdir -p $(@D)
	$(CC) -o $@ $(OBJASPATH) $(LDFLAGS)

$(BUILDDIR)/bin/$(PROJECT).hex: $(BUILDDIR)/bin/$(PROJECT).elf
	$(info Building Hex File)
	$(OBJCOPY) -O ihex $< $@

$(BUILDDIR)/bin/$(PROJECT).bin: $(BUILDDIR)/bin/$(PROJECT).hex
	$(info Building Bin File)
	$(OBJCOPY) -I ihex $< -O binary $@

size: $(BUILDDIR)/bin/$(PROJECT).elf
	-@echo ''
	@$(SIZE) $<
	-@echo ''

.PHONY: all clean
clean:
	$(info Removing build)
	$(RM) $(BUILDBASEDIR)