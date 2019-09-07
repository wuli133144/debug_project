#include <stdio.h>
#include <stdlib.h>


#define CUNIT_DEBUG
#include <registry/registry.h>

//curl test

#include <curl/curl.h>

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
//REGISTER_STANDALONE_TEST_CUNIT_FUNC(testfunc1);

static inline size_t handlecallback(void *buffer,size_t size,size_t nmemb,void *fp)
{   
    char * pbuf = (char *)buffer;
    //size = strlen(pbuf);
    //printf("%s",pbuf);
    return size*nmemb;
}

static inline int testcurlGet(TEST_ENTRY *entry) 
{
    CURL *handle = curl_easy_init();
    if (handle == NULL) {
        printf("init curl failed!");
        exit(-1);
    } else {
        printf("init curl success!");
    }

    curl_easy_setopt(handle,CURLOPT_URL,"www.baidu.com");
    curl_easy_setopt(handle,CURLOPT_HTTPGET,1);
    curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION,handlecallback);
    curl_easy_setopt(handle,CURLOPT_WRITEDATA,NULL);

    CU_ASSERT_EQUAL(curl_easy_perform(handle),CURLE_OK);
    return 0;
}
REGISTER_STANDALONE_TEST_CUNIT_FUNC(testcurlGet);

int main(int argc,char **argv) {
    //argc = 0;
    
    curl_global_init(CURL_GLOBAL_ALL);
    //curl_easy_getinfo(handle,)
    if (argc > 1) {
        int targc = argc - 1;
        //printf("=====%s===",argv[1]);
        //execute_test_designated_func(argv[1], (void *)&targc, argv + 1);
        execute_test_cunit_designated_func(argv[1], (void *)&targc, argv + 1);
    } else {
        execute_test_funcs_cunit();
    }
    curl_global_cleanup();
  return 0;
}