#
# JSON read test makefile
#
bin_PROGRAMS=json_tokens_test
json_tokens_test_SOURCES = main.cpp
json_tokens_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
json_tokens_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a

