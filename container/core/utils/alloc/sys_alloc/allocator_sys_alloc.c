/*  
 * Copyright (c) 2015-2020 alan lin <a1an1in@sina.com>
 *  
 *  
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libobject/core/utils/alloc/inc_files.h>
#include <libobject/core/utils/registry/registry.h>

static void *__alloc(allocator_t *alloc, uint32_t size)
{
    return malloc(size);
}

static void __free(allocator_t *alloc, void *addr)
{
    free(addr);
}

#ifdef __JEMALLOC_ALLOC
#include <jemalloc/jemalloc.h>
static void *jemalloc_alloc(allocator_t *alloc,uint32_t size)
{
    return je_malloc(size);
}

static void jemalloc_free(allocator_t *alloc,void *addr)
{
    return je_free(addr);
}

#endif

int allocator_sys_alloc_register(){
    allocator_module_t salloc = {
        .allocator_type = ALLOCATOR_TYPE_SYS_MALLOC, 
        .alloc_ops = {
            .init    = NULL, 
            .alloc   = __alloc, 
            .free    = __free, 
            .destroy = NULL, 
        }
    };
    memcpy(&allocator_modules[ALLOCATOR_TYPE_SYS_MALLOC], &salloc, sizeof(allocator_module_t));
    #ifdef __JEMALLOC_ALLOC
        allocator_module_t salloc_jemalloc = {
        .allocator_type = ALLOCATOR_TYPE_CTR_JEMALLOC, 
        .alloc_ops = {
            .init    = NULL, 
            .alloc   = jemalloc_alloc, 
            .free    = jemalloc_free, 
            .destroy = NULL, 
        }
    };
    memcpy(&allocator_modules[ALLOCATOR_TYPE_CTR_JEMALLOC], &salloc_jemalloc, sizeof(allocator_module_t));
    #endif 
    return 0;
}
REGISTER_CTOR_FUNC(REGISTRY_CTOR_PRIORITY_LIBALLOC_REGISTER_MODULES, 
                   allocator_sys_alloc_register);

