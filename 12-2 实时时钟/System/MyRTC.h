#ifndef __MYRTC_H
#define __MYRTC_H

extern uint16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime();
void MyRTC_ReadTime(void);

#endif