#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "driver/uart.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "espconn.h"
#include "osapi.h"
#include "mem.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

LOCAL char *precvbuffer;
os_event_t    user_procTaskQueue[user_procTaskQueueLen];
static void loop(os_event_t *events);
char sta_mac[6] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xab};

static int count = 0;
static bool wifiStatus = false;

static unsigned char lights = 0;

void user_rf_pre_init(void){
}

LOCAL void ICACHE_FLASH_ATTR
webserver_recv(void *arg, char *pusrdata, unsigned short length)
{
    char *pParseBuffer = NULL;
    bool parse_flag = false;
    struct espconn *ptrespconn = arg;

    os_printf("len:%u\n",length);
        
    if(length == 5){
       os_printf("%2X %2X %2X %2X %2X\n",
          pusrdata[0], pusrdata[1], pusrdata[2], 
          pusrdata[3], pusrdata[4]);
       if (pusrdata[0] == 0x19 && pusrdata[1] == 0x78 &&
           pusrdata[4] == (pusrdata[0] + pusrdata[1] + pusrdata[2] + 
          pusrdata[3])){
            os_printf("DATA Ok\n");
          switch (pusrdata[2]){
             case (0x00):
                break;
             case (0x10):
		lights = lights & ~0x01;
                break;
             case (0x20):
		lights = lights & ~0x02;
                break;
             case (0x30):
		lights = lights & ~0x04;
                break;
             case (0x11):
		lights = lights | 0x01;
                break;
             case (0x21):
		lights = lights | 0x02;
                break;
             case (0x31):
		lights = lights | 0x04;
                break;
             case (0xF0):
		lights = 0;
                break;
             case (0xF1):
		lights = 7;
                break;
             default:
		os_printf("Wrong command\n");
                break;

          } 
          switch (lights){
             case (0):
                gpio_output_set(0, BIT12|BIT13|BIT14, BIT12|BIT13|BIT14, 0);                
                break;
             case (1):
                gpio_output_set(BIT12, BIT13|BIT14, BIT13|BIT14, BIT12);                
                break;
             case (2):
                gpio_output_set(BIT13, BIT12|BIT14, BIT12|BIT14, BIT13);                
                break;
             case (3):
                gpio_output_set(BIT12|BIT13, BIT14, BIT14, BIT12|BIT13);                
                break;
             case (4):
                gpio_output_set(BIT14, BIT12|BIT13, BIT12|BIT13, BIT14);                
                break;
             case (5):
                gpio_output_set(BIT12|BIT14, BIT13, BIT13, BIT12|BIT14);                
                break;
             case (6):
                gpio_output_set(BIT13|BIT14, BIT12, BIT12, BIT13|BIT14);                
                break;
             case (7):
                gpio_output_set(BIT12|BIT13|BIT14, 0, 0, BIT12|BIT13|BIT14);                
                break;
             default:
                break;
          }
          espconn_sent(ptrespconn, &lights, 1);
       }
    }
}

LOCAL ICACHE_FLASH_ATTR
void webserver_recon(void *arg, sint8 err)
{
    struct espconn *pesp_conn = arg;

    os_printf("webserver's %d.%d.%d.%d:%d err %d reconnect\n", pesp_conn->proto.tcp->remote_ip[0],
    		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
    		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port, err);
}

LOCAL ICACHE_FLASH_ATTR
void webserver_discon(void *arg)
{
    struct espconn *pesp_conn = arg;

    os_printf("webserver's %d.%d.%d.%d:%d disconnect\n", pesp_conn->proto.tcp->remote_ip[0],
        		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
        		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port);
}

LOCAL void ICACHE_FLASH_ATTR
server_listen(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_recvcb(pesp_conn, webserver_recv);
    espconn_regist_reconcb(pesp_conn, webserver_recon);
    espconn_regist_disconcb(pesp_conn, webserver_discon);
}

void ICACHE_FLASH_ATTR
user_server_init(uint32 port)
{
    LOCAL struct espconn esp_conn;
    LOCAL esp_tcp esptcp;

    esp_conn.type = ESPCONN_TCP;
    esp_conn.state = ESPCONN_NONE;
    esp_conn.proto.tcp = &esptcp;
    esp_conn.proto.tcp->local_port = port;
    espconn_regist_connectcb(&esp_conn, server_listen);

    espconn_accept(&esp_conn);

}

//Main code function
static void ICACHE_FLASH_ATTR
loop(os_event_t *events)
{
int i;

    gpio16_output_set(0);



       if (wifi_station_connect()){
           os_printf("Not Connected to WiFi\n\r");
           wifiStatus = true;
       }        

 //   for(i=0;i<1000;i++)
       os_delay_us(1000);

 //   system_os_post(user_procTaskPrio, 0, 0 );
}

//Init function 
void ICACHE_FLASH_ATTR
user_init()
{
    char ssid[32] = "MY_SSID";
    char password[64] = "MY_PASS";
    struct station_config stationConf;
    
    os_printf("Init a\n\r");

    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    gpio16_output_conf();

    gpio16_output_set(0);

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
    
    gpio_output_set(0, BIT12|BIT13|BIT14, BIT12|BIT13|BIT14, 0);

    //Set station mode
    //wifi_set_macaddr(uint8 if_index, uint8 *macaddr)
    wifi_set_opmode_current( STATION_MODE );
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64);
    stationConf.bssid_set = 0;
    wifi_station_set_config_current(&stationConf);
//    wifi_status_led_install (16, uint32 gpio_name, FUNC_GPIO16)

    os_printf("Init Ok! %d\n\r", wifi_station_get_connect_status());


    wifi_station_set_auto_connect(1);

    wifi_station_connect();

    wifi_station_dhcpc_start();
    
    user_server_init(8888);

    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);

  //  system_os_post(user_procTaskPrio, 0, 0 );

}
