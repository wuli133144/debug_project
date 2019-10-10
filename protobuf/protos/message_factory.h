#ifndef __PROTOS_FACTORY_H__
#define __PROTOS_FACTORY_H__

#include "google/protobuf-c/protobuf-c.h"
#include "protos/protos.h"

#define MESSAGE_DESCRIPTOR_SUFFIX	"message__descriptor"

#define MAX_PROTOS_PER_FACTORY	1024

struct MessageFactory {
#define PROTO_MESSAGE_NAME_LEN	32
	char messagename[PROTO_MESSAGE_NAME_LEN];
	const ProtobufCMessageDescriptor *descriptor;
};

const ProtobufCMessageDescriptor *protobuf_c_get_descriptor(char *messagename);

#endif
