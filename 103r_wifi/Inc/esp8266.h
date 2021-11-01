#ifndef esp8266_h
#define esp8266_h
#include "main.h"

void esp8266_init(void);
u8 esp8266_send_cmd(u8* cmd, u8* ack, u8 waittime);
u8 atk_8266_check_cmd(u8 *str);
void ack_determine(char* strings);
void esp8266_send_data(u8* data);
u8 atk_8266_quit_trans(void);

#endif
