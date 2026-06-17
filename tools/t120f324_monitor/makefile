PROJ_NAME=monitor

STANDALONE = ..

SRCS = 	$(wildcard src/*.c) \
		$(wildcard src/*.cpp) \
		$(wildcard src/*.S) \
        ${STANDALONE}/common/start.S \
        ${STANDALONE}/common/trap.S


include ${STANDALONE}/$(PROJ_NAME)/bsp.mk
include ${STANDALONE}/common/riscv64-unknown-elf.mk
include ${STANDALONE}/common/standalone.mk


