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
