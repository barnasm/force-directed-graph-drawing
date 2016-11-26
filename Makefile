CC=g++
CFLAGS= -c -std=c++11 -Wall -Werror

TARGET = graph

#CONFIG += link_pkgconfig
#PKGCONFIG += gtk+-3.0

#CPPFLAGS="-I/opt/gtk/include"
#LDFLAGS="-L/opt/gtk/lib"
#PKG_CONFIG_PATH="/opt/gtk/lib/pkgconfig"
#export CPPFLAGS LDFLAGS PKG_CONFIG_PATH

X_CFLAGS= -std=c++11 -Werror `pkg-config --cflags gtk+-3.0`
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


OBJS = $(SOURCES:.cpp=.o)

INCLUDES = -I/home/Uname/include  -I../include
#LFLAGS = -L/home/Uname/lib  -L../lib
# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm

#.PHONY: depend clean

all: $(TARGET)
$(TARGET):	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(X_CFLAGS) $(X_LDFLAGS)
		

.cpp.o:
	$(CC) $(CFLAGS) $<  -o $@ $(X_CFLAGS) $(X_LDFLAGS)

#.PHONY: clean

#main.o: main.cpp forcedirectedgraphdrawing.o
#	$(CC) $(CFLAGS) main.cpp $(X_CFLAGS) $(X_LDFLAGS)
#	
#color.o: color.cpp 
#	$(CC) $(CFLAGS) color.cpp

#edgeclass.o: edgeclass.cpp vertexclass.o color.o
#	$(CC) $(CFLAGS) edgeclass.cpp
#	
#forcedirectedgraphdrawing.o:	forcedirectedgraphdrawing.cpp\
#								graphclass.cpp getdistance.o 
#	$(CC) $(CFLAGS) forcedirectedgraphdrawing.cpp
#	
#getdistance.o: getdistance.cpp vertexclass.o edgeclass.o
#	$(CC) $(CFLAGS) getdistance.cpp
#	
#graphclass.o: graphclass.cpp vertexclass.o edgeclass.o getdistance.o
#	$(CC) $(CFLAGS) graphclass.cpp
#	
#vertexclass.o: vertexclass.cpp color.o
#	$(CC) $(CFLAGS) vertexclass.cpp

clean:
	rm -rf *o
	
depend: $(SRCS)
		makedepend $(INCLUDES) $^
