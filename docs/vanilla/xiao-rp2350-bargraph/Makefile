############################################################################
# xiao-rp2350-bargraph/Makefile
#
# SPDX-License-Identifier: Apache-2.0
#
# Out-of-tree build & test driver for the Seeed Studio XIAO RP2350
# Potentiometer + 6-LED Bar-Graph Apache NuttX firmware.
############################################################################

PROJECT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
WS_DIR      := $(abspath $(PROJECT_DIR)/..)
NUTTX_DIR   := $(WS_DIR)/nuttx
APPS_DIR    := $(WS_DIR)/apps
BOARD_CFG   := ../xiao-rp2350-bargraph/board/configs/bargraph

HOSTCC      ?= cc
HOSTCFLAGS  ?= -std=c11 -O2 -g -Wall -Wextra -Werror -I$(PROJECT_DIR)/include

BUILD_DIR   := $(PROJECT_DIR)/build
TEST_BIN    := $(BUILD_DIR)/test_bargraph
UF2_OUT     := $(NUTTX_DIR)/nuttx.uf2
ROMFS_C     := $(PROJECT_DIR)/board/src/rp23xx_romfs.c
ETC_DIR     := $(PROJECT_DIR)/board/src/etc

.PHONY: all firmware test configure romfs clean distclean

all: firmware

$(ROMFS_C): $(ETC_DIR)/init.d/rc.sysinit $(ETC_DIR)/init.d/rcS
	@genromfs -f $(BUILD_DIR)/romfs.img -d $(ETC_DIR) -V "NSHInitVol" 2>/dev/null || \
	  (mkdir -p $(BUILD_DIR) && genromfs -f $(BUILD_DIR)/romfs.img -d $(ETC_DIR) -V "NSHInitVol")
	@{ \
	  echo '#include <nuttx/compiler.h>'; \
	  (cd $(BUILD_DIR) && xxd -i romfs.img) | \
	    sed -e 's/^unsigned char romfs_img/const unsigned char aligned_data(4) romfs_img/g' \
	        -e 's/^unsigned int romfs_img_len/const unsigned int romfs_img_len/g'; \
	} > $(ROMFS_C)
	@rm -f $(BUILD_DIR)/romfs.img

romfs: $(ROMFS_C)

configure: $(ROMFS_C)
	@ln -sfn ../xiao-rp2350-bargraph/apps $(APPS_DIR)/external
	@if [ ! -f "$(NUTTX_DIR)/.config" ] || ! cmp -s "$(PROJECT_DIR)/board/configs/bargraph/defconfig" "$(NUTTX_DIR)/defconfig"; then \
		cd "$(NUTTX_DIR)" && ./tools/configure.sh -E $(BOARD_CFG); \
	fi

firmware: configure
	$(MAKE) -C "$(NUTTX_DIR)"
	@test -f "$(UF2_OUT)" && echo "Built firmware: $(UF2_OUT)"

$(TEST_BIN): $(PROJECT_DIR)/src/bargraph_logic.c $(PROJECT_DIR)/tests/test_bargraph.c $(PROJECT_DIR)/include/bargraph_logic.h
	@mkdir -p "$(BUILD_DIR)"
	$(HOSTCC) $(HOSTCFLAGS) $(PROJECT_DIR)/src/bargraph_logic.c $(PROJECT_DIR)/tests/test_bargraph.c -o "$(TEST_BIN)"

test: $(TEST_BIN)
	"$(TEST_BIN)"

clean:
	rm -rf "$(BUILD_DIR)"
	@if [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" clean; \
	fi

distclean:
	rm -rf "$(BUILD_DIR)"
	@if [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" distclean; \
	fi
	rm -f "$(APPS_DIR)/external"
