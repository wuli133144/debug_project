    
#if !defined(__MESSAGE_H)
#define __MESSAGE_H

#include <libobject/core/map.h>
#include <libobject/core/obj.h>
#include <libobject/core/queue.h>
#include <libobject/core/utils/dbg/debug.h>
#include <stdio.h>

typedef struct Message_S Message;

struct Message_S
{
    Obj obj;

    int (*construct)(Message *, char *init_str);
    int (*deconstruct)(Message *);
    int (*set)(Message *, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);
    void (*hello)(Message *);
    /*virtual methods reimplement*/

    /*
    */
};

#endif 
