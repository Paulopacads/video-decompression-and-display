
CC = g++

CPP_FILES = decode.cc display.cc #mpeg_reader.cc #bob.cc tools/mpeg2dec/libmpeg2/decode.c
HXX_FILES = tools/mpeg2dec/include/mpeg2.h decode.hh display.hh #mpeg_reader.hh #bob.hh
OBJ_FILES = $(CPP_FILES:.cpp=.o)

CXX_FLAGS += -Wall -Wextra -O3 -g
LDXX_FLAGS = 
CFLAGS = $(shell pkg-config --cflags opencv)
LIBS = $(shell pkg-config --libs opencv)
MAIN_FILE = main.cc
DIST = main

SKEL_DIST_DIR = pogl_skel_tp
SKEL_FILES = $(CPP_FILES) $(HXX_FILES) $(MAIN_FILE) Makefile


#For gcc 4.9
#CXXFLAGS+=-fdiagnostics-color=auto
export GCC_COLORS=1

define color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then \
	    tput setaf $(1); \
        fi;\
    fi
endef

define default_color
    if test -n "${TERM}" ; then\
	if test `tput colors` -gt 0 ; then  tput sgr0 ; fi; \
    fi
endef


all: post-build

pre-build:
	@$(call color,4)
	@echo "******** Starting Compilation ************"
	@$(call default_color)

post-build:
	@make --no-print-directory main-build ; \
	sta=$$?;	  \
	$(call color,4); \
	echo "*********** End Compilation **************"; \
	$(call default_color); \
	exit $$sta;

main-build: pre-build build

build: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) $(CFLAGS) -o $(DIST) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS) $(LIBS)


%.o: %.cpp %.hh
	@$(call color,2)
	@echo "[$@] $(CXX_FLAGS)"
	@$(call default_color)
	@$(CC) -c -o $@ $< $(CXX_FLAGS) ; \
	sta=$$?;	  \
	if [ $$sta -eq 0 ]; then  \
	  $(call color,2) ; \
	  echo "[$@ succes]" ; \
	  $(call default_color) ; \
	else  \
	  $(call color,1) ; \
	  echo "[$@ failure]" ; \
	  $(call default_color) ; \
	fi ;\
	exit $$sta

.PHONY: all clean pre-build post-build main-build build skel

clean:
	rm -f $(OBJ_FILES)
	rm -f $(DIST)
	rm -rf $(SKEL_DIST_DIR).tar.bz2


skel:
	rm -rf $(SKEL_DIST_DIR)
	mkdir $(SKEL_DIST_DIR)
	cp $(SKEL_FILES) $(SKEL_DIST_DIR)
	tar -cjvf $(SKEL_DIST_DIR).tar.bz2 $(SKEL_DIST_DIR)
	rm -rf $(SKEL_DIST_DIR)
