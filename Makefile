#  Copyright (c) 2009 Vinay Pulim

#  This file is part of TankTV.

#  TankTV is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.

#  TankTV is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with TankTV.  If not, see <http://www.gnu.org/licenses/>.

ifeq ($(CXX), cc)
CXX=g++
endif
CFLAGS:=$(CFLAGS) `pkg-config directfb freetype2 taglib sqlite3 --cflags`
LDFLAGS:=$(LDFLAGS) `pkg-config directfb freetype2 taglib sqlite3 --libs` -ldl -lmp4ff -lfaad -lmpg123 -lz -lcurl

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
	MenuItem.cpp 

OBJECTS = $(SOURCES:.cpp=.o)
APP = tanktv

all: $(SOURCES) $(APP)

$(APP): $(OBJECTS)
	$(CXX) -Wl,-E $(OBJECTS) -o $@ $(LDFLAGS) 
	scp -i /Users/vp/nmt/id_rsa_root.openssh tanktv root@192.168.1.5:/share/Apps/TankTV

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(APP) *~

depend:
	makedepend -Y *.cpp 2>/dev/null

# DO NOT DELETE

Application.o: Application.h Event.h config.h Screen.h Widget.h Box.h
Application.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
Application.o: Database.h
Audio.o: Audio.h config.h Decoder.h Types.h File.h Utils.h
Box.o: Box.h Utils.h
Curl.o: Curl.h
Database.o: Database.h config.h Types.h Utils.h File.h
DownloadsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h
DownloadsMenu.o: Application.h Renderer.h ImageLoader.h Thread.h Audio.h
DownloadsMenu.o: Decoder.h Types.h Database.h MenuItem.h File.h
FFMpegDecoder.o: File.h config.h Decoder.h
File.o: File.h config.h Utils.h
FileMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
FileMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
FileMenu.o: Database.h MenuItem.h File.h Player.h
Font.o: Font.h config.h Renderer.h Box.h Event.h ImageLoader.h Thread.h
Font.o: Utils.h
ImageLoader.o: ImageLoader.h Thread.h
MP3Decoder.o: Decoder.h config.h
MP4Decoder.o: Decoder.h config.h
Main.o: Application.h Event.h config.h Screen.h Widget.h Box.h Renderer.h
Main.o: ImageLoader.h Thread.h Audio.h Decoder.h Types.h Database.h Menu.h
Main.o: MenuItem.h File.h
MainMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MainMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
MainMenu.o: Database.h MenuItem.h File.h
MediaMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MediaMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
MediaMenu.o: Database.h MenuItem.h File.h
Menu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
Menu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
Menu.o: Database.h MenuItem.h File.h Utils.h
MenuItem.o: MenuItem.h Menu.h Screen.h Widget.h Event.h config.h Box.h
MenuItem.o: Application.h Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h
MenuItem.o: Types.h Database.h File.h Player.h Utils.h
MoviesMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MoviesMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
MoviesMenu.o: Database.h MenuItem.h File.h
MusicItem.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MusicItem.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
MusicItem.o: Database.h MenuItem.h File.h Player.h Utils.h
MusicMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MusicMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
MusicMenu.o: Database.h MenuItem.h File.h Player.h Utils.h
Player.o: Player.h Screen.h Widget.h Event.h config.h Box.h Audio.h Decoder.h
Player.o: Types.h Application.h Renderer.h ImageLoader.h Thread.h Database.h
Renderer.o: Renderer.h config.h Box.h Event.h ImageLoader.h Thread.h Font.h
Renderer.o: Utils.h
Screen.o: Screen.h Widget.h Event.h config.h Box.h Application.h Renderer.h
Screen.o: ImageLoader.h Thread.h Audio.h Decoder.h Types.h Database.h
SettingsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
SettingsMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
SettingsMenu.o: Database.h MenuItem.h File.h
TVShowsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
TVShowsMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h Decoder.h Types.h
TVShowsMenu.o: Database.h MenuItem.h File.h
Thread.o: Thread.h
Utils.o: Utils.h
Widget.o: Widget.h Event.h config.h Box.h Application.h Screen.h Renderer.h
Widget.o: ImageLoader.h Thread.h Audio.h Decoder.h Types.h Database.h Utils.h
Xml.o: Xml.h
