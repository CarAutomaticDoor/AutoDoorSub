/**********************************************************************************************************************
 * \file In_Module_Switch_Door.c
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
#include "IfxPort_PinMap.h"
#include "IfxPort.h"
#include "IfxStm.h"
#include "XIO.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/



/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
uint8 doNum;
uint8 pre_InDoorOpen_SW = 1;
uint8 temp_InDoorOpen_SW;
 IfxPort_Pin x_IO_DO[MAX_XIO_NUMBER] = {XIO22, XIO23, XIO24, XIO25, XIO26, XIO27, XIO28, XIO29, XIO30, XIO31,
         XIO32, XIO33, XIO34, XIO35, XIO36, XIO37, XIO38, XIO39, XIO40, XIO41,
         XIO42, XIO43, XIO44, XIO45, XIO46, XIO47, XIO48, XIO49, XIO50, XIO51};
/*********************************************************************************************************************/
/*--------------------------------------------Private Variables/Constants--------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

void In_Module_Switch_Door_Open_init(uint8 btnNumber){ /*btnNumber : Close btn 연결한 보드 핀 번호(XIO22~51)   */
    doNum = btnNumber-22;
    IfxPort_setPinModeInput(x_IO_DO[btnNumber-22].port, x_IO_DO[btnNumber-22].pinIndex, IfxPort_Mode_inputPullUp);
/* ex) IfxPort_setPinModeInput(IfxPort_P00_0.port, IfxPort_P00_0.pinIndex, IfxPort_Mode_inputPullUp);  */
    return;
}

uint8 In_Module_Switch_Door_Open_return(){   /* btn(switch)눌리면 1 return  */
    temp_InDoorOpen_SW = (uint8)IfxPort_getPinState(x_IO_DO[doNum].port, x_IO_DO[doNum].pinIndex); /*Close Btn(Switch) 값 읽어와 tempSW에 저장  */
    if(pre_InDoorOpen_SW & !temp_InDoorOpen_SW){return 1;}    /* temp는 눌릴때 0 (pull up)*/
    else{return 0;}
}

