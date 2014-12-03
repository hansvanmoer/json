#
# Makefile script
#

ACLOCAL_AMFLAGS = -I m4
SUBDIRS=src/main\
    src/test/flags\
    src/test/read\
    src/test/tokens\
    src/test/type_policy\
    src/test/storage_policy\
    src/test/input_buffer

dist_doc_DATA=README.md

TESTS=src/test/tokens/json_tokens_test\
    src/test/flags/flags_test\
    src/test/type_policy/json_basic_type_policy_test\
    src/test/storage_policy/strict_storage_policy_test\
    src/test/input_buffer/eager_input_buffer_test\
    src/test/input_buffer/lazy_input_buffer_test
