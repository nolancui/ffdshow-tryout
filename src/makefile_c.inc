all:.all

#64BIT=yes
#DEBUG=yes

#if FFDSHOW_PREFIX is set, assume cross-compilation
ifeq ($(FFDSHOW_PREFIX),)
  YASM=yasm.exe
else
  YASM=yasm
endif

ifeq ($(64BIT),yes)
    FFDSHOW_PREFIX=x86_64-pc-mingw32-
endif

ifeq ($(64BIT),yes)
    TARGET_OS=x86_64-pc-mingw32
    CFLAGS+=-DWIN64 -D_WIN64
    OPTFLAGS+=-O2 -m64
else
    TARGET_OS=i686-pc-mingw32
    CFLAGS+=-DWIN32 -D_WIN32
    OPTFLAGS+=-O3 -march=i686 -mmmx
endif

CFLAGS+=-mno-cygwin -mdll -mthreads -pipe
OPTFLAGS+=-fno-common -fno-tree-vectorize

ifeq ($(SSE),yes)
    OPTFLAGS+=-msse -mfpmath=sse
endif
ifeq ($(SSE2),yes)
    OPTFLAGS+=-msse2
endif

ifeq ($(DEBUG),yes)
    CFLAGS+=-DDEBUG -D_DEBUG -g 
    YASMFLAGS+=-g cv8
else
    CFLAGS+=-DNDEBUG -UDEBUG
    OPTFLAGS+=-fomit-frame-pointer
    LDFLAGS+=--strip-all
endif

ifeq ($(UNICODE),yes)
    CFLAGS+=-DUNICODE -D_UNICODE
endif

ifeq ($(FORCECPP),1)
    CFLAGS+=-x c++
endif

CPPFLAGS+=-fno-rtti

ifeq ($(EXCEPTIONS),1)
    CPPFLAGS+=-fexceptions
else
    CPPFLAGS+=-fno-exceptions
endif

OUTPUTFLAG=-MMD -o\

GCCDEF=$(DEF)

OBJS+=$(SRCS:.cpp=.o) $(SRCS_C:.c=.o) $(SRCS_YASM:.asm=.o) $(RESSRC:.rc=.res.o)

.c.o:
	$(FFDSHOW_PREFIX)$(CC) -c $(CFLAGS) $(OPTFLAGS) $(OUTPUTFLAG)$@ $<

.cpp.o:
	$(FFDSHOW_PREFIX)$(CC) -c $(CFLAGS) $(CPPFLAGS) $(OPTFLAGS) $(OUTPUTFLAG)$@ $<

%.o: %.asm
ifeq ($(64BIT),yes)
	$(YASM) -f win64 -m amd64 -DWIN64 -DARCH_X86_64 -DPREFIX $(YASMFLAGS) -I$(<D)/ -o $@  $<
else
	$(YASM) -f win32 -m x86 -DWIN32 -DARCH_X86_32 -DPREFIX $(YASMFLAGS) -I$(<D)/ -o $@  $<
endif

%.res.o: %.rc
	$(FFDSHOW_PREFIX)windres $< -o $@ -D_WIN32_IE=0x0500 -DIDCLOSE=8

$(SLIB): $(OBJS) $(DEF)
	$(FFDSHOW_PREFIX)dllwrap -mno-cygwin --target=$(TARGET_OS) \
             -Wl,--enable-stdcall-fixup $(LDFLAGS) \
             --def $(GCCDEF) $(GCCDLLENTRY) -o $@ $(OBJS) $(GCCLIBS)

$(SEXE): $(OBJS)
	$(FFDSHOW_PREFIX)$(CC) -mno-cygwin --target=$(TARGET_OS) \
             -Wl,--enable-stdcall-fixup -o $@ $(OBJS) $(GCCLIBS)
	$(FFDSHOW_PREFIX)strip $@

distclean: clean

TEMPFILES=*.i *.ii *.d *.s *.o

-include $(SRCS:.cpp=.d) $(SRCS_C:.c=.d)