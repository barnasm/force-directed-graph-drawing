CC=g++
CFLAGS=-c -Wall

CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0

CPPFLAGS="-I/opt/gtk/include"
LDFLAGS="-L/opt/gtk/lib"
PKG_CONFIG_PATH="/opt/gtk/lib/pkgconfig"
export CPPFLAGS LDFLAGS PKG_CONFIG_PATH

X_CFLAGS=`pkg-config --cflags gtk+-3.0`
X_LDFLAGS=`pkg-config --libs gtk+-3.0` -lm

SOURCES += main.cpp \
    forcedirectedgraphdrawing.cpp \
    getdistance.cpp \
    graphclass.cpp \
    edgeclass.cpp \
    vertexclass.cpp \
    color.cpp

HEADERS += \
    forcedirectedgraphdrawing.h \
    getdistance.h \
    graphclass.h \
    edgeclass.h \
    vertexclass.h \
    color.h


all: Graph
Graph Drawing: main.o color.o edgeclass.o forcedirectedgraphdrawing.o getdistance.o graphclass.o vertexclass.o
		$(CC) main.o forcedirectedgraphdrawing.o color.o edgeclass.o vertexclass.o getdistance.o graphclass.o -o Graph $(X_CFLAGS) $(X_LDFLAGS)
		

main.o: main.cpp forcedirectedgraphdrawing.o
	$(CC) $(CFLAGS) main.cpp $(X_CFLAGS) $(X_LDFLAGS)
	
color.o: color.cpp 
	$(CC) $(CFLAGS) color.cpp

edgeclass.o: edgeclass.cpp vertexclass.o color.o
	$(CC) $(CFLAGS) edgeclass.cpp
	
forcedirectedgraphdrawing.o: forcedirectedgraphdrawing.cpp graphclass.cpp getdistance.o 
	$(CC) $(CFLAGS) forcedirectedgraphdrawing.cpp
	
getdistance.o: getdistance.cpp vertexclass.o edgeclass.o
	$(CC) $(CFLAGS) getdistance.cpp
	
graphclass.o: graphclass.cpp vertexclass.o edgeclass.o getdistance.o
	$(CC) $(CFLAGS) graphclass.cpp
	
vertexclass.o: vertexclass.cpp color.o
	$(CC) $(CFLAGS) vertexclass.cpp
	
clean:
	rm -rf *o Graph
