#
# Buffered document test makefile
#
bin_PROGRAMS=buffered_document_test

buffered_document_test_SOURCES = BufferedDocumentTest.cpp
buffered_document_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
buffered_document_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a