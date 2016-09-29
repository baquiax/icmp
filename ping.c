#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h> 

typedef struct {
	uint8_t type;
	uint8_t code;
	uint16_t chksum;
	uint16_t id;
	uint16_t seq;
	char data[4];
} icmp_echo_request_struct;

typedef struct {
	uint8_t type;
	uint8_t code;
	uint16_t chksum;
	uint16_t id;
	uint16_t seq;
	char data[4];
} icmp_echo_reply_struct;

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


double ping(char *ip_address) {	
   clock_t start = clock();   

	int ping_socket = socket(PF_INET, SOCK_RAW, 1);
	if (ping_socket <= 0) {
		printf("El socket no se ha creado correctamente %d\n", ping_socket);
		exit(0);
	}     

	icmp_echo_request_struct request;

	request.code = 0;
	request.type = 8;
	request.chksum = 0;
	request.id = getpid();
	request.seq = 1;	
	request.data[0] = 'a';
	request.data[1] = 'b';
	request.data[2] = 'c';
	request.data[3] = '\0';
	request.chksum =  checksum((uint16_t *)&request, sizeof(request));

	struct sockaddr_in ip;
	ip.sin_family = AF_INET;
	ip.sin_port = 0;
	ip.sin_addr.s_addr = inet_addr(ip_address);
	
	int result = sendto(ping_socket, &request, sizeof(request), 0, (struct sockaddr*)&ip, sizeof(ip));
	printf("ICMP sent!\n");

	if (result < 0) {
		printf("No se ha enviado el ping\n");
		exit(0);		
	}

    unsigned int response_address_size;
	struct sockaddr response_address;
    unsigned char response[30] = "";
	
	int receive = recvfrom(ping_socket, response, sizeof(response), 0, &response_address, &response_address_size);	
	printf("Response: %d\n", receive);
	
	if (receive < 0) {
		printf("Error al recibir la respuesta\n");
		exit(0);
	}

	printf("Response size: %d\n", response_address_size);
	clock_t end = clock();
   	double diff = (end)  - (start);
	return diff;
}

int main(int argc, char *strings[]) {
	if (argc < 2) {
		printf("An IP is required\n");
		exit(0);
	}
	for (int i = 0; i < 10 ; i++) {
		printf("Attemp #%d - Time %.2f\n",i,ping(strings[1])/1000);
	}
    
	printf("BYE\n");
}
