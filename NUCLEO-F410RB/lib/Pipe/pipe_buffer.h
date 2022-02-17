/******************************************************************************
 * @file pipe_com
 * @brief communication driver
 * @author Luos
 * @version 0.0.0
 ******************************************************************************/
#ifndef PIPE_BUFFER_H
#define PIPE_BUFFER_H

#include <stdint.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef PIPE_TO_LUOS_BUFFER_SIZE
#define PIPE_TO_LUOS_BUFFER_SIZE 1024
#endif
#ifndef LUOS_TO_PIPE_BUFFER_SIZE
#define LUOS_TO_PIPE_BUFFER_SIZE 2048
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Function
 ******************************************************************************/
void PipeBuffer_Init(void);
uint8_t *PipeBuffer_GetP2LBuffer(void);
uint8_t PipeBuffer_GetP2LTask(uint8_t **data, uint16_t *size);
void PipeBuffer_AllocP2LTask(uint16_t PositionLastData, uint8_t overflow);

uint8_t *PipeBuffer_GetL2PBuffer(void);
void PipeBuffer_PutL2PBuffer(uint8_t *data, uint16_t size);

#endif /* PIPE_BUFFER_H */
