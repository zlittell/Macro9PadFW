# describe project
PROJECT = Macro9PadFW
MCU = SAMD11D14AM
BUILD = _build

# commands setup
CC = arm-none-eabi-gcc # C Compiler
LD = arm-none-eabi-ld #  Linker
OBJCOPY = arm-none-eabi-objcopy # Final Binary Builder
SIZE = arm-none-eabi-size # Size tool
RM      = rm -rf # Remove recursively command

# list of source files
C_SRCS +=  \
Macro9Pad/Device_Startup/startup_samd11.c \
Macro9Pad/Device_Startup/system_samd11.c \
Macro9Pad/Drivers/MSF_I2C/MSF_SAMD11_I2C.c \
Macro9Pad/Drivers/MSF_NVM_FLASH/MSF_NVM_FLASH_SAMD11.c \
Macro9Pad/Drivers/MSF_RGB/MSF_RGB_PCA9632.c \
Macro9Pad/macropad.c \
Macro9Pad/main.c \
Macro9Pad/serialnumber.c \
Macro9Pad/sys_samd11.c \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hal/src/hal_atomic.c \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hpl/gclk/hpl_gclk.c \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hpl/pm/hpl_pm.c \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hpl/sysctrl/hpl_sysctrl.c \
Macro9Pad/tinyusb/src/class/dfu/dfu_rt_device.c \
Macro9Pad/tinyusb/src/class/hid/hid_device.c \
Macro9Pad/tinyusb/src/class/vendor/vendor_device.c \
Macro9Pad/tinyusb/src/common/tusb_fifo.c \
Macro9Pad/tinyusb/src/device/usbd.c \
Macro9Pad/tinyusb/src/device/usbd_control.c \
Macro9Pad/tinyusb/src/portable/microchip/samd/dcd_samd.c \
Macro9Pad/tinyusb/src/tusb.c \
Macro9Pad/USBCallbacks.c \
Macro9Pad/usb_descriptors.c \
Macro9Pad/version.c

# convert c files to list of .o files
OBJ += $(C_SRCS:.c=.o)
OBJASPATH = $(addprefix $(BUILD)/obj/, $(OBJ))

# folders to include for finding header files
INC += \
Macro9Pad/ \
Macro9Pad/tinyusb/src \
Macro9Pad/tinyusb/hw  \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11 \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/config \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/include \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hal/include \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hal/utils/include \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hpl/pm \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hpl/port \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/hri \
Macro9Pad/tinyusb/hw/mcu/microchip/samd11/CMSIS/Core/Include \
Macro9Pad/Drivers

#<------------------FILE COMPILATION FLAGS--------------------->
COMPILEFLAGS += \
	-x c \
	-mthumb \
	-D__SAMD11D14AM__ \
	-ffunction-sections \
	-mlong-calls \
	-Wall \
	-mcpu=cortex-m0plus \
	-std=gnu99

# Debugging/Optimization
ifeq ($(DEBUG), 1)
COMPILEFLAGS += \
  -DDEBUG \
  -Og \
  -g3 
else
COMPILEFLAGS += \
  -DNDEBUG \
  -Os
endif

# add all include folders to cflags with -I prefix
COMPILEFLAGS += $(addprefix -I,$(INC))

LDFLAGS += \
  -mthumb \
	-Wl,-Map="Macro9Pad.map" \
	--specs=nano.specs \
	--specs=nosys.specs \
	-L"./Macro9Pad/Device_Startup" \
	-Wl,--gc-sections \
	-mcpu=cortex-m0plus \
	-T./Macro9Pad/Device_Startup/samd11d14am_flash.ld

LINKER_SCRIPT_DEP+=  \
../Device_Startup/samd11d14am_flash.ld \
../Device_Startup/samd11d14am_sram.ld

.DEFAULT_GOAL := all
all: $(BUILD)/bin/$(PROJECT).elf size

#Compile individual object files without linking
$(info Building Object Files)
%.o:%.c
	$(info Building $<)
	mkdir -p $(BUILD)/obj/$(@D)
	$(CC) -c $(COMPILEFLAGS) -o "$(BUILD)/obj/$@" "$<"

$(info Building ELF file)
$(BUILD)/bin/$(PROJECT).elf: $(OBJ)
	mkdir -p $(@D)
	$(CC) -o $@ $(OBJASPATH) $(LDFLAGS)

size: $(BUILD)/bin/$(PROJECT).elf
	-@echo ''
	@$(SIZE) $<
	-@echo ''

.PHONY: all clean
clean:
	$(info Removing build)
	$(RM) $(BUILD)