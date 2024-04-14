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
#include "IfxAsclin_reg.h"

#include "Uart.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define IDX_AUTO_LOCK       0       // 오토락 버튼 상태 받아오기
#define IDX_DOOR_OPCL       1       // 차량문 열고 닫힘 버튼 상태 받아오기
#define IDX_DOOR_LOCK       2       // 차량 문잠금(키즈락) 버튼 상태 받아오기
#define IDX_EMERGENCY       3       // 비상 정지 버튼 상태 받아오기

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
IfxPort_Pin g_tmp[4] = { {&MODULE_P33, 0}, {&MODULE_P11, 10}, {&MODULE_P33, 3}, {&MODULE_P11, 3}};
boolean g_btns[4];
boolean g_prevv[4] = { TRUE, TRUE, TRUE, TRUE };
boolean g_state_change = FALSE;

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
    PIN_MODE(PIN_LED, OUTPUT_MODE);
    SET_PIN(PIN_LED, LOW);

    Init_Gtm();
    Init_Servos();  //이게 없어야 uart 돌아감
    Init_Vadc();
    Init_Finger_Detector();
    Init_Touch_Sensor();
    Init_Buttons();
    Init_Foot_Sensor();
    Init_Obstacle_Sensor();
    Init_Uart();
    Init_Light();
    Start_Adc_Scan();
}

void Auto_Door_Start() {
    while(1) {
        MODULE_ASCLIN0.RXDATA.U;
        Sensors();
        Change_State();
        if (g_state_change == TRUE) {
            g_state_change = FALSE;
            continue;
        }
        Actuators();
    }
}

void Sensors(void) {
    // 터치센서
    if ((g_auto_lock == FALSE) && (g_door == DOOR_CLOSE)) {
        g_touch = Read_Touch_State();
    } else {
        g_touch = FALSE;
    }

    // 손끼임 센서
    if ((g_door == DOOR_CLOSING) || (g_door == DOOR_CLOSE)) {
        g_finger = Read_Finger_State();
    } else {
        g_finger = FALSE;
    }

    // 4개의 버튼
    boolean tmp;
    for (int i=0;i<4;i++) {
        tmp = GET_PIN(g_tmp[i]);
        g_btns[i] = (!tmp && g_prevv[i]);
        g_prevv[i] = tmp;
    }

    // 발 센서
    if ((g_door == DOOR_CLOSE) && (g_auto_lock == FALSE)) {
        g_foot = Read_Foot_Detection_State();
    } else {
        g_foot = FALSE;
    }

    // 장애물 센서
    if ((g_door == DOOR_OPENING)) {
        g_obstacle = Read_Obstacle_Detection_State();
    } else {
        g_obstacle = FALSE;
    }

    Delay_Ms(10);
}

void Change_State(void)
{
    // 센서에서 읽은 정보 바탕으로 문의 동작 상태 천이
    Change_Door_State();

    // 센서에서 읽은 정보 바탕으로 자동문 기능 상태 천이
    Change_Auto_Lock_State();

    // 센서에서 읽은 정보 바탕으로 잠금 장치 상태 천이
    Change_Door_Lock_State();
}


void Change_Auto_Lock_State(void) {
    // 문이 열려있으면 잠금기능 변경 불가
    if (g_door != DOOR_CLOSE) {
        return;
    }

    // 버튼, 블루투스로 잠금 해제, 또는 잠금으로 천이
    if((g_btns[IDX_AUTO_LOCK] == TRUE) || (g_rx_data == UART_LOCK)) {
        g_auto_lock = (g_auto_lock == TRUE) ? FALSE : TRUE;
        g_rx_data = UART_NOINPUT;
    }
}

void Change_Door_Lock_State(void) {
    // 문이 열려있으면 잠금장치 변경 불가
    if (g_door != DOOR_CLOSE) {
        return;
    }

    switch (g_door_lock) {
    // 버튼, 블루투스로 잠금에서 잠금 해제로 천이
    case LOCK:
        if ((g_btns[IDX_DOOR_LOCK] == TRUE) || (g_auto_lock == FALSE)) {
            g_door_lock = UNLOCK;
        }
        break;
    // 버튼으로 잠금해제에서 잠금으로 천이
    // 문을 잠그면 자동문 기능 잠금
    case UNLOCK:
        if (g_btns[IDX_DOOR_LOCK] == TRUE) {
            g_auto_lock = TRUE;
            g_door_lock = LOCK;
        }
        break;
    default:
        break;
    }
}

void Change_Door_State(void) {
    // 자동문 기능 가능하고, 문이 잠겨있지 않으면 문 동작 변경 가능
    if (g_auto_lock == TRUE) {
        return;
    }

    if (g_door_lock != UNLOCK) {
        return;
    }

    switch (g_door) {
    // 완전히 열린 문은 버튼, 혹은 블루투스 명령으로 닫을 수 있음
    case DOOR_OPEN:
        if((g_rx_data == UART_CLOSE) || (g_btns[IDX_DOOR_OPCL] == TRUE)){
            g_door = DOOR_CLOSING;
            g_state_change = TRUE;
            g_rx_data = UART_NOINPUT;
        }
        break;

    // 완전히 닫힌 문은 자동문 기능 사용 가능하고, 잠금 장치가 해제되었을 때,
    // 버튼, 블루투스, 터치 센서, 발 센서로 열 수 있음
    case DOOR_CLOSE:
        if((g_auto_lock == FALSE) && (g_door_lock == UNLOCK) &&
          ((g_rx_data == UART_OPEN) ||( g_btns[IDX_DOOR_OPCL] == TRUE) || (g_touch == TRUE) || (g_foot == TRUE))) {
            g_door = DOOR_OPENING;
            g_state_change = TRUE;
            g_rx_data = UART_NOINPUT;
        }
        break;
    // 열고 있는 중에는 장애물이 있거나 긴급 정지 버튼으로 문을 정지시킴
    case DOOR_OPENING:
        if ((g_obstacle == TRUE) || (g_btns[IDX_EMERGENCY] == TRUE)) {
            g_door = DOOR_STOP;
            g_state_change = TRUE;
        }
        break;
    case DOOR_CLOSING:
        // 닫고있는 중에 긴급 정지 버튼을 누르면 문을 정지시킴
         if (g_btns[IDX_EMERGENCY] == TRUE) {
             g_door = DOOR_STOP;
             g_state_change = TRUE;
         }
         // 닫고있는 중에 손가락이 끼이면 다시 문을 염
         else if (g_finger == TRUE) {
             g_door = DOOR_OPENING;
             g_state_change = TRUE;
         }
        break;
        // 정지상태에서 블루투스로 열거나 닫을 수 있고, 버튼으로는 닫을 수 있음
    case DOOR_STOP:
        if(g_rx_data == UART_OPEN){
            g_door = DOOR_OPENING;
            g_state_change = TRUE;
            g_rx_data = UART_NOINPUT;
        }
        if((g_rx_data == UART_CLOSE) || g_btns[IDX_DOOR_OPCL]){
            g_door = DOOR_CLOSING;
            g_state_change = TRUE;
            g_rx_data = UART_NOINPUT;
        }
        break;
    default:
        break;
    }
}

void Actuators(void) {
//     상태정보에 따라 잠금 장치를 동작시킴
    Control_Lock(&g_door_lock);

    // 상태정보에 따라 문을 동작시킴
    Control_Door(&g_door);
    Control_Light(g_auto_lock);
//    Control_Audio();
}
