## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = TestRomNumCalc
TESTS = TestRomNumCalc

TestRomNumCalc_SOURCES = TestRomNumCalc.c ../src/lib/RomNumMath.h
TestRomNumCalc_CFLAGS = $(CFLAGS) $(CHECK_CFLAGS)
TestRomNumCalc_LDADD = ../lib/libRomNumMath.la $(LDFLAGS) $(CHECK_LIBS)
