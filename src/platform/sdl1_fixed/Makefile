SRCS=	main.cpp \
	sound.cpp \
	../../fixed/common.cpp \
	../../fixed/render/gl1.cpp
CC=g++

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

CFLAGS+=-std=c++11 -O3 -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Wl,--gc-sections -D__SDL1__ $(SDL_CFLAGS)
#CFLAGS+=-std=c++11 -g -D__SDL1__ $(SDL_CFLAGS)
LDFLAGS+=$(SDL_LDFLAGS) -lm -lrt -lpthread -lGL -lGLU

INCLUDES+=-I../../fixed

openlara:
	$(CC) $(INCLUDES) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

install : openlara
	install -d $(DESTDIR)$(PREFIX)/bin/
	install openlara $(DESTDIR)$(PREFIX)/bin/OpenLara

uninstall :
	rm -f $(DESTDIR)$(PREFIX)/bin/OpenLara

clean:
	rm -f openlara
