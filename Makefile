#############################################################################
#
#    Filename: Makefile
# Description: general makefle for compiling C and C++ source files.
#     License: GPL (General Public License)
#    Modified: hongtai zhang (carl_cheer@foxmail.com)
#        Date: 2013/08/06
#
############################################################################

EXTRA_CFLAGS 	= 
#lib.a
EXTRA_LDFLAGS   = 
INCLUDE   		= -I$(PWD) -I$(PWD)/include 
CPPFLAGS  		= -Wall $(INCLUDE) -D_LINUX -D_DTPDEBUG
LDFLAGS   		= -ldl -lcrypt -lpthread -lrt
SRCDIRS   		:= 
EXECUTE 		= main
PROGRAM 		= bin/$(EXECUTE)

SRCEXTS 		= .cpp
HDREXTS 		= .h
CFLAGS  		= -g -O2
CXXFLAGS 		= -g -O2
CC     			= gcc
CXX 			= g++
RM 				= -rm -f

ETAGS 			= etags
ETAGSFLAGS 		=

CTAGS 			= ctags
CTAGSFLAGS 		=

ifeq ($(SRCDIRS),)
  SRCDIRS := $(shell find $(SRCDIRS) -type d)
endif
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
HEADERS = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(HDREXTS))))
SRC_CXX = $(filter-out %.c,$(SOURCES))
OBJS    = $(addsuffix .o, $(basename $(SOURCES)))
DEPS   = $(OBJS:%.o=%.d) #replace %.d with .%.d (hide dependency files)

DEP_OPT = $(shell if `$(CC) --version | grep -i "GCC" >/dev/null`; then \
                  echo "-MM"; else echo "-M"; fi )
DEPEND.d    = $(CC)  $(DEP_OPT)  $(EXTRA_CFLAGS) $(CFLAGS) $(CPPFLAGS)
COMPILE.c   = $(CC)  $(EXTRA_CFLAGS) $(CFLAGS)   $(CPPFLAGS) -c
COMPILE.cxx = $(CXX) $(EXTRA_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
LINK.c      = $(CC)  $(EXTRA_CFLAGS) $(CFLAGS)   $(CPPFLAGS) $(LDFLAGS)
LINK.cxx    = $(CXX) $(EXTRA_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all objs tags ctags clean distclean install uninstall

.SUFFIXES:

all: $(PROGRAM)

objs:$(OBJS)
%.o:%.c
	$(COMPILE.c) $< -o $@

%.o:%.C
	$(COMPILE.cxx) $< -o $@

%.o:%.cc
	$(COMPILE.cxx) $< -o $@

%.o:%.cpp
	$(COMPILE.cxx) $< -o $@

%.o:%.CPP
	$(COMPILE.cxx) $< -o $@

%.o:%.c++
	$(COMPILE.cxx) $< -o $@

%.o:%.cp
	$(COMPILE.cxx) $< -o $@

%.o:%.cxx
	$(COMPILE.cxx) $< -o $@

tags: $(HEADERS) $(SOURCES)
	$(ETAGS) $(ETAGSFLAGS) $(HEADERS) $(SOURCES)

ctags: $(HEADERS) $(SOURCES)
	$(CTAGS) $(CTAGSFLAGS) $(HEADERS) $(SOURCES)

$(PROGRAM):$(OBJS)
ifeq ($(SRC_CXX),)
    $(LINK.c) $(OBJS) $(EXTRA_LDFLAGS) -o $@
	@echo
    @echo Key in ./$@ to execute the program.
else
	$(LINK.cxx) $(OBJS) $(EXTRA_LDFLAGS) -o $@
	@echo
	@echo Key in ./$@ to execute the program.
endif

strip:
	strip $(PROGRAM)

install:
	cp $(PROGRAM) $(HOME)/bin/$(EXECUTE)

uninstall:
	rm -f $(HOME)/bin/$(EXECUTE)

clean:
	$(RM) $(OBJS) $(PROGRAM)
	$(RM) `find . -name ".*.d"`

distclean: clean
	$(RM) $(DEPS) TAGS
