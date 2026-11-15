############################################################################
# xiao-rp2350-bargraph/apps/bargraph/Makefile
#
# SPDX-License-Identifier: Apache-2.0
############################################################################

include $(APPDIR)/Make.defs

PROGNAME  = $(CONFIG_BARGRAPH_APP_PROGNAME)
PRIORITY  = $(CONFIG_BARGRAPH_APP_PRIORITY)
STACKSIZE = $(CONFIG_BARGRAPH_APP_STACKSIZE)
MODULE    = $(CONFIG_BARGRAPH_APP)

BARGRAPH_ROOT = $(APPDIR)$(DELIM)..$(DELIM)xiao-rp2350-bargraph

MAINSRC = bargraph_main.c
CSRCS   = bargraph_logic.c

VPATH   += :$(BARGRAPH_ROOT)$(DELIM)src
DEPPATH += --dep-path $(BARGRAPH_ROOT)$(DELIM)src
CFLAGS  += ${INCDIR_PREFIX}"$(BARGRAPH_ROOT)$(DELIM)include"

include $(APPDIR)/Application.mk
