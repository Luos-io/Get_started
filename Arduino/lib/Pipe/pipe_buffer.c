/******************************************************************************
 * @file pipe_buffer
 * @brief communication buffer
 * @author Luos
 * @version 0.0.0
 ******************************************************************************/
#include <stdbool.h>
#include <string.h>
#include "luos_utils.h"

#include "pipe_com.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PIPE_TO_LUOS_MAX_TASK 10

typedef struct
{
    uint8_t *data_pt; /*!< Start pointer of the data on P2L_Buffer. */
    uint16_t size;    /*!< size of the data. */
    uint8_t *end;     /*!< Start pointer of the data on P2L_Buffer. */
} P2LTask_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t P2L_Buffer[PIPE_TO_LUOS_BUFFER_SIZE] = {0};
P2LTask_t P2LTask[PIPE_TO_LUOS_MAX_TASK]     = {0};
uint8_t P2LTaskID                            = 0;
volatile uint16_t P2LBuffer_PrevStartData    = 0;

uint8_t L2P_Buffer[LUOS_TO_PIPE_BUFFER_SIZE] = {0};
volatile uint8_t L2P_Buffer_WritePointer     = 0;

/*******************************************************************************
 * Function
 ******************************************************************************/
static uint8_t PipeBuffer_P2LTaskNeedClear(uint16_t PositionLastData);
static void PipeBuffer_ClearP2LTask(void);
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void PipeBuffer_Init(void)
{
    memset((void *)P2LTask, 0, sizeof(P2LTask));
    P2LBuffer_PrevStartData = 0;
    P2LTaskID               = 0;
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
uint8_t *PipeBuffer_GetP2LBuffer(void)
{
    return &P2L_Buffer[0];
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
uint8_t PipeBuffer_GetP2LTask(uint8_t **data, uint16_t *size)
{
    if (P2LTaskID != 0)
    {
        *data = P2LTask[0].data_pt;
        *size = P2LTask[0].size;
        PipeBuffer_ClearP2LTask();
        return true;
    }
    return false;
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void PipeBuffer_ClearP2LTask(void)
{
    uint8_t i = 0;
    for (i = 0; i < P2LTaskID; i++)
    {
        P2LTask[i].data_pt = P2LTask[i + 1].data_pt;
        P2LTask[i].size    = P2LTask[i + 1].size;
        P2LTask[i].end     = P2LTask[i + 1].end;
    }
    if (P2LTaskID != 0)
    {
        P2LTaskID--;
        P2LTask[P2LTaskID].data_pt = 0;
        P2LTask[P2LTaskID].size    = 0;
        P2LTask[P2LTaskID].end     = 0;
    }
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void PipeBuffer_AllocP2LTask(uint16_t size)
{
    LUOS_ASSERT(size < PIPE_TO_LUOS_BUFFER_SIZE);
    uint16_t PositionLastData = P2LBuffer_PrevStartData + (size - 1);
    while (PipeBuffer_P2LTaskNeedClear(PositionLastData) != false)
        ;
    LUOS_ASSERT(P2LTaskID < PIPE_TO_LUOS_MAX_TASK);
    for (uint8_t i = 0; i < PIPE_TO_LUOS_MAX_TASK; i++)
    {
        if (P2LTask[i].data_pt == 0)
        {
            P2LTask[i].data_pt = &P2L_Buffer[P2LBuffer_PrevStartData];
            P2LTask[i].end     = &P2L_Buffer[PositionLastData];
            P2LTask[i].size    = size;
            PositionLastData++;
            if (PositionLastData < PIPE_TO_LUOS_BUFFER_SIZE)
            {
                P2LBuffer_PrevStartData = PositionLastData;
            }
            else
            {
                P2LBuffer_PrevStartData = 0;
            }
            P2LTaskID++;
            return;
        }
    }
    // No more space
    LUOS_ASSERT(0);
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
static uint8_t PipeBuffer_P2LTaskNeedClear(uint16_t PositionLastData)
{
    uint8_t result = false;
    if (P2LTaskID > 0)
    {
        if (((&P2L_Buffer[PositionLastData] > (P2LTask[0].data_pt)) && (&P2L_Buffer[PositionLastData] < (P2LTask[0].end))) || ((&P2L_Buffer[PositionLastData] < (P2LTask[0].data_pt)) && (&P2L_Buffer[PositionLastData] > (P2LTask[0].end))))
        {
            result = true;
            PipeBuffer_ClearP2LTask();
        }
    }
    return result;
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
uint8_t *PipeBuffer_GetL2PBuffer(void)
{
    return &L2P_Buffer[0];
}
