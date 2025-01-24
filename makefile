# ----------------------------
# Makefile Options
# ----------------------------

NAME = SPIRO
ICON = icon.png
DESCRIPTION = "Spirograph"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -O3
CXXFLAGS = -Wall -Wextra -Oz

PREFER_OS_LIBC=NO

# ----------------------------

include $(shell cedev-config --makefile)
