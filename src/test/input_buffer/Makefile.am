#
# JSON read test makefile
#
bin_PROGRAMS=eager_input_buffer_test lazy_input_buffer_test

eager_input_buffer_test_SOURCES = EagerInputBufferTest.cpp
eager_input_buffer_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
eager_input_buffer_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a

lazy_input_buffer_test_SOURCES = LazyInputBufferTest.cpp
lazy_input_buffer_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
lazy_input_buffer_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a

