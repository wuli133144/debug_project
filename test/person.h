    
#if !defined(__PERSON_H)
#define __PERSON_H

#include <libobject/core/map.h>
#include <libobject/core/obj.h>
#include <libobject/core/queue.h>
#include <libobject/core/utils/dbg/debug.h>
#include <stdio.h>

typedef struct Person_S Person;

struct Person_S
{
    Obj obj;

    int (*construct)(Person *, char *init_str);
    int (*deconstruct)(Person *);
    int (*set)(Person *, char *attrib, void *value);
    void *(*get)(void *obj, char *attrib);
  
    /*virtual methods reimplement*/

    void (*print)(Person *);

    /*
    */
};

#endif 
