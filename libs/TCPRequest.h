#ifndef _TCP_REQUEST_H
#define _TCP_REQUEST_H

#include <stdio.h>
#include <string>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"

typedef struct POSTRequestData
{
    char * ipAddress;
    char * bodyString;
    uint16_t port;
} POSTRequestData;


void tcp_setup(tcp_pcb *connPcb, POSTRequestData *data);
err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
err_t tcpSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcpErrorHandler(void *arg, err_t err);


#endif