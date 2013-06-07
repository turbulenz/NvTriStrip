CC=gcc
CPP=g++
AR=ar
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

INCLUDES=-Iinclude
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
LIBDIR=lib/debug
else
OBJDIR=obj/release
LIBDIR=lib/release
endif

dummy := $(shell test -d $(LIBDIR) || mkdir -p $(LIBDIR))
dummy := $(shell test -d $(OBJDIR) || mkdir -p $(OBJDIR))

LIB=$(LIBDIR)/libNvTriStrip.a

SRCDIR=src
SOURCES=$(SRCDIR)/NvTriStripObjects.cpp $(SRCDIR)/NvTriStrip.cpp
OBJECTS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

all: $(SOURCES) $(LIB)

clean:
	rm -f $(OBJECTS)
	rm -f $(LIB)
	-rmdir -p $(OBJDIR)
	-rmdir -p $(LIBDIR)

$(LIB): $(OBJECTS)
	$(AR) cr $@ $(OBJECTS)

$(OBJDIR)/%.o : %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
