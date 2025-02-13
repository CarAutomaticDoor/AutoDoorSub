#ifndef INCLUDES_APPLICATION_SIDE_DOOR_H_
#define INCLUDES_APPLICATION_SIDE_DOOR_H_

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "Platform_Types.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define     DOOR_OPEN        0
#define     DOOR_CLOSE       1
#define     DOOR_OPENING     2
#define     DOOR_CLOSING     3
#define     DOOR_STOP        4

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
void Control_Door(uint8 *state);

#endif /* INCLUDES_APPLICATION_SIDE_DOOR_H_ */
