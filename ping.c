#include <netinet/ip_icmp.h>
#include<sys/socket.h>

unsigned short checksum(void *b, int len) {	
    unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;

	if ( len == 1 )
		sum += *(unsigned char*)buf;

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void ping(char *ipAddress) {	
	int pingSocket =     
}

int main(int count, char *strings[]) {

    ping("127.0.0.1");
}
