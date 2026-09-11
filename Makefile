TARGET = output
CPU = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = $(CPU) -O0 -g3 -Wall -Wextra \
         -IDrivers/CMSIS/Include \
         -IDrivers/CMSIS/Device/ST/STM32G4xx/Include \
         -DSTM32G474xx

LDFLAGS = $(CPU) -Tstm32g474re.ld --specs=nano.specs -lc -lgcc -nostdlib

SRC = Core/Src/main.c

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o build/$(TARGET).elf
	$(OBJCOPY) -O binary build/$(TARGET).elf build/$(TARGET).bin

clean:
	rm -rf build/*