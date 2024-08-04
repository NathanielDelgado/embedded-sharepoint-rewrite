######################################
# inputs
######################################
ifndef PROJECT_TARGET
$(error PROJECT_TARGET is not defined. Please provide a target to build for.)
endif

ifndef PROJECT_BUILD_DIR
$(error PROJECT_BUILD_DIR is not defined. Please provide a place to build.)
endif

ifndef PROJECT_C_SOURCES
$(error PROJECT_C_SOURCES is not defined. Please provide a set of C source files.)
endif

ifndef PROJECT_C_INCLUDES
$(error PROJECT_C_INCLUDES is not defined. Please provide a set of C header files.)
endif


######################################
# target
######################################
ifeq ($(PROJECT_TARGET),stm32f413rht)
	TARGET_C_SOURCES = \
	$(filter-out %template.c, $(wildcard stm32f413rht/STM32F4xx_HAL_Driver/Src/*.c))
	TARGET_ASM_SOURCES = \
	stm32f413rht/startup_stm32f413xx.s
	TARGET_C_DEFS = \
	STM32F413xx
	TARGET_C_INCLUDES = \
	stm32f413rht/STM32F4xx_HAL_Driver/Inc \
	stm32f413rht/STM32F4xx_HAL_Driver/Inc/Legacy \
	stm32f413rht/CMSIS/Device/ST/STM32F4xx/Include \
	stm32f413rht/CMSIS/Include
	TARGET_LDSCRIPT = \
	stm32f413rht/STM32F413RHTx_FLASH.ld
else ifeq ($(PROJECT_TARGET),stm32f446ret)
	TARGET_C_SOURCES = \
	$(filter-out %template.c, $(wildcard stm32f446ret/STM32F4xx_HAL_Driver/Src/*.c))
	TARGET_ASM_SOURCES = \
	stm32f446ret/startup_stm32f446xx.s
	TARGET_C_DEFS = \
	STM32F446xx
	TARGET_C_INCLUDES = \
	stm32f446ret/STM32F4xx_HAL_Driver/Inc \
	stm32f446ret/STM32F4xx_HAL_Driver/Inc/Legacy \
	stm32f446ret/CMSIS/Device/ST/STM32F4xx/Include \
	stm32f446ret/CMSIS/Include
	TARGET_LDSCRIPT = \
	stm32f446ret/STM32F446RETx_FLASH.ld
else ifeq ($(PROJECT_TARGET),stm32l476rgt)
	TARGET_C_SOURCES = \
	$(filter-out %template.c, $(wildcard stm32l476rgt/STM32L4xx_HAL_Driver/Src/*.c))
	TARGET_ASM_SOURCES = \
	stm32l476rgt/startup_stm32l476xx.s
	TARGET_C_DEFS = \
	STM32L476xx
	TARGET_C_INCLUDES = \
	stm32l476rgt/STM32L4xx_HAL_Driver/Inc \
	stm32l476rgt/STM32L4xx_HAL_Driver/Inc/Legacy \
	stm32l476rgt/CMSIS/Device/ST/STM32L4xx/Include \
	stm32l476rgt/CMSIS/Include
	TARGET_LDSCRIPT = \
	stm32l476rgt/STM32L476RGTx_FLASH.ld
else
	$(error PROJECT_TARGET is unsupported. Please choose a supported target to build for.)
endif

TARGET = $(PROJECT_TARGET)


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = $(PROJECT_BUILD_DIR)

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(PROJECT_C_SOURCES) \
$(TARGET_C_SOURCES) \
sysmem.c \
syscalls.c

# ASM sources
ASM_SOURCES =  \
$(TARGET_ASM_SOURCES)

# ASM sources
ASMM_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
$(addprefix -D,$(TARGET_C_DEFS))


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
$(addprefix -I,$(PROJECT_C_INCLUDES)) \
$(addprefix -I,$(TARGET_C_INCLUDES))


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Werror -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(TARGET_LDSCRIPT)

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)


#######################################
# clean up
#######################################
flash:
	-st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000
  

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***