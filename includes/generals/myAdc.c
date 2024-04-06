/**********************************************************************************************************************
 * \file ADC_Background_Scan.c
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
#include "myAdc.h"
#include "IfxVadc_Adc.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define CHANNELS_NUM    4                       /* Number of used channels                                          */

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void Init_Vadc_Module(void);                      /* Function to initialize the Vadc module with default parameters   */
void Init_Vadc_Group(void);                       /* Function to initialize the Vadc group                            */
void Init_Vadc_Channels(void);                    /* Function to initialize the Vadc used channels                    */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxVadc_Adc g_vadc;                                           /* Global variable for configuring the Vadc module    */
IfxVadc_Adc_Group g_vadc_group;                                /* Global variable for configuring the Vadc group     */
IfxVadc_Adc_Channel g_vadc_channel[CHANNELS_NUM];              /* Global variable for configuring the Vadc channels  */

/* Define the used channels */
IfxVadc_ChannelId g_vadc_channel_ids[] = {IfxVadc_ChannelId_4,  /* AN36: channel 4 of group 4                         */
                                          IfxVadc_ChannelId_5,  /* AN37: channel 5 of group 4                         */
                                          IfxVadc_ChannelId_6,  /* AN38: channel 6 of group 4                         */
                                          IfxVadc_ChannelId_7}; /* AN39: channel 7 of group 4                         */

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Function to initialize the Vadc module */
void Init_Vadc(void) {
    Init_Vadc_Module();                                                   /* Initialize the Vadc module               */
    Init_Vadc_Group();                                                    /* Initialize the Vadc group                */
    Init_Vadc_Channels();                                                 /* Initialize the used channels             */

    /* Start the scan */
    IfxVadc_Adc_startBackgroundScan(&g_vadc);
}

/* Function to initialize the Vadc module with default parameters */
void Init_Vadc_Module(void) {
    IfxVadc_Adc_Config adc_config;                                         /* Define a configuration structure         */
    IfxVadc_Adc_Init_Module_Config(&adc_config, &MODULE_VADC);               /* Fill it with default values              */
    IfxVadc_Adc_initModule(&g_vadc, &adc_config);                          /* Apply the configuration                  */
}

/* Function to initialize the Vadc group */
void Init_Vadc_Group(void) {
    IfxVadc_Adc_GroupConfig adc_group_config;                               /* Define a configuration structure         */
    IfxVadc_Adc_initGroupConfig(&adc_group_config, &g_vadc);                /* Fill it with default values              */

    adc_group_config.groupId = IfxVadc_GroupId_4;                           /* Select the Group 4                       */
    adc_group_config.master = adc_group_config.groupId;                         /* Set the same group as master group       */

    /* Enable the background scan source and the background auto scan functionality */
    adc_group_config.arbiter.requestSlotBackgroundScanEnabled = TRUE;
    adc_group_config.backgroundScanRequest.autoBackgroundScanEnabled = TRUE;

    /* Enable the gate in "always" mode (no edge detection) */
    adc_group_config.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

    IfxVadc_Adc_initGroup(&g_vadc_group, &adc_group_config);                 /* Apply the configuration                  */
}

/* Function to initialize the Vadc used channels */
void Init_Vadc_Channels(void) {
    IfxVadc_Adc_ChannelConfig adc_channel_config[CHANNELS_NUM];             /* Array of configuration structures        */

    size_t i;
    for(i = 0; i < CHANNELS_NUM; i++) {                            /* Initialize all the channels in a loop    */
        /* Fill the configuration with default values */
        IfxVadc_Adc_initChannelConfig(&adc_channel_config[i], &g_vadc_group);

        /* Set the channel ID and the corresponding result register */
        adc_channel_config[i].channelId = g_vadc_channel_ids[i];          /* The channels 4..7 are initialized        */
        adc_channel_config[i].resultRegister = (IfxVadc_ChannelResult)(i);
        adc_channel_config[i].backgroundChannel = TRUE;                   /* Enable background scan for the channel   */

        /* Apply the channel configuration */
        IfxVadc_Adc_initChannel(&g_vadc_channel[i], &adc_channel_config[i]);

        /* Add the channel to background scan */
        unsigned chn_enable_bit = (1 << adc_channel_config[i].channelId);   /* Set the the corresponding input channel  */
        unsigned mask = chn_enable_bit;                                   /* of the respective group to be added in   */
        IfxVadc_Adc_setBackgroundScan(&g_vadc, &g_vadc_group, chn_enable_bit, mask); /* the background scan sequence.  */
    }
}

/* Function to read the Vadc measurement */
uint16 Read_Vadc_Value(uint8 channel)
{
    Ifx_VADC_RES conversion_result;
    do {
        conversion_result = IfxVadc_Adc_getResult(&g_vadc_channel[channel]);
    } while(!conversion_result.B.VF);

    return conversion_result.B.RESULT;
}
