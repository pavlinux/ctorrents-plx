#
# Pavlinux's Makefile skeleton (c) 2011
#
# If not linked try:   ln -sf `g++ -print-file-name=libstdc++.a`
#

CXX ?= g++
CC  ?= gcc

CXXFLAGS =-W -Wall -Wextra -mtune=core2 -O2 -DFORTIFY_SOURCE=2 -g0 -fomit-frame-pointer -std=gnu++0x -static-libstdc++
CFLAGS =-W -Wall -Wextra -mtune=core2 -O2 -DFORTIFY_SOURCE=2 -g0 -fomit-frame-pointer -std=gnu99 -static

# CXXFLAGS=-W -Wall -Wextra -mtune=generic -O0 -g3 -gdwarf-2 -fno-omit-frame-pointer -std=gnu++0x -static-libstdc++
# CFLAGS=-W -Wall -Wextra -mtune=generic -O0 -g3 -gdwarf-2 -fno-omit-frame-pointer -std=gnu99 -static

#CXXFLAGS=-DDEBUG -W -Wall -Wextra -Wshadow -std=gnu99 -O0 -g3 -ggdb3 -gdwarf-2 -fno-omit-frame-pointer

LINK ?= g++
LDFLAGS=-pthread
LIBS=-lrt -static-libstdc++ -L.

EXEC = ctorrent

SOURSES = bencode.cpp bitfield.cpp btconfig.cpp btcontent.cpp btfiles.cpp \
	  btrequest.cpp btstream.cpp bufio.cpp connect_nonb.cpp \
	  console.cpp ctcs.cpp ctorrent.cpp downloader.cpp httpencode.cpp \
	  iplist.cpp peer.cpp peerlist.cpp rate.cpp setnonblock.cpp \
	  sigint.cpp tracker.cpp sha1.c

OBJECTS = bencode.o bitfield.o btconfig.o btcontent.o btfiles.o \
	  btrequest.o btstream.o bufio.o connect_nonb.o \
	  console.o ctcs.o ctorrent.o downloader.o httpencode.o \
	  iplist.o peer.o peerlist.o rate.o setnonblock.o sigint.o \
	  tracker.o sha1.o

ifneq (,$(findstring DDEBUG,$(CXXFLAGS)))
    CXXFLAGS += -lgcov -fprofile-arcs -ftest-coverage
    LINK   += -lgcov -fprofile-arcs -ftest-coverage
endif

VERSION = 0.0.7

CPUS = $(shell grep processor /proc/cpuinfo | wc -l)
MAKEFLAGS += j${CPUS}

.SUFFIXES: .o .cpp .c

.o:
	$(CXX) -c $(CXXFLAGS) -lrt -o $@ $<

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LINK) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $(EXEC)

small:
	strip --discard-locals $(EXEC)
	strip --discard-all $(EXEC)
	strip --strip-debug $(EXEC)
	strip --strip-all $(EXEC)
	sstrip $(EXEC)
install:
	install -s -D -m 755 -o 65535 -g 65534 -D ${EXEC} /usr/bin/;

uninstall:
	rm -f /usr/bin/${EXEC};

clean:
	rm -f *.[oiB] *.*~  *.spin *.gcno *.gcda *.gcov;
	rm -f ${EXEC};

gz:
	tar -c *.c *.cpp *.h Makefile | gzip -c9 > ${EXEC}-${VERSION}.tar.gz
bz2:
	tar -c *.c *.cpp *.h Makefile | bzip2 -c9 > ${EXEC}-${VERSION}.tar.bz2
lzma:
	tar -c *.c *.cpp *.h Makefile | lzma -z -c9 > ${EXEC}-${VERSION}.tar.lzma
xz:	
	tar -c *.c *.cpp *.h Makefile | xz -z -c9 > ${EXEC}-${VERSION}.tar.xz
