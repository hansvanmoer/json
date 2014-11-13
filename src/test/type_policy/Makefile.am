#
# JSON read test makefile
#
bin_PROGRAMS=json_basic_type_policy_test
json_basic_type_policy_test_SOURCES = BasicTypePolicyTest.cpp
json_basic_type_policy_test_CPPFLAGS= -DNO_THROW='throw()' -std=c++11 -I.. -I../../main
json_basic_type_policy_test_LDADD=$(top_srcdir)/src/main/libsimplejson.a

