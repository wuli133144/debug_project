
/**
 * @file http_server.c
 * @Synopsis  
 * @author 
 * @version 
 * @date 2017-10-11
 */
/* Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "http_server.h"
#include <assert.h>
#include <libobject/core/utils/config/config.h>
#include <libobject/core/utils/dbg/debug.h>
#include <libobject/core/utils/timeval/timeval.h>
#include <stdio.h>
#include <string.h>

static int __construct(Http_server *self, char *init_str)
{
    allocator_t *allocator = self->obj.allocator;
    dbg_str(DBG_DETAIL, "Http_server construct, Person addr:%p", self);
    self->__http_server = NULL;
    timeval_clear(&self->timeout);
    memset(self->ip, 0, sizeof(self->ip));
    //self->port = 0;
    return 0;
}

static int __deconstrcut(Http_server *self)
{
    dbg_str(DBG_DETAIL, "Http_server deconstruct, stack addr:%p", self);
    timeval_clear(&self->timeout);
    if (self->__http_server)
    {
        evhttp_free(self->__http_server);
    }
    return 0;
}

static int __set(Http_server *self, char *attrib, void *value)
{
    if (strcmp(attrib, "set") == 0)
    {
        self->set = value;
    }
    else if (strcmp(attrib, "get") == 0)
    {
        self->get = value;
    }
    else if (strcmp(attrib, "construct") == 0)
    {
        self->construct = value;
    }
    else if (strcmp(attrib, "deconstruct") == 0)
    {
        self->deconstruct = value;
    }
    else if (strcmp(attrib, "start") == 0)
    {
        self->start = value;
    }
    else if (strcmp(attrib, "init") == 0)
    {
        self->init = value;
    }
    else if (strcmp(attrib, "register_handle") == 0)
    {
        self->register_handle = value;
    }
    else
    {
        dbg_str(DBG_DETAIL, "Http_server set, not support %s setting", attrib);
    }

    return 0;
}

static void *__get(Http_server *self, char *attrib)
{
    if (strcmp(attrib, "") == 0)
    {
    }
    else
    {
        dbg_str(DBG_WARNNING, "Http_server *self get, \"%s\" getting attrib is not supported", attrib);
        return NULL;
    }

    return NULL;
}

static void __start()
{
    event_dispatch();
    //实际上不会释放，代码不会运行到这一步
    //evhttp_free(http_server);
}

static void __init(Http_server *self, const char *addr, unsigned short p)
{
    assert(addr != NULL);
    assert(p > 0);
    self->port = p;
    strncpy(self->ip, addr, strlen(addr));
    event_init();

    self->__http_server = evhttp_start((const char *)self->ip, self->port);
    if (!self->__http_server)
    {
        //ret = -1;
        dbg_str(DBG_ERROR, "http_server start failed!");
        //return ret;
    }
    else
    {
        evhttp_set_timeout(self->__http_server, 5);
    }
}

static void __register_handle(Http_server *self,
                              const char *uri,
                              void (*handle)(struct evhttp_request *, void *arg),
                              void *arg)
{
    assert(uri != NULL);
    assert(handle != NULL);
    //assert(uri != NULL);
    evhttp_set_cb(self->__http_server, uri, handle, arg);
}

static class_info_entry_t Http_server_class_info[] = {
    [0] = {ENTRY_TYPE_OBJ, "Obj", "obj", NULL, sizeof(void *)},
    [1] = {ENTRY_TYPE_FUNC_POINTER, "", "set", __set, sizeof(void *)},
    [2] = {ENTRY_TYPE_FUNC_POINTER, "", "get", __get, sizeof(void *)},
    [3] = {ENTRY_TYPE_FUNC_POINTER, "", "construct", __construct, sizeof(void *)},
    [4] = {ENTRY_TYPE_FUNC_POINTER, "", "deconstruct", __deconstrcut, sizeof(void *)},
    [5] = {ENTRY_TYPE_FUNC_POINTER, "", "init", __init, sizeof(void *)},
    [6] = {ENTRY_TYPE_FUNC_POINTER, "", "start", __start, sizeof(void *)},
    [7] = {ENTRY_TYPE_FUNC_POINTER, "", "register_handle", __register_handle, sizeof(void *)},
    [8] = {ENTRY_TYPE_END},
};
REGISTER_CLASS("Http_server", Http_server_class_info);
