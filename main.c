#include <stdio.h>
#include <stdlib.h>
#include <registry/registry.h>

#define CUNIT_DEBUG

static inline int testfunc(TEST_ENTRY *entry) {
    printf("testfunc running====");
    int ret = 1;
    CU_ASSERT_EQUAL(ret,1);
    CU_ASSERT_EQUAL(ret,2);
    return 1;
}

static inline int testfunc1(TEST_ENTRY *entry) {
    printf("testfunc1 running====");
    char *p = "he";
    CU_ASSERT_STRING_EQUAL(p,"he");
    CU_ASSERT_STRING_EQUAL(p,"hello");
    CU_ASSERT_STRING_NOT_EQUAL(p,"hello");
    CU_ASSERT_STRING_NOT_EQUAL(p,"he");
    return 1;
}
//REGISTER_TEST_FUNC(testfunc);

REGISTER_STANDALONE_TEST_CUNIT_FUNC(testfunc);
REGISTER_STANDALONE_TEST_CUNIT_FUNC(testfunc1);

int main(int argc,char **argv) {
    //argc = 0;
    if (argc > 0) {
        int targc = argc - 1;
        //printf("=====%s===",argv[1]);
        //execute_test_designated_func(argv[1], (void *)&targc, argv + 1);
        execute_test_cunit_designated_func(argv[1], (void *)&targc, argv + 1);
    } else {
        execute_test_funcs();
    }
  return 0;
}