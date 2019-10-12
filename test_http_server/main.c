#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <libobject/attrib_priority.h>
#include <libobject/core/utils/args/cmd_args.h>
#include <libobject/libobject.h>
#include <libobject/test.h>

#include "http_server.h"

void generic_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    if (!buf)
    {
        puts("failed to create response buffer \n");
        return;
    }

    evbuffer_add_printf(buf, "Server Responsed. Requested: %s\n", evhttp_request_get_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

int main(int argc, char const *argv[])
{
    libobject_init();

    const char *paddr = "172.24.187.139";
    unsigned short port = 8080;
    Http_server *server = new_object_default(Http_server, NULL);

    server->init(server, paddr, port);
    server->register_handle(server, "/getname", generic_handler, NULL);
    server->start();

    object_destroy(server);
    return 0;
}
