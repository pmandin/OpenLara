SRCS=main.cpp \
     ../../libs/stb_vorbis/stb_vorbis.c \
     ../../libs/minimp3/minimp3.cpp \
     ../../libs/tinf/tinflate.c
CC=g++

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

CFLAGS+=-std=c++11 -O3 -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Wl,--gc-sections -D__SDL1__ $(SDL_CFLAGS)
LDFLAGS+=$(SDL_LDFLAGS) -lm -lrt -lpthread -lasound -ludev

INCLUDES+=-I../../

openlara :
	$(CC) $(INCLUDES) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

install : openlara
	install -d $(DESTDIR)$(PREFIX)/bin/
	install openlara $(DESTDIR)$(PREFIX)/bin/OpenLara

uninstall :
	rm -f $(DESTDIR)$(PREFIX)/bin/OpenLara

clean:
	rm -f OpenLara

