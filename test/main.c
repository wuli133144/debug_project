
#ifndef __JEMALLOC_ALLOC
#define __JEMALLOC_ALLOC
#endif 

#include <stdio.h>
#include <unistd.h>
#include <signal.h>



#include <libobject/attrib_priority.h>
#include <libobject/core/utils/args/cmd_args.h>
#include <libobject/libobject.h>
#include <libobject/test.h>


#include "person.h"

int main(int argc, char const *argv[])
{
    libobject_init();
    //libobjet
    printf(">>>>>>>>>>>>>");
    allocator_t *allocator = allocator_get_default_alloc();
    Person * person = OBJECT_NEW(allocator,Person,NULL);
    person->print(person);
    object_destroy(person);

    //pause();

    return 0;
}
