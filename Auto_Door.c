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
#include "IfxPort.h"
#include "IfxPort_PinMap.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
uint8 g_auto_lock = TRUE;
uint8 g_door_lock = LOCK;
uint8 g_door      = DOOR_CLOSE;

boolean g_touch    = FALSE;
boolean g_finger   = FALSE;
boolean g_foot     = FALSE;
boolean g_obstacle = FALSE;
/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void Setup(void);
void Auto_Door_Start(void);
void Sensors(void);
void Change_State(void);
void Actuators(void);
void Change_Door_State(void);
void Change_Auto_Lock_State(void);
void Change_Door_Lock_State(void);

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void Auto_Door(void) {
    Setup();
    Auto_Door_Start();
}

void Setup(void) {
//    PIN_MODE(IfxPort_P10_2, OUTPUT_MODE);
    PIN_MODE(IfxPort_P00_4, INPUT_MODE);
    Init_Gtm();
    initGtmTom();
    Init_Vadc();
    Init_Finger_Detector();
    Init_Touch_Sensor();
//    Init_Buttons();
//
    Init_Foot_Sensor();
    Init_Obstacle_Sensor();
//    Init_Uart();
    Start_Adc_Scan();
}

void Auto_Door_Start() {
    while(1) {
        Sensors();
        Change_State();
        Actuators();
    }
}

void Sensors(void) {
    if ((g_auto_lock == FALSE) && (g_door == DOOR_CLOSE)) {
        g_touch = Read_Touch_Sensor();
    } else {
        g_touch = FALSE;
    }

    // g_door == DOOR_CLOSE일 때?
    if ((g_door == DOOR_CLOSING)) {
        g_finger = Read_Finger_Detector();
    } else {
        g_finger = FALSE;
    }

//    Read_Buttons();

    Delay_Ms(10);

    if ((g_door == DOOR_CLOSE) && (g_auto_lock == FALSE)) {
        g_foot = Read_Foot_Detection_State();
    } else {
        g_foot = FALSE;
    }

    // g_door == DOOR_CLOSE 또는 g_door == DOOR_OPEN일 때?
    if ((g_door == DOOR_OPENING)) {
        g_obstacle = Read_Obstacle_Detection_State();
    } else {
        g_obstacle = FALSE;
    }

    Delay_Ms(10);
}

void Change_State(void) {
    Change_Door_State();
    Change_Auto_Lock_State();
    Change_Door_Lock_State();
}


void Change_Auto_Lock_State(void) {
    if (g_door != DOOR_CLOSE) {
        return;
    }

    // if (버튼 기능 잠금 해제) || (uart 기능 잠금 해제)
    {g_auto_lock = (g_auto_lock == TRUE) ? FALSE : TRUE;}
}

void Change_Door_Lock_State(void) {
    if (g_door != DOOR_CLOSE) {
        return;
    }

    switch (g_door_lock) {
    case LOCK:
        // if (버튼 잠금 해제) || g_auto_lock == FALSE)
        {g_door_lock = UNLOCKING;}
        break;
    case UNLOCK:
        // if (버튼 잠금)
        {g_door_lock = LOCKING;}
        break;
    case LOCKING:
    case UNLOCKING:
    default:
        break;
    }
}

void Change_Door_State(void) {
    if (g_auto_lock == TRUE) {
        return;
    }

    if (g_door_lock != UNLOCK) {
        return;
    }

    switch (g_door) {
        case DOOR_OPEN:
            // if (uart 닫기 || 버튼 닫기)
            {g_door = DOOR_CLOSING;}
            break;
        case DOOR_CLOSE:
            // 문이 잠긴 상황에서 열기 버튼을 누르면 자동 잠금 해제 후 열기?
            // if (uart 열기 || 버튼 열기) && 기능 가능 && 문 잠기지 않음
            {g_door = DOOR_OPENING;}
            break;
        case DOOR_OPENING:
            // if (장애물 || 버튼 멈춤)
            {g_door = DOOR_STOP;}
            // else if (끝까지 도달)
            {g_door = DOOR_OPEN;}
            break;
        case DOOR_CLOSING:
            // if (손가락)
            {g_door = DOOR_OPENING;}
            // else if (버튼 멈춤)
            {g_door = DOOR_STOP;}
            // else if (끝까지 도달)
            {g_door = DOOR_CLOSE;}
            break;
        case DOOR_STOP:
            // 문 버튼으로 닫고 싶은 경우?
            // if (문 버튼 || uart 열기)
            {g_door = DOOR_OPENING;}
            // else if (uart 닫기)
            {g_door = DOOR_CLOSING;}
            break;
        default:
            break;
        }
}

void Actuators(void) {
    Control_Door_Lock(&g_door_lock);

    if (g_auto_lock == FALSE && g_door_lock == UNLOCK) {
        Control_Door(&g_door);
    }
//    Control_Audio();
}
