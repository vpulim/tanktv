#  Copyright (c) 2009 Vinay Pulim
#
#  This file is part of TankTV.
#
#  TankTV is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  TankTV is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.

#-------------------------------------------------------------------------
# general definitions
#-------------------------------------------------------------------------

MAKE		= Makefile
CTAGS 		= ctags
CTAGS_FLAGS = -R --c++-kinds=p --fields=+ias --extra=+fq --sort=foldcase

#-------------------------------------------------------------------------
# command definitions
#-------------------------------------------------------------------------

CC			= $(TARGET)-gcc
CXX			= $(TARGET)-g++
LD			= $(TARGET)-gcc
AR			= $(TARGET)-ar
NM			= $(TARGET)-nm
OBJCOPY		= $(TARGET)-objcopy
OBJDUMP		= $(TARGET)-objdump

#-------------------------------------------------------------------------
# 3rd Party libraries:
# - faad
# - mpg123
# - curl
# - sqlite3
# - taglib
# - zlib
#-------------------------------------------------------------------------

CFLAGS += -I3rdparty/include
CFLAGS += -I3rdparty/include/curl
CFLAGS += -I3rdparty/include/taglib
LDFLAGS += -L3rdparty/lib -lmp4ff -lfaad -lmpg123 -lcurl -lsqlite3 -ltag -lz

# Freetype 2
CFLAGS += -I3rdparty/include/freetype2
LDFLAGS += -lfreetype

# DirectFB
CFLAGS += -D_REENTRANT -I3rdparty/include/directfb
LDFLAGS += -ldirect -ldirectfb -lfusion -lpthread -ldl

# Boost: program options
LDFLAGS += -lboost_program_options

#-------------------------------------------------------------------------
# Application definitions
#-------------------------------------------------------------------------

CFLAGS	+= -I$(SDIR) -Wall -g -O2

#-------------------------------------------------------------------------
# directory definitions
#-------------------------------------------------------------------------

SRC 	= src
OBJ 	= obj
DIST 	= dist
LIB 	= lib
DIST_LIB = $(DIST)/$(LIB)

#-------------------------------------------------------------------------
# source file definitions
#-------------------------------------------------------------------------

SOURCES = Main.cpp \
	Utils.cpp \
	Thread.cpp \
	ImageLoader.cpp \
	File.cpp \
	Application.cpp \
	Database.cpp \
	Audio.cpp \
	MP3Decoder.cpp \
	MP4Decoder.cpp \
	Renderer.cpp \
	Curl.cpp \
	Font.cpp \
	Player.cpp \
	Box.cpp \
	Widget.cpp \
	Screen.cpp \
	Menu.cpp \
	MainMenu.cpp \
	FileMenu.cpp \
	MusicMenu.cpp \
	SettingsMenu.cpp \
	MenuItem.cpp \
	Indexer.cpp \
	NMTSettings.cpp

#-------------------------------------------------------------------------
# macro definitions
#-------------------------------------------------------------------------

SRC_FILES	= $(SOURCES:%.cpp=$(SRC)/%.cpp)
SRC_OBJS	= $(SOURCES:%.cpp=$(OBJ)/%.o)

#-------------------------------------------------------------------------
# implicit rule definitions
#-------------------------------------------------------------------------

$(OBJ)/%.o: $(SRC)/%.cpp
	@echo Compiling $<
	@$(CXX) $(CFLAGS) -c -o $@ $<

APP = tanktv

all: $(OBJ) $(SRC_OBJS) $(APP)

$(APP): $(SRC_OBJS) $(MAKE)
	@echo Linking $@
	@$(CXX) -Wl,-E $(SRC_OBJS) -o $@ $(LDFLAGS) 

$(SRC_OBJS): $(MAKE)

$(OBJ):
	mkdir $@

tags: $(SRC_FILES)
	@echo Generating tags...
	@$(CTAGS) $(CTAGS_FLAGS) $(SRC)
	@echo OK

clean:
	@echo Cleaning up...
	rm -rf $(OBJ)
	rm -rf $(DIST_LIB)
	rm -f $(APP) *~
	@echo OK

dist: $(APP) $(DIST_LIB)
	@echo Copying $(APP) to $@...
	@cp $(APP) $(DIST)
	@echo OK

$(DIST_LIB):
	@mkdir $@
	@echo Copying libs to $@...
	@cp -R $(LIB) $(DIST)

DEPENDENCIES = $(MAKE).dep

depend: $(DEPENDENCIES)
$(DEPENDENCIES): $(MAKE) $(SRC_FILES)
	@echo Generating dependencies
	@rm -f $(DEPENDENCIES)
	@$(CXX) $(CFLAGS) -M $(SRC_FILES) | sed '/^[^      ]*:/s/^/'$(OBJ)'\//' >> $(DEPENDENCIES)

include $(DEPENDENCIES)

