#include "TCPRequest.h"

void tcp_setup(tcp_pcb *connPcb, POSTRequestData *data)
{
    ip_addr_t ip;

    ip4addr_aton(data->ipAddress, &ip);

    connPcb = tcp_new();

    tcp_arg(connPcb, data);

    tcp_err(connPcb, tcpErrorHandler);
    tcp_recv(connPcb, tcpRecvCallback);
    tcp_sent(connPcb, tcpSentCallback);

    tcp_connect(connPcb, &ip, data->port, connectCallback);
}

err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    

    //POSTRequestData *args = (POSTRequestData *)arg;
    POSTRequestData *args = static_cast<POSTRequestData *>(arg);

    std::string ipAddress = std::string(args->ipAddress);
    std::string postBody = args->bodyString;
    uint16_t port = args->port;

    if (err != ERR_OK) {
        printf("Error with establishing connection\n");
        return 1;
    }
    else
    {
        printf("Connected, sending packet...\n");
        std::string httpString = "POST /posts HTTP/1.1\r\nHost: " + ipAddress + ":" + std::to_string(port) + "\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(postBody.length()) + "\r\n\r\n" + postBody + "\r\n\r\n";
        //std::string httpString = "HEAD / HTTP/1.1\r\nHost: " + ipAddress + ":8080" + "\r\n\r\n";
        uint16_t length = httpString.length();

        err_t error = tcp_write(tpcb, (char *)httpString.c_str(), length, TCP_WRITE_FLAG_COPY);

        if (error != ERR_OK)
        {
            printf("Error in tcp_write, error: %d\n", error);

            return 1;
        } else {
            error = tcp_output(tpcb);
            if(error != ERR_OK) {
                printf("Error with sending the request\n");
                return 1;
            }

            printf("Http string: %s\n", httpString.c_str());
        }
    }
    return 0;
}
err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    printf("Data recieved.\n");
    if (p == NULL) {
        printf("The remote host closed the connection.\n");
        printf("Connection closed as pbuf == NULL.\n");
    } else {
        printf("Number of pbufs %d\n", pbuf_clen(p));
        printf("Contents of pbuf %s\n", (char *)p->payload);
    }

    pbuf_free(p);

    err_t error = tcp_close(tpcb);

    if(error != ERR_OK) {
        printf("Aborting connection as it's not closing properly \n");
        tcp_abort(tpcb);
    }

    tpcb = NULL;

    return 0;
}
err_t tcpSentCallback(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    printf("Client information sent to the server...\n");

    return ERR_OK;
}
void tcpErrorHandler(void *arg, err_t err) {
    if (err != ERR_ABRT) {
        printf("tcp_client_err %d\n", err);
    } else {
        printf("Abort error\n");
    }
}
