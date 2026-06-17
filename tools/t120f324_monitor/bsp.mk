BSP_PATH ?= ${STANDALONE}/../../bsp/${BSP}
CFLAGS += -I${BSP_PATH}/include
CFLAGS += -I${BSP_PATH}/app
CFLAGS += -I${STANDALONE}/${PROJ_NAME}/include

include ${BSP_PATH}/include/soc.mk

LDSCRIPT ?= ${STANDALONE}/$(PROJ_NAME)/default.ld