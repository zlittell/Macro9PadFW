# describe project
PROJECT = Macro9PadFW
MCU = SAMD11D14AM
BUILD = _build

# commands setup
CC = arm-none-eabi-gcc # C Compiler
LD = arm-none-eabi-ld #  Linker
OBJCOPY = arm-none-eabi-objcopy # Final Binary Builder
FLASHER = lm4flash # Flashing utility
RM      = rm -rf # Remove recursively command
MKDIR   = @mkdir -p $(@D) # Creates folders if not present

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
OBJ += $(addprefix $(BUILD)/obj/, $(SRC_S:.s=.o))
OBJ += $(addprefix $(BUILD)/obj/, $(SRC_C:.c=.o))

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

# Set up compiler flags
CFLAGS += -mcpu=cortex-m0plus -mthumb
CFLAGS += -D__SAMD11D14AM__
CFLAGS += -Wall -Werror -std=c11
CFLAGS += -O0
CFLAGS += -g
#CFLAGS += -specs=nosys.specs -specs=nano.specs
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections 
CFLAGS += -Wl,-Map=$(PROJECT).map

# Add linker files as flags with -T prefix
CFLAGS += -T Macro9Pad/Device_Startup/samd11d14am_flash.ld

# add all include folders to cflags with -I prefix
CFLAGS += $(addprefix -I,$(INC))

.DEFAULT_GOAL := all
all: $(BUILD)/bin/$(PROJECT).bin

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD)/bin/$(PROJECT).elf: $(OBJ)
	$(MKDIR)
	$(LD) -o $@ $^ $(CFLAGS)

$(BUILD)/bin/$(PROJECT).bin: $(BUILD)/bin/$(PROJECT).elf
	$(OBJCOPY) -o binary $@ $<

.PHONY: all clean
clean:
	$(RM) -rf $(BUILD)