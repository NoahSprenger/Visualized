CC := g++
CFLAGS := $(CFLAGS) -fpic

LIB_DIRS := \
	/usr/local/lib

INCLUDE_DIRS := \
	/usr/local/include

BUILD_FLAGS := \
	$(BUILD_FLAGS) \
	-pthread

LINK_LIBRARIES := \
	$(LINK_LIBRARIES) \
	stdc++fs \
	X11 \
	box2d \
	sqlite3

PRODUCTION_LINUX_ICON := sfml

PRODUCTION_LINUX_APP_NAME := Visualized
PRODUCTION_LINUX_APP_COMMENT := A to do list that helps you visualize tasks
