#!/bin/bash

#echo "hello world!~"

FILE_NAME=$1
FILE_NAME_UPPER=$(echo $FILE_NAME | tr '[a-z]' '[A-Z]')
FILE_NAME_LOWWER=$(echo $FILE_NAME | tr '[A-Z]' '[a-z]')
CLASS_NAME=$(echo $FILE_NAME_LOWWER | sed -e "s/\b\(.\)/\u\1/g")

#VALID=$(grep -rn "generator" | wc -l)

if [[ -e ${FILE_NAME}.h ]] && [[ -e ${FILE_NAME}.c ]];then
    echo "file ${FILE_NAME}.h/.c has existed. do u wanna modify it?"
    exit -1
fi
 
 ###############create header file
 if [[ ! -e ${FILE_NAME}.h ]]
 then
    echo "${FILE_NAME_UPPER}"
    cat << EOF >> ${FILE_NAME}.h
    
#if !defined(__${FILE_NAME_UPPER}_H)
#define __${FILE_NAME_UPPER}_H

#include <libobject/core/map.h>
#include <libobject/core/obj.h>
#include <libobject/core/queue.h>
#include <libobject/core/utils/dbg/debug.h>
#include <stdio.h>

typedef struct ${CLASS_NAME}_S ${CLASS_NAME};

struct ${CLASS_NAME}_S
{
    Obj obj;

    int (*construct)(${CLASS_NAME} *, char *init_str);
    int (*deconstruct)(${CLASS_NAME} *);
    int (*set)(${CLASS_NAME} *, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);

    /*virtual methods reimplement*/

    /*
    */
};

#endif 
EOF
fi

#### create source file *.c 
 if [[ ! -e ${FILE_NAME}.c ]]
 then
    echo "${FILE_NAME_UPPER}"
    cat << EOF >> ${FILE_NAME}.c
    
/**
 * @file ${FILE_NAME}.c
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
#include "${FILE_NAME}.h"
#include <libobject/core/utils/config/config.h>
#include <libobject/core/utils/dbg/debug.h>
#include <libobject/core/utils/timeval/timeval.h>
#include <stdio.h>
#include <unistd.h>


static int __construct(${CLASS_NAME} *self, char *init_str)
{
    allocator_t *allocator = self->obj.allocator;
    dbg_str(DBG_DETAIL, "${CLASS_NAME} construct, Person addr:%p", self);

    return 0;
}

static int __deconstrcut(${CLASS_NAME} *self)
{
    dbg_str(DBG_DETAIL, "${CLASS_NAME} deconstruct, stack addr:%p", self);
    return 0;
}

static int __set(${CLASS_NAME} *self, char *attrib, void *value)
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
    else
    {
        dbg_str(DBG_DETAIL, "${CLASS_NAME} set, not support %s setting", attrib);
    }

    return 0;
}

static void *__get(${CLASS_NAME} *self, char *attrib)
{
    if (strcmp(attrib, "") == 0)
    {
    }
    else
    {
        dbg_str(DBG_WARNNING, "${CLASS_NAME} *self get, \"%s\" getting attrib is not supported", attrib);
        return NULL;
    }

    return NULL;
}

static class_info_entry_t ${CLASS_NAME}_class_info[] = {
    [0] = {ENTRY_TYPE_OBJ, "${CLASS_NAME}", "parent", NULL, sizeof(void *)},
    [1] = {ENTRY_TYPE_FUNC_POINTER, "", "set", __set, sizeof(void *)},
    [2] = {ENTRY_TYPE_FUNC_POINTER, "", "get", __get, sizeof(void *)},
    [3] = {ENTRY_TYPE_FUNC_POINTER, "", "construct", __construct, sizeof(void *)},
    [4] = {ENTRY_TYPE_FUNC_POINTER, "", "deconstruct", __deconstrcut, sizeof(void *)},
    [5] = {ENTRY_TYPE_END},
};
REGISTER_CLASS("${CLASS_NAME}", ${CLASS_NAME}_class_info);
EOF
fi