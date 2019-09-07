#include <stdio.h>
#include <stdlib.h>


#define CUNIT_DEBUG
#include <registry/registry.h>

//curl test

#include <curl/curl.h>
//#include <3rd/json-c-0.9/json.h>
#include <json/json.h>
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

static inline int testjson_c(TEST_ENTRY * entry) 
{
    //array 
    json_object * jobj = NULL;
    int i = 0;
    jobj = json_object_new_array();
    json_object_array_add(jobj,json_object_new_string("wuyujie"));
    json_object_array_add(jobj,json_object_new_string("wuyujie1"));
    json_object_array_add(jobj,json_object_new_string("wuyujie2"));
    json_object_array_add(jobj,json_object_new_string("wuyujie3"));
    json_object_array_add(jobj,json_object_new_string("wuyujie4"));
    json_object_array_add(jobj,json_object_new_string("wuyujie5"));

    //printf("%s",json_object_to_json_string(jobj));

    CU_ASSERT_EQUAL(json_object_array_length(jobj),6);
    
    CU_ASSERT_STRING_EQUAL(json_object_get_string(json_object_array_get_idx(jobj,0)),"wuyujie");

    for (i = 0;i < json_object_array_length(jobj);i++) {
        if (i > 0) {
            char buf[100] = {0};
            sprintf(buf,"wuyujie%d",i);
            CU_ASSERT_STRING_EQUAL(json_object_get_string(json_object_array_get_idx(jobj,i)),buf);
        }else {
            CU_ASSERT_STRING_EQUAL(json_object_get_string(json_object_array_get_idx(jobj,i)),"wuyujie");
        }
    }
    
    return 1;
}
REGISTER_STANDALONE_TEST_CUNIT_FUNC(testjson_c);

static inline int testjson_c1(TEST_ENTRY *entry) 
{
    //construct json object
    json_object * root = NULL;
    json_object * array = NULL;
    json_object * name = NULL;

    root = json_object_new_object();
    if (root == NULL) {
        printf("construct object json failed");
        return -1;
    }
    
    array = json_object_new_array();
    if (!array) {
        goto error;
    }
    json_object_array_add(array,json_object_new_string("test1"));
    json_object_array_add(array,json_object_new_string("test2"));
    json_object_array_add(array,json_object_new_string("test3"));

    name = json_object_new_string("name");
    if (!name) {
        goto error;
    }

    json_object_object_add(root,"array",array);
    json_object_object_add(root,"name",name);

    printf("%s",json_object_to_json_string(root));
    json_object_put(root);

    return 1;
    
error:
    json_object_put(root);
    return 0;
}
REGISTER_STANDALONE_TEST_CUNIT_FUNC(testjson_c1);

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