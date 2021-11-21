#ifndef INC_TCP_CLIENT_H_
#define INC_TCP_CLIENT_H_
#include "lwip/tcp.h"

enum connectionState { NONE = 0, CONNECTED, RECEIVING, DISCONNECTING };

struct tcpClientDetails {
	connectionState state;
	uint8_t retries;
	struct tcp_pcb *controlBlock;
	struct pbuf *buffer;
};

class TcpClient {
private:
	static err_t connectedCallback(void* clientDetails, tcp_pcb *controlBlock, err_t err);  // fired when client connects to remote server
	static err_t recvCallback(void *clientDetails, tcp_pcb *controlBlock, pbuf *buffer, err_t err);  // fired when data is received
	static err_t pollCallback(void *clientDetails, tcp_pcb *controlBlock);  // fired when server polls for client
	static err_t ackCallback(void *clientDetails, tcp_pcb *controlBlock, u16_t len);  // fired when server acknowledges

	static void send(tcpClientDetails *clientDetails);  // sends data to remote server
	static void disconnect(tcp_pcb *controlBlock, tcpClientDetails *clientDetails);
	static void handle(tcp_pcb *controlBlock, tcpClientDetails *clientDetails);

public:
	static void init();
};

#endif
