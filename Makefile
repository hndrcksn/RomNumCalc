.PHONY : clean all directories

SHELL        = /bin/sh
CC           = gcc
MKDIR_P      = mkdir -p
#FLAGS        = -std=gnu99 -Iinclude
LIBCFLAGS    = -c -Wall -Werror -fPIC
LIBLDFLAGS   = -shared
CFLAGS       = -Wall -Werror

LIBSRCDIR  = ./src/lib
APPSRCDIR  = ./src/app
TESTSRCDIR = ./src/tests
LIBCHECKDIR = /usr/lib/arm-linux-gnueabihf/

OBJDIR = ./obj
BINDIR = ./bin

LIB        = RomNumMath
APPTARGET  = RomNumCalcApp
TESTTARGET = TestRomNumCalc
LIBSRC     = $(LIB).c
APPSRC     = $(APPTARGET).c
TESTSRC    = $(TESTTARGET).c
LIBOBJ     = $(LIB).o
APPOBJ     = $(APPTARGET).o
TESTOBJ    = $(TESTTARGET).o
LIBTARGET  = lib$(LIB).so
LIBCHECK   = check.a

clean : directories
	rm -f $(OBJDIR)/* $(BINDIR)/*

directories : $(OBJDIR) $(BINDIR)

$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)

$(BINDIR):
	$(MKDIR_P) $(BINDIR)

$(LIBOBJ) : directories $(LIBSRCDIR)/$(LIBSRC)
	$(CC) $(LIBCFLAGS) $(LIBSRCDIR)/$(LIBSRC) -o $(OBJDIR)/$(LIBOBJ)

$(OBJDIR)/$(LIBOBJ) : directories $(LIBSRCDIR)/$(LIBSRC)
	$(CC) $(LIBCFLAGS) $(LIBSRCDIR)/$(LIBSRC) -o $(OBJDIR)/$(LIBOBJ)

$(LIBTARGET) : directories $(OBJDIR)/$(LIBOBJ)
	$(CC) $(LIBLDFLAGS) -o $(BINDIR)/$(LIBTARGET) $(OBJDIR)/$(LIBOBJ)

$(BINDIR)/$(LIBTARGET) : directories $(OBJDIR)/$(LIBOBJ)
	$(CC) $(LIBLDFLAGS) -o $(BINDIR)/$(LIBTARGET) $(OBJDIR)/$(LIBOBJ)

$(APPTARGET) : directories $(APPSRCDIR)/$(APPSRC) $(BINDIR)/$(LIBTARGET)
	$(CC) -L$(BINDIR) $(CFLAGS) $(APPSRCDIR)/$(APPSRC) -o $(BINDIR)/$(APPTARGET) -l$(LIB)

$(BINDIR)/$(APPTARGET) : directories $(APPSRCDIR)/$(APPSRC) $(BINDIR)/$(LIBTARGET)
	$(CC) -L$(BINDIR) $(CFLAGS) $(APPSRCDIR)/$(APPSRC) -o $(BINDIR)/$(APPTARGET) -l$(LIB)

$(TESTTARGET) : directories $(TESTSRCDIR)/$(TESTSRC) $(BINDIR)/$(LIBTARGET)
	$(CC) -L$(BINDIR),$(LIBCHECKDIR) $(CFLAGS) $(TESTSRCDIR)/$(TESTSRC) -o $(BINDIR)/$(TESTTARGET) -l$(LIB) -l$(LIBCHECK)

$(BINDIR)/$(TESTTARGET) : directories $(TESTSRCDIR)/$(TESTSRC) $(BINDIR)/$(LIBTARGET)
	$(CC) -L$(BINDIR),$(LIBCHECKDIR) $(CFLAGS) $(TESTSRCDIR)/$(TESTSRC) -o $(BINDIR)/$(TESTTARGET) -l$(LIB) -l$(LIBCHECK)

all : directories $(BINDIR)/$(LIBTARGET) $(BINDIR)/$(APPTARGET) $(BINDIR)/$(TESTTARGET)
	echo "Building all..."

