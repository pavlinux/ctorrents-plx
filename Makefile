#
# Pavlinux's Makefile skeleton (c) 2011
#
# If not linked try:   ln -sf `g++ -print-file-name=libstdc++.a`
#

# With link time optimization binary too fat (avg. +50k)
WITH_LTO = 0

CXX ?= g++
CC  ?= gcc

ifeq ($(shell getconf LONG_BIT), 64)
    X86_FLAGS=-m64 -mtune=nocona -msse -msse2 -msse3
endif
ifeq ($(shell getconf LONG_BIT), 32)
    X86_FLAGS=-m32
endif

X86_CFLAGS := ${X86_FLAGS} -Os -g0 -pipe -W -Wextra -Wall

ZYNQ_CFLAGS := -march=armv7-a -mtune=cortex-a9 -mfloat-abi=hard -mfpu=vfpv3-d16 -mthumb -mtls-dialect=gnu -fgnu89-inline -fmerge-all-constants -frounding-math
ZYNC_CXXFLAGS := -march=armv7-a -mtune=cortex-a9 -mfloat-abi=hard -mfpu=vfpv3-d16 -mthumb -mtls-dialect=gnu -fmerge-all-constants -frounding-math

####################
#   Result Flags   #
####################
CXXFLAGS :=-std=gnu++11 ${CXXFLAGS}
CFLAGS :=-std=gnu99 ${CFLAGS}

LINK ?= ${CXX}
LDFLAGS := ${ARCH_FLAGS} -lrt -Wl,-O1,-hashvals,--hash-style=both -pipe
LIBS :=-L. -static-libstdc++ -static-libgcc
# -Wl,-Bstatic -lssl -Wl,-Bstatic -lcrypto

CPUS = $(shell grep processor /proc/cpuinfo | wc -l)
MAKEFLAGS += -j${CPUS}

ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
    CC := clang
    export WITH_LTO = 0
else
    CC ?= gcc
endif

# Link Time Optimization
ifeq ($(WITH_LTO), 1)

    LTO_CFLAGS := -flto -fno-toplevel-reorder -flto=${CPUS}
    LTO_FINAL_CFLAGS := ${LTO_CFLAGS} -fwhole-program
    LTO_FINAL_CFLAGS += -freg-struct-return
    LTO_FINAL_CFLAGS += -fuse-linker-plugin
    LTO_FINAL_CFLAGS += -fno-omit-frame-pointer
    LTO_FINAL_CFLAGS += -fno-delete-null-pointer-checks
    LTO_FINAL_CFLAGS += -fno-strict-aliasing
    LTO_FINAL_CFLAGS += -fno-strict-overflow

    CXXFLAGS  += ${LTO_CFLAGS}
    CFLAGS  += ${LTO_CFLAGS}
    LDFLAGS += ${LTO_FINAL_CFLAGS}
endif


EXEC = ctorrent

OBJECTS = bencode.o bitfield.o btconfig.o btcontent.o btfiles.o \
	  btrequest.o btstream.o bufio.o connect_nonb.o \
	  console.o ctcs.o ctorrent.o downloader.o httpencode.o \
	  iplist.o peer.o peerlist.o rate.o setnonblock.o sigint.o \
	  tracker.o sha1.o

ifneq (,$(findstring DDEBUG,$(CXXFLAGS)))
    CXXFLAGS += -lgcov -fprofile-arcs -ftest-coverage
    LINK   += -lgcov -fprofile-arcs -ftest-coverage
endif

VERSION = 0.1.0

.SUFFIXES: .o .cpp .c

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LINK) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(EXEC)

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
