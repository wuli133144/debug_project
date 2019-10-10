#include <stdio.h>
#include <stdlib.h>
#include "protos/message_factory.h"

#define MAX_MSG_SIZE 1024

	static size_t
read_buffer (unsigned max_length, uint8_t *out)
{
	size_t cur_len = 0;
	uint8_t nread;
	FILE *fp;

	fp = fopen("./amessage.out", "r");

	while ((nread=fread(out + cur_len, 1, max_length - cur_len, fp)) != 0)
	{
		cur_len += nread;
		if (cur_len == max_length)
		{
			fprintf(stderr, "max message length exceeded\n");
			exit(1);
		}
	}

	fclose(fp);
	return cur_len;
}


int main (int argc, const char * argv[]) 
{
	Env *msg;
	const ProtobufCMessageDescriptor *desc = NULL;

	uint8_t buf[MAX_MSG_SIZE];
	size_t msg_len = read_buffer (MAX_MSG_SIZE, buf);
	
	desc = protobuf_c_get_descriptor("env");

	// Unpack the message using protobuf-c.
	msg = (Env *)protobuf_c_message_unpack(desc, NULL, msg_len, (const uint8_t*)buf);   
	if (msg == NULL)
	{
		fprintf(stderr, "error unpacking incoming message\n");
		exit(1);
	}

	// display the message's fields.
	printf("Received: vsysid=%d\n", msg->vsysid);
	printf("Received: ha=%d\n", msg->ha);
	printf("Received: lang=%d\n", msg->lang);
	printf("Received: from=%d\n", msg->from);
	printf("Received: sessionid=%d\n", msg->sessionid);
	printf("Received: username=%s\n", msg->username);
	printf("\n");

	// Free the unpacked message
	protobuf_c_message_free_unpacked((ProtobufCMessage *)msg, NULL);
	return 0;
}
