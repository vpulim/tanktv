ifeq ($(CXX), cc)
CXX=g++
endif
CFLAGS=`pkg-config directfb --cflags`
LDFLAGS=`pkg-config directfb --libs` -ldl -lmpg123

SOURCES = Main.cpp \
	Utils.cpp \
	Thread.cpp \
	ImageLoader.cpp \
	File.cpp \
	Application.cpp \
	Audio.cpp \
	Renderer.cpp \
	Player.cpp \
	Box.cpp \
	Widget.cpp \
	Screen.cpp \
	Menu.cpp \
	MenuItem.cpp \
	MainMenu.cpp \
	MusicMenu.cpp \
	DownloadsMenu.cpp \
	MoviesMenu.cpp 

OBJECTS = $(SOURCES:.cpp=.o)
APP = nmti

all: $(SOURCES) $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) -Wl,-E $(OBJECTS) -o $@
	scp -i /home/vp/id_rsa_root.openssh nmti root@192.168.1.4:/share/Apps/TankTV

.cpp.o:
	cat config.in > config.h
	echo "#define NMT 1" >> config.h
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(APP) *~
