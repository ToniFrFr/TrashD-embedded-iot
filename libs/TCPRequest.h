#ifndef _TCP_REQUEST_H
#define _TCP_REQUEST_H

#include <stdio.h>
#include <string>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

/**
 * @brief TCPRequest includes all of the callback functions for the TCP requests and connections. The functions were developed based on different tutorials, guides and API documentation
 * To make a request, the tcp_setup must be called first, which will then setup the TCP connection. The tcp_setup then starts to call rest of the wireless library functionality.
 * connectCallback is called to handle connection status and context, which will then call library functions to write request options and info
 * tcpRecvCallback is called to handle received info, tcpSentCallback when the request is sent, tcpErrorHandler handles all unexpected error states of the connection
 * The void *arg on the callback functions is used to handle passing of programmer made arguments, in this code it includes the POSTRequestData struct
 */

typedef struct POSTRequestData
{
    char * ipAddress;
    std::string bodyString;
    std::string addressRoute;
    uint16_t port;
} POSTRequestData;


void tcp_setup(tcp_pcb *connPcb, POSTRequestData *data);
err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
err_t tcpSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcpErrorHandler(void *arg, err_t err);


#endif