/**********************************************************************************************************************
 * \file Foot_Sensor.c
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
#include "Foot_Sensor.h"

#include "Ultrasonic.h"
#include "Pin_Map.h"

#include "Platform_Types.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define TIME_OUT    0.00001f
#define THRESHOLD   20.0f

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
Ultrasonic g_foot_config = {.trig = {&MODULE_P00, 0}, .echo = {&MODULE_P00, 1}} ;
float32 g_foot_distance = 0.0f;

/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void Init_Foot_Sensor(){
    // 초기 설정. trig, echo pin 입출력 세팅.
    Init_Ultrasonic(&g_foot_config);
}

float32 Read_Foot_Distance(void) {
    // read ultrasonic distance

    // 여러번 디버거를 찍어보니 TimeOut 일 때, == 0 이 되는 경우가 있었다.
    // 따라서, 이후 초음파 센서 값으로 코딩을 할때 조건문을 => if(3 <= distance <= 10s)와 같이 작성한다.
    g_foot_distance = Get_Ultrasonic_Distance(&g_foot_config);

    return g_foot_distance;
}

// return TRUE if obstacle detected
boolean Read_Foot_Detection_State() {

    /*
     * 초기값은 return : FALSE  (감지 X), 감지가 된다면 return : TRUE (감지 O)
     * 매개변수 threshold_dist : 임계값 거리
     */

    // float32 foot_distance = Get_Ultrasonic_Distance(&g_Foot_Config);
    g_foot_distance = Get_Ultrasonic_Distance(&g_foot_config);

    return ((TIME_OUT < g_foot_distance) && (g_foot_distance <= THRESHOLD));
}
