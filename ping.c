#include <stdio.h>
#include<sys/socket.h>

typdef struct {
	uint8_t type;
	uint8_t code;
	uint16_t chksum;
	uint32_t data;
} icmp_header_struct;

int32_t checksum(uint16_t *buf, int32_t len) {    
    int32_t nleft = len;
    int32_t sum = 0;
    uint16_t *w = buf;
    uint16_t answer = 0;

    while(nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }
    
    if(nleft == 1) {
        sum += *(uint8_t *)w;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

void ping(char *ip_address) {	
	int ping_socket = socket(PF_INET, SOCK_RAW, 1);
	if ping_socket < 0 {
		printf("El socket no se ha creado correctamente");
		exit(0);
	}     

	icmp_header_struct header;
	header.code = 8; //echo
	header.type = 0;
	header.chksum = 0;
	header.data = 0;

	header.chksum =  checksum((uint16_t *)&header, sizeof(header));

	struct sockaddr_in ip_address;
	ip_address.sin_family = AF_INET;
	ip_address.sin_port = 0;
	ip_address.sin_address.s_addr = inet_addr(ip_address);

	int result = sendto(ping_socket, &header, sizeof(header), 0, (struct sockaddr*)&ip_address, sizeof(ip_address));

}

int main(int count, char *strings[]) {	
    ping("127.0.0.1");
}
