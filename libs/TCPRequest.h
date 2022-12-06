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
} POSTRequestData;


static void tcp_setup(tcp_pcb *connPcb, std::string ipAddress, std::string postBody);
static err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcpSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcpErrorHandler(void *arg, err_t err);


#endif