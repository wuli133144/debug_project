#include <stdio.h>
#include <stdlib.h>
#include "protos/message_factory.h"

int main (int argc, const char * argv[]) 
{
	Env msg; // AMessage
	const ProtobufCMessageDescriptor *desc = NULL;
	void *buf;                     // Buffer to store serialized data
	unsigned len;                  // Length of serialized data
	FILE *fp = NULL;

	fp = fopen("./amessage.out", "w+");

	desc = protobuf_c_get_descriptor("env");

	protobuf_c_message_init(desc, (void *)&msg);

	msg.vsysid = 1;
	msg.ha = 2;
	msg.lang = 3;
	msg.from = 4;
	msg.sessionid = 5;
	msg.username = "root";

	len = protobuf_c_message_get_packed_size((const ProtobufCMessage *)&msg);

	buf = malloc(len);
	protobuf_c_message_pack((const ProtobufCMessage *)&msg,buf);

	fprintf(stderr,"Writing %d serialized bytes\n",len); // See the length of message
	fwrite(buf,len,1,fp); // Write to stdout to allow direct command line piping

	free(buf); // Free the allocated serialized buffer
	fclose(fp);
	return 0;
}
