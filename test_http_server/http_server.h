    
#if !defined(__HTTP_SERVER_H)
#define __HTTP_SERVER_H

#include <libobject/core/map.h>
#include <libobject/core/obj.h>
#include <libobject/core/queue.h>
#include <libobject/core/utils/dbg/debug.h>
#include <stdio.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

typedef struct Http_server_S Http_server;

struct Http_server_S
{
    Obj obj;

    int (*construct)(Http_server *, char *init_str);
    int (*deconstruct)(Http_server *);
    int (*set)(Http_server *, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);

    void (*init)(Http_server * ,const char *ipaddr,short port);
    void (*start)(void);
    void (*register_handle)(Http_server*,const char *uri,void (*)(struct evhttp_request *,void *arg),void *arg);
    
    /*virtual methods reimplement*/
    struct evhttp *__http_server;
    struct timeval timeout;
    
    char ip[100];
    unsigned short port;
    /*
    */
};

#endif 
