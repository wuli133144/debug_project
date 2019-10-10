#include <string.h>
#include "message_factory.h"

#define MESSAGE_FACTORY(__messagename) \
	{#__messagename, &(__messagename##__descriptor)},

struct MessageFactory mf[MAX_PROTOS_PER_FACTORY] = {
	#include "message_factory.map"
};		


const ProtobufCMessageDescriptor *protobuf_c_get_descriptor(char *messagename)
{
	int i;

	for (i = 0; i < MAX_PROTOS_PER_FACTORY; i++) {
		if (!strcmp(messagename, mf[i].messagename)) {
			return mf[i].descriptor;
		}
	}

	return NULL;
}

