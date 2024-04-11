/**********************************************************************************************************************
 * \file Pin_Map.h
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

#ifndef INCLUDES_APPLICATION_PIN_MAP_H_
#define INCLUDES_APPLICATION_PIN_MAP_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxPort_PinMap.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define PIN_BTN_AUTO_LOCK               IfxPort_P33_0       // PIN40    XIO22   P33_0
#define PIN_BTN_DOOR_OPCL               IfxPort_P11_10      // PIN42    XIO24   P11_10
#define PIN_BTN_KIDS_LOCK               IfxPort_P33_3       // PIN44    XIO26   P33_3
#define PIN_BTN_EMERGENCY_STOP          IfxPort_P11_3       // PIN46

#define PIN_TOUCH_SENSOR                                    // AD0      ADCL1   SAR4_7  P32_3
#define PIN_FINGER_SENSOR                                   // AD1      ADCL2   SAR4_8  P32_4

#define PIN_FOOT_DETECTOR_TRIG          IfxPort_P00_0       // PIN25    XIO5    P00_0
#define PIN_FOOT_DETECTOR_ECHO          IfxPort_P00_1       // PIN27    XIO7    P00_1

#define PIN_OBSTACLE_DETECTOR_TRIG      IfxPort_P00_3       // PIN31    XIO11   P00_3
#define PIN_OBSTACLE_DETECTOR_ECHO      IfxPort_P00_4       // PIN33    XIO13   P00_4

#define PIN_DOOR_CTRL                                       // PWM8     PWMH1   P02_6
#define PIN_LOCK_CTRL                                       // PWM9     PWMH2   P02_7

#define PIN_AUDIO                                           // PWM10    PWMH3   P10_5

#define PIN_AMBIENT_1                                       // PIN39    XIO19   P00_7
#define PIN_AMBIENT_2                                       // PIN41    XIO21   P11_9
#define PIN_AMBIENT_3                                       // PIN43    XIO23   P11_11

#define INPUT_MODE              IfxPort_Mode_inputPullDown
#define OUTPUT_MODE             IfxPort_Mode_outputPushPullGeneral

#define HIGH                    IfxPort_State_high
#define LOW                     IfxPort_State_low

#define PIN_MODE(pin, mode)     IfxPort_setPinMode(pin.port, pin.pinIndex, mode)
#define SET_PIN(pin, state)     IfxPort_setPinState(pin.port, pin.pinIndex, state)
#define TOGGLE_PIN(pin)         IfxPort_togglePin(pin.port, pin.pinIndex)
#define CMP_PINS(pin1, pin2)            (((pin1.port) == (pin2.port) && (pin1.pinIndex == pin2.pinIndex)))
/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*-------------------------------------------------Data Structures---------------------------------------------------*/
/*********************************************************************************************************************/
 
/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/


#endif /* INCLUDES_APPLICATION_PIN_MAP_H_ */
