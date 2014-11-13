#
# JSON read test makefile
#
bin_PROGRAMS=strict_storage_policy_test
strict_storage_policy_test_SOURCES = StrictStoragePolicyTest.cpp
strict_storage_policy_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
strict_storage_policy_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a

