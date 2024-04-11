/**********************************************************************************************************************
 * \file Auto_Door.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/


/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Auto_Door.h"

#include "Headers.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
uint8 Audio_Mode = 0;
boolean Switch_Lock_Unlock_Flag = FALSE;
boolean Switch_Open_Close_Flag = FALSE;

Ultrasonic g_Ultra_Door = {.trig = {&MODULE_P00, 0}, .echo = {&MODULE_P00, 1}};
Ultrasonic g_Ultra_Foot = {.trig = {&MODULE_P00, 5}, .echo = {&MODULE_P00, 6}};

//boolean Switch_Kidslock_Flag = FALSE;

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/
uint64 Time_Diff = 0;
uint64 Time_Limit = 0;
uint64 Time_Start = 0;

float32 Foot_Distance = 0.0f;
float32 Door_Distance = 0.0f;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void Setup(void);
void Auto_Door_Start(void);
void Sensors(void);
void Change_State(void);
void Actuators(void);

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void Auto_Door(void) {
    Time_Diff = Get_Cur_Millis(); // 처음 시작하고 시간 저장.

    Setup();
    Auto_Door_Start();
}

void Setup(void) {
    Init_Gtm();
    Init_Vadc();
    Init_Finger_Detector();
    Init_Touch_Sensor();
    Start_Adc_Scan();
    Init_Buttons();
    Init_Side_Door();
    Init_Foot_Sensor();

    Init_Audio();
    Init_Side_Door();
    Init_Ultrasonic(&g_Ultra_Foot);
    Init_Ultrasonic(&g_Ultra_Door);
}

void Auto_Door_Start() {
    while(1) {
//        Sensors();
//        Change_State();
//        Actuators();

        // 초음파 센서 거리.
        Foot_Distance = Get_Ultrasonic_Distance(&g_Ultra_Foot);
        Door_Distance = Get_Ultrasonic_Distance(&g_Ultra_Door);

        // pin_num : 3가지(잠금, Open/Close, 키즈락) 중 읽고자하는 값의 입력핀 -> Pin_Map.h 에서 확인.
        Switch_Lock_Unlock_Flag = Get_Button_State(PIN_BTN_AUTO_LOCK);
        Switch_Open_Close_Flag = Get_Button_State(PIN_BTN_DOOR_OPCL);


        if(Switch_Open_Close_Flag && (Door_Distance == 0 || Door_Distance > 10.0)){
            Time_Limit = 1000;
            Audio_Mode = OPEN_CLOSE_SOUND;
            Time_Start = Get_Cur_Millis();
        }
        else if(Switch_Lock_Unlock_Flag){
            Time_Limit = 500;
            Audio_Mode = WELCOME_SOUND;
            Time_Start = Get_Cur_Millis();
        }


        // 시간 초기화 문제 이후에 고래해주어야 함. (Get_Cur_Millis() - Time_Diff < 0  오버플로우 되서 음수가 되면 일단 그냥 초기화.. <- 수정 필요.
        if((Get_Cur_Millis() - Time_Diff > Time_Limit) || (Get_Cur_Millis() - Time_Diff < 0)){
            Time_Diff = Get_Cur_Millis();
            if(1 && (Get_Cur_Millis() - Time_Start < 5000)){  // if 조건은 :  UNLOCK 상태이면서 &&  (손 끼임 || 문 열기 || 닫기)
                Play_Audio_Case_Of_Situation(Audio_Mode); // (1) : 파라미터 정해야함. 정하는 IF~ELSE 필요.
                /*
                   #define DANGER_SOUND 1       :   손 끼임 소리 출력
                   #define OPEN_CLOSE_SOUND 2   :   문 열고 닫힐 때, 소리 출력
                   #define WELCOME_SOUND 3      :   웰컴 사운드 출력.
                */
            }
            else{
                Audio_Mode = NO_SOUND;
                Play_Audio_Case_Of_Situation(Audio_Mode);
                Time_Start = 0;
            }
        }

    }
}

void Sensors(void) {

}

void Change_State(void) {

}

void Actuators(void) {

}
