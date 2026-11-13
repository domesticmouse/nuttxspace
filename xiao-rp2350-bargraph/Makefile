############################################################################
# xiao-rp2350-bargraph/Makefile
############################################################################

NUTTX_DIR  := ../nuttx
APPS_DIR   := ../apps
EXT_LINK   := $(APPS_DIR)/external
EXT_TARGET := ../xiao-rp2350-bargraph/external
BOARD_CONF := ../xiao-rp2350-bargraph/board/configs/bargraph
COMMON_DIR := $(NUTTX_DIR)/boards/arm/rp23xx/common
TEST_BIN   := tests/test_bargraph_logic

.PHONY: all setup configure test clean distclean

all: setup
	@if [ ! -f "$(NUTTX_DIR)/.config" ]; then \
		cd "$(NUTTX_DIR)" && ./tools/configure.sh -e -l "$(BOARD_CONF)"; \
	fi
	$(MAKE) -C "$(NUTTX_DIR)"

setup: external/Makefile
	@if [ -e "$(EXT_LINK)" ] && [ ! -L "$(EXT_LINK)" ]; then \
		echo "error: $(EXT_LINK) exists and is not a symlink" >&2; \
		exit 1; \
	fi
	@created=0; \
	if [ ! -L "$(EXT_LINK)" ] || [ "$$(readlink "$(EXT_LINK)")" != "$(EXT_TARGET)" ]; then \
		ln -sfn "$(EXT_TARGET)" "$(EXT_LINK)"; \
		created=1; \
	fi; \
	if [ "$$created" -eq 1 ] || [ ! -f external/Kconfig ] || [ ! -f "$(APPS_DIR)/Kconfig" ] || [ external/bargraph/Kconfig -nt "$(APPS_DIR)/Kconfig" ]; then \
		rm -f "$(APPS_DIR)/Kconfig" external/Kconfig; \
	fi
	@if [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" olddefconfig; \
	fi

configure: setup
	cd "$(NUTTX_DIR)" && ./tools/configure.sh -e -l "$(BOARD_CONF)"

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): tests/test_bargraph_logic.c external/bargraph/bargraph_logic.h
	gcc -Wall -Wextra -Werror -Iexternal/bargraph -o $(TEST_BIN) tests/test_bargraph_logic.c

clean:
	@if [ -f "$(NUTTX_DIR)/Make.defs" ] && [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" clean; \
	fi
	rm -rf "$(COMMON_DIR)/etctmp" "$(COMMON_DIR)/etctmp.c" "$(TEST_BIN)"

distclean:
	@if [ -f "$(NUTTX_DIR)/Make.defs" ] && [ -f "$(NUTTX_DIR)/.config" ]; then \
		$(MAKE) -C "$(NUTTX_DIR)" distclean; \
	fi
	@if [ -L "$(EXT_LINK)" ]; then \
		rm -f "$(EXT_LINK)"; \
	fi
	rm -f external/Kconfig external/.kconfig "$(APPS_DIR)/Kconfig" "$(TEST_BIN)"
	rm -rf "$(COMMON_DIR)/etctmp" "$(COMMON_DIR)/etctmp.c"
