# ----------------------------
# Makefile Options
# ----------------------------

NAME = SSBCE
ICON = icon.png
DESCRIPTION = "Super Smash Bros. CE"
COMPRESSED = NO
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
