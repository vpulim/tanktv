ifeq ($(CXX), cc)
CXX=g++
endif
CFLAGS=`pkg-config directfb freetype2 --cflags`
LDFLAGS=`pkg-config directfb freetype2 --libs` -ldl -lmpg123

SOURCES = Main.cpp \
	Utils.cpp \
	Thread.cpp \
	ImageLoader.cpp \
	File.cpp \
	Application.cpp \
	Audio.cpp \
	Renderer.cpp \
	Font.cpp \
	Player.cpp \
	Box.cpp \
	Widget.cpp \
	Screen.cpp \
	Menu.cpp \
	MenuItem.cpp \
	MainMenu.cpp \
	MediaMenu.cpp \
	MusicMenu.cpp \
	DownloadsMenu.cpp \
	SettingsMenu.cpp \
	MoviesMenu.cpp \
	TVShowsMenu.cpp 

OBJECTS = $(SOURCES:.cpp=.o)
APP = nmti

all: $(SOURCES) $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) -Wl,-E $(OBJECTS) -o $@
	scp -i /Users/vp/nmt/id_rsa_root.openssh nmti root@192.168.1.4:/share/Apps/TankTV

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(APP) *~

depend:
	makedepend -Y *.cpp 2>/dev/null

# DO NOT DELETE

Application.o: Application.h Event.h config.h Screen.h Widget.h Box.h
Application.o: Renderer.h ImageLoader.h Thread.h Audio.h
Audio.o: Audio.h config.h
Box.o: Box.h Utils.h
DownloadsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h
DownloadsMenu.o: Application.h Renderer.h ImageLoader.h Thread.h Audio.h
DownloadsMenu.o: MenuItem.h File.h
File.o: File.h config.h Utils.h
Font.o: Font.h config.h Renderer.h Event.h ImageLoader.h Thread.h
ImageLoader.o: ImageLoader.h Thread.h
Main.o: Application.h Event.h config.h Screen.h Widget.h Box.h Renderer.h
Main.o: ImageLoader.h Thread.h Audio.h Menu.h MenuItem.h
MainMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MainMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h
Menu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
Menu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h Utils.h
MenuItem.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MenuItem.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h
MoviesMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MoviesMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h File.h
MusicMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
MusicMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h Player.h
MusicMenu.o: File.h
Player.o: Player.h Screen.h Widget.h Event.h config.h Box.h Audio.h
Player.o: Application.h Renderer.h ImageLoader.h Thread.h
Renderer.o: Renderer.h config.h Event.h ImageLoader.h Thread.h Font.h Utils.h
Screen.o: Screen.h Widget.h Event.h config.h Box.h Application.h Renderer.h
Screen.o: ImageLoader.h Thread.h Audio.h
SettingsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
SettingsMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h
TVShowsMenu.o: Menu.h Screen.h Widget.h Event.h config.h Box.h Application.h
TVShowsMenu.o: Renderer.h ImageLoader.h Thread.h Audio.h MenuItem.h File.h
Thread.o: Thread.h
Utils.o: Utils.h
Widget.o: Widget.h Event.h config.h Box.h Application.h Screen.h Renderer.h
Widget.o: ImageLoader.h Thread.h Audio.h
