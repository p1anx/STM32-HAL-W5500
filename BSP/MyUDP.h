#ifndef MYUDP_H
#define MYUDP_H
#include "MyUDP.h"
#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "spi.h"
#include <string.h>  // memcmp
#define SOCK_TCPS  0    //定义socket号
#define SOCK_TCPC  1    //定义socket号
#define SOCK_UDPS  2    //定义socket号
extern  uint8_t buff[128];   //定义缓冲区   
extern uint8_t UDP_send_buff[128];   //定义UDP发送缓冲区                                            
extern uint8_t remote_ip[4]; //远程IP地址
extern uint16_t remote_port; //远程端口号

void UDPinit(void);
void do_udp(void);
void UDP_send(uint8_t* data,uint8_t len);
void Analysis(uint8_t *buf);
void do_udp1(void);
void do_tcp_server(void);
void do_tcp_server1(void);
void do_tcp_client(void);
void do_tcp_client1(void);
void w5500_main(void);
#endif // MYUDP_H
