#
# Pavlinux's Makefile skeleton (c) 2011
#
# If not linked try:   ln -sf `g++ -print-file-name=libstdc++.a`
#
VERSION = 0.0.3

CXX = g++
CC  = gcc

CXXFLAGS=-march=native -g0 -Os -s
CFLAGS	=-march=native -g0 -Os -s

CACHE=$(shell cat /proc/cpuinfo | grep "cache size" | head -1 | cut -d" " -f3)

# GCC 4.5+
# for l2-cache-size view you're /proc/cpuinfo
ZZFLAGS =-frecord-gcc-switches -flto \
        -g0 -Ofast -march=native \
        -funroll-all-loops -ftree-vectorize \
        -fno-inline-functions-called-once \
        -fmerge-all-constants -ffreestanding \
        --param l2-cache-size=$(CACHE) \
        -floop-interchange -floop-block -floop-strip-mine \
        -ftree-loop-distribution -fexcess-precision=fast \
	-fno-strict-aliasing -fwhole-program \
	-fipa-sra -fsplit-stack -s -pipe

CXXFLAGS = $(ZZFLAGS)
CFLAGS = $(ZZFLAGS)

#CXXFLAGS=-DDEBUG -W -Wall -Wextra -Wshadow -std=gnu99 -O0 -g3 -ggdb3 -gdwarf-2 -fno-omit-frame-pointer

LINK = g++
LIBS=-lrt
LDFLAGS=-static-libstdc++ $(LIBS) -Wl,-Ofast -Wl,--as-needed

# -static-libstdc++

EXEC = ctorrent

SOURSES = bencode.cpp bitfield.cpp btconfig.cpp btcontent.cpp btfiles.cpp \
	  btrequest.cpp btstream.cpp bufio.cpp compat.c connect_nonb.cpp \
	  console.cpp ctcs.cpp ctorrent.cpp downloader.cpp httpencode.cpp \
	  iplist.cpp peer.cpp peerlist.cpp rate.cpp setnonblock.cpp sigint.cpp \
	  tracker.cpp sha1.c

OBJECTS = bencode.o bitfield.o btconfig.o btcontent.o btfiles.o \
	  btrequest.o btstream.o bufio.o compat.o connect_nonb.o \
	  console.o ctcs.o ctorrent.o downloader.o httpencode.o \
	  iplist.o peer.o peerlist.o rate.o setnonblock.o sigint.o \
	  tracker.o sha1.o
	  
CPUS = $(shell grep processor /proc/cpuinfo | wc -l)
MAKEFLAGS += j${CPUS}

.SUFFIXES: .o .cpp .c

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LINK) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $(EXEC)

small:
	strip --discard-locals $(EXEC)
	strip --discard-all $(EXEC)
	strip --strip-debug $(EXEC)
	strip --strip-all $(EXEC)
	upx -9 $(EXEC)

install:
	install -s -m 550 -o 0 -g 0 -D ${EXEC} /usr/bin/;

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


