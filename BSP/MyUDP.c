#include "MyUDP.h"
#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "spi.h"
#include <string.h>  // memcmp
#include "usart.h"

uint8_t buff[128];  
uint8_t UDP_send_buff[128]; 
                                                    
uint8_t remote_ip[4] = {192, 168, 1, 22}; //远程IP地址
uint16_t remote_port = 5002; //远程端口号
uint16_t local_port = 5001; //local端口号

//片选
void W5500_Select(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_RESET);
}
void W5500_Unselect(void) {
    HAL_GPIO_WritePin(W5500_CS_GPIO_Port, W5500_CS_Pin, GPIO_PIN_SET);
}
void W5500_Restart(void) {
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);  // delay 1ms
    HAL_GPIO_WritePin(W5500_RST_GPIO_Port, W5500_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(1600);  // delay 1600ms
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi2, buff, len, HAL_MAX_DELAY);
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi2, buff, len, HAL_MAX_DELAY);
}

uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}

//配置W5500网络信息
wiz_NetInfo gSetNetInfo = {
  .mac  = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11},
  .ip   = {192, 168, 1, 2},
  .sn   = {255, 255, 255, 0},
  .gw   = {192, 168, 1, 1},
  .dns  = {144, 144, 144, 144},
  .dhcp = NETINFO_STATIC};

wiz_NetInfo gGetNetInfo;

enum Status
{
  Failed = 0,
  Success = 1
};

/**
 * @brief valid the result of set net info
 * @return 1: Success
 *         0: Failed
*/
uint8_t validSetNetInfoResult(wiz_NetInfo* _set, wiz_NetInfo* _get)
{
  return (!memcmp(_set, _get, sizeof(wiz_NetInfo)));  // if same, memcmp return 0
}


void UDPinit(void)
{
  W5500_Restart();  // hardware restart through RESET pin
  reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
  reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);


  ctlnetwork(CN_SET_NETINFO, (void*)&gSetNetInfo);  // set net info
  // maybe need delay
  ctlnetwork(CN_GET_NETINFO, (void*)&gGetNetInfo);  // get net info

  // W5500 has 8 channel, 32k buffer, 2 means 2KBytes
  uint8_t buffer_size_8channel_tx_rx[16] = {2, 2, 2, 2, 2, 2, 2, 2,  // 8 channel tx
                                            2, 2, 2, 2, 2, 2, 2, 2}; // 8 channel rx
  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)buffer_size_8channel_tx_rx))
  {
    // failed
    
  }

  // uint8_t sta = getSn_SR(SOCK_UDPS);
  // if(sta == SOCK_CLOSED)
  // {
  //   socket(SOCK_UDPS, Sn_MR_UDP, 5001, 0);
  // }
  HAL_Delay(100);

}


void do_udp(void)
{                                                              
  static uint32_t last_send_time = 0;
	uint16_t len=0;	
  uint8_t str1[] = "hello wwww\n";
	switch(getSn_SR(SOCK_UDPS))       /*获取socket的状态*/
	{
		case SOCK_CLOSED:                 /*socket处于关闭状态*/
			socket(SOCK_UDPS,Sn_MR_UDP,5001,0);    /*初始化socket*/
		  break;
		
		case SOCK_UDP:      /*socket初始化完成*/
			HAL_Delay(10);  
			if(getSn_IR(SOCK_UDPS) & Sn_IR_RECV) //检查是否有接收中断
			{
				setSn_IR(SOCK_UDPS, Sn_IR_RECV);  /*清接收中断*/
			}
			if((len=getSn_RX_RSR(SOCK_UDPS))>0)  /*接收到数据*/
			{
				recvfrom(SOCK_UDPS,buff, len, remote_ip,&remote_port);               /*W5500接收计算机发送来的数据W5500接收计算机发送来的数据，并获取发送方的IP地址和端口号*/
				sendto(SOCK_UDPS,str1, sizeof(str1)/sizeof(str1[0]), remote_ip, remote_port);                /*W5500把接收到的数据发送*/
        // HAL_UART_Transmit(&huart1, &remote_port, 1, 1000);
				sendto(SOCK_UDPS,buff,len-8, remote_ip, remote_port);                /*W5500把接收到的数据发送*/

        Analysis(buff);//分析数据
        memset(buff, 0, sizeof(buff));
			}
            // if (HAL_GetTick() - last_send_time >= 1000)  // 每秒发送一次
            // {
            //     sendto(SOCK_UDPS, str1, sizeof(str1), remote_ip, remote_port);  // 发送数据
            //     last_send_time = HAL_GetTick();  // 更新最后发送时间
            // }
			break;
	}

}
void do_udp1(void)
{                                                              
  static uint32_t last_send_time = 0;
	uint16_t len=0;	
  uint8_t str1[] = "hello wwww\n";
	switch(getSn_SR(SOCK_UDPS))       /*获取socket的状态*/
	{
		case SOCK_CLOSED:                 /*socket处于关闭状态*/
			socket(SOCK_UDPS,Sn_MR_UDP, local_port,0);    /*初始化socket*/
		  break;
		
		case SOCK_UDP:      /*socket初始化完成*/
			HAL_Delay(10);  
				sendto(SOCK_UDPS,str1, sizeof(str1)/sizeof(str1[0]), remote_ip, remote_port);                /*W5500把接收到的数据发送*/
			break;
	}

}

void UDP_send(uint8_t* data,uint8_t len)
{
  sendto(SOCK_UDPS, data, len, remote_ip, remote_port);
  memset(data, 0, len);
}

//分析数据
void Analysis(uint8_t *buf)
{

}
void do_tcp_server(void)
{                                                              
	uint16_t len=0;	
	uint8_t str[]="I am TCP Server\n";
	switch(getSn_SR(SOCK_TCPS))       /*获取socket的状态*/
	{
	case SOCK_CLOSED:                 /*socket处于关闭状态*/
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, 0);    /*初始化socket*/
	  break;
	case SOCK_INIT:
		listen(SOCK_TCPS);
		break;
	case SOCK_ESTABLISHED:      /*socket初始化完成*/

		if(getSn_IR(SOCK_TCPS) & Sn_IR_CON) //检查是否有接收中断
		{
			setSn_IR(SOCK_TCPS, Sn_IR_CON);  /*清接收中断*/
		}
		if((len=getSn_RX_RSR(SOCK_TCPS))>0)  /*接收到数据*/
		{
			recv(SOCK_TCPS,buff, len);               /*W5500接收计算机发送来的数据W5500接收计算机发送来的数据，并获取发送方的IP地址和端口号*/
			
			//sendto(SOCK_TCPS,buff,len-8, remote_ip, remote_port);                /*W5500把接收到的数据发送*/
			printf("%s", buff);
			send(SOCK_TCPS, str, sizeof(str)/sizeof(str[0]));
        Analysis(buff);//分析数据
        memset(buff, 0, sizeof(buff));
			}
			break;
	case SOCK_CLOSE_WAIT:
		close(SOCK_TCPS);
		break;
	}

}
void do_tcp_server1(void)
{                                                              
	uint16_t len=0;	
	uint8_t str[]="I am TCP Server\n";
	switch(getSn_SR(SOCK_TCPS))       /*获取socket的状态*/
	{
	case SOCK_CLOSED:                 /*socket处于关闭状态*/
		socket(SOCK_TCPS, Sn_MR_TCP, local_port, 0);    /*初始化socket*/
	  break;
	case SOCK_INIT:
		listen(SOCK_TCPS);
		break;
	case SOCK_ESTABLISHED:      /*socket初始化完成*/

			printf("%s",str);
			send(SOCK_TCPS, str, sizeof(str)/sizeof(str[0]));
			break;
	case SOCK_CLOSE_WAIT:
		close(SOCK_TCPS);
		break;
	}

}
void do_tcp_client(void)
{                                                              
	uint16_t len=0;	
	uint8_t str[]="I am TCP Client\n";
	switch(getSn_SR(SOCK_TCPC))       /*获取socket的状态*/
	{
	case SOCK_CLOSED:                 /*socket处于关闭状态*/
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);    /*初始化socket*/
	  break;
	case SOCK_INIT:
		connect(SOCK_TCPC, remote_ip, remote_port);
		break;
	case SOCK_ESTABLISHED:      /*socket初始化完成*/
		if(getSn_IR(SOCK_TCPC) & Sn_IR_CON) //检查是否有接收中断
		{
			setSn_IR(SOCK_TCPC, Sn_IR_CON);  /*清接收中断*/
		}
		if((len=getSn_RX_RSR(SOCK_TCPC))>0)  /*接收到数据*/
		{
			recv(SOCK_TCPC,buff, len);               /*W5500接收计算机发送来的数据W5500接收计算机发送来的数据，并获取发送方的IP地址和端口号*/
			printf("%s", buff);
			send(SOCK_TCPC, str, sizeof(str)/sizeof(str[0])); /*W5500把接收到的数据发送*/
		        Analysis(buff);//分析数据
		        memset(buff, 0, sizeof(buff));
			}
			break;
	case SOCK_CLOSE_WAIT:
		close(SOCK_TCPC);
		break;
	}

}
void do_tcp_client1(void)
{                                                              
	uint16_t len=0;	
	uint8_t str[]="I am TCP Client 1\n";
	switch(getSn_SR(SOCK_TCPC))       /*获取socket的状态*/
	{
	case SOCK_CLOSED:                 /*socket处于关闭状态*/
		socket(SOCK_TCPC, Sn_MR_TCP, local_port, Sn_MR_ND);    /*初始化socket*/
	  break;
	case SOCK_INIT:
		connect(SOCK_TCPC, remote_ip, remote_port);
		break;
	case SOCK_ESTABLISHED:      /*socket初始化完成*/
			printf("%s", str);
			send(SOCK_TCPC, str, sizeof(str)/sizeof(str[0])); /*W5500把接收到的数据发送*/
			break;
	case SOCK_CLOSE_WAIT:
		close(SOCK_TCPC);
		break;
	}

}
#define UDP 0
#define UDP1 1
#define TCPS 2
#define TCPS1 3
#define TCPC  4
#define TCPC1 5

void w5500_main(void){
  char str[] = "hello w5500\n";
  char udp[] = "this is udp\n";
  char udp1[] = "this is udp1\n";
  char tcps[] = "this is tcp server\n";
  char tcps1[] = "this is tcp server 1\n";
  char tcpc[] = "this is tcp client\n";
  char tcpc1[] = "this is tcp client1\n";
  int select = 3; // 1:udp1 0:udp
  // int procotol = UDP;
  // int procotol = UDP1;
  // int procotol = TCPS;
  // int procotol = TCPS1;
  // int procotol = TCPC;
  int procotol = TCPC1;
  
  UDPinit();
  
  while(1){

    switch(procotol){
      case UDP:
        HAL_UART_Transmit(&huart1, udp, sizeof(udp)/sizeof(udp[0]), HAL_MAX_DELAY);
        do_udp();
        break;
      case UDP1:
        HAL_UART_Transmit(&huart1, udp1, sizeof(udp1)/sizeof(udp1[0]), HAL_MAX_DELAY);
        do_udp1();
        HAL_Delay(1000);
        break;
      case TCPS:
        HAL_UART_Transmit(&huart1, tcps, sizeof(tcps)/sizeof(tcps[0]), HAL_MAX_DELAY);
        do_tcp_server();
        break;
      case TCPS1:
        HAL_UART_Transmit(&huart1, tcps1, sizeof(tcps1)/sizeof(tcps1[0]), HAL_MAX_DELAY);
        do_tcp_server1();
        HAL_Delay(1000);
        break;
      case TCPC:
        HAL_UART_Transmit(&huart1, tcpc, sizeof(tcpc)/sizeof(tcpc[0]), HAL_MAX_DELAY);
        do_tcp_client();
        break;
      case TCPC1:
        HAL_UART_Transmit(&huart1, tcpc1, sizeof(tcpc1)/sizeof(tcpc1[0]), HAL_MAX_DELAY);
        do_tcp_client1();
        HAL_Delay(1000);
        break;
    }

    // vTaskDelay(500);

  }
}