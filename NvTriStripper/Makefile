CC=gcc
CPP=g++
PLATFORM := $(shell uname -s)
M_ARCH := $(shell uname -m)
ifeq ($(PLATFORM),Linux)
ifeq ($(M_ARCH),x86_64)
LIBTYPE=linux64
else
ifeq ($(M_ARCH),i686)
LIBTYPE=linux32
else
echo Unsupported architecture $(M_ARCH)
endif
endif
LDFLAGS=$(LIBPATHS)
else
ifeq ($(PLATFORM),Darwin)
LIBTYPE=macosx
LDFLAGS=$(LIBPATHS) -arch i386
else
echo Unsupported platform $(PLATFORM)
endif
endif

NVTRISTRIPDIR=../NvTriStrip

INCLUDES=-I$(NVTRISTRIPDIR)/include
DEFINES=
CFLAGS=-c $(DEFINES) $(INCLUDES)
ifeq ($(PLATFORM),Darwin)
CFLAGS += -arch i386 -march=pentium4 -msse2 -mfpmath=sse
else
ifeq ($(M_ARCH),i686)
CFLAGS += -march=pentium4 -msse2 -mfpmath=sse
endif
endif

ifeq ($(DEBUG), 1)
  CFLAGS += -g -DDEBUG -O0
  LDFLAGS += -g
else
  CFLAGS += -O2 -fdata-sections -ffunction-sections
ifeq ($(PLATFORM),Darwin)
  LDFLAGS += -dead-strip
else
  LDFLAGS += -Wl,--gc-sections
endif
endif

ifeq ($(DEBUG), 1)
OBJDIR=obj/debug
BINDIR=bin/debug
NVTRISTRIPLIBDIR=$(NVTRISTRIPDIR)/lib/debug
else
OBJDIR=obj/release
BINDIR=bin/release
NVTRISTRIPLIBDIR=$(NVTRISTRIPDIR)/lib/release
endif

dummy := $(shell test -d $(OBJDIR) || mkdir -p $(OBJDIR))
dummy := $(shell test -d $(BINDIR) || mkdir -p $(BINDIR))

LDFLAGS += -Wl,-L$(NVTRISTRIPLIBDIR) -Wl,-lNvTriStrip

SOURCES=main.cpp
OBJECTS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCES))
TOOL=$(BINDIR)/NvTriStripper

all: $(SOURCES) $(TOOL) $(NVTRISTRIPLIB)

clean:
	rm -f $(OBJECTS)
	rm -f $(TOOL)
	-rmdir -p $(OBJDIR)
	-rmdir -p $(BINDIR)

$(TOOL): $(OBJECTS)
	$(CPP) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : $(EXTSRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : $(HTTPSRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
