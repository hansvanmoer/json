#
# JSON read test makefile
#
bin_PROGRAMS=flags_test

flags_test_SOURCES = FlagsTest.cpp
flags_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
flags_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a