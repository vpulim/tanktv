ifeq ($(CXX), cc)
CXX=g++
endif
CFLAGS=`pkg-config directfb pixman-1 fontconfig cairo --cflags`
LDFLAGS=`pkg-config directfb pixman-1 fontconfig cairo --libs` -lpng12 -lfreetype -lz -lxml2

SOURCES = Main.cpp Application.cpp Renderer.cpp Box.cpp Widget.cpp MenuItem.cpp
OBJECTS = $(SOURCES:.cpp=.o)
APP = nmti

all: $(SOURCES) $(APP)

$(APP): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
	scp -i /home/vp/id_rsa_root.openssh nmti root@192.168.1.4:/share/Apps/nmti

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(APP) *~