#ifndef BUTTON_H
#define BUTTON_H

#define Switch_0_Pressed 0b00000001
#define Switch_1_Pressed 0b00000010
#define Switch_2_Pressed 0b00000100
#define Switch_3_Pressed 0b00001000
#define Switch_4_Pressed 0b00010000
#define Switch_5_Pressed 0b00100000
#define Switch_6_Pressed 0b01000000
#define Switch_7_Pressed 0b10000000

extern unsigned char Trg; //button trigger sign
extern unsigned char Cont;//button continue sign

void key_configure();//inital key setting
//void key_deal();
#endif