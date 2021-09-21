/******************************************************************************
 * @file pipe_com
 * @brief communication driver
 * @author Luos
 * @version 0.0.0
 ******************************************************************************/

#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include "pipe_com.h"
#include "luos_utils.h"

#ifdef __cplusplus
}
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t is_sending       = false;
volatile uint16_t size_to_send    = 0;
volatile uint16_t size_to_receive = 0;
/*******************************************************************************
 * Function
 ******************************************************************************/

/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void PipeCom_Init(void)
{
    Serial.begin(1000000);
    PipeBuffer_Init();
}

/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
volatile uint8_t PipeCom_SendL2PPending(void)
{
    return is_sending;
}
/******************************************************************************
 * @brief init must be call in project init
 * @param None
 * @return None
 ******************************************************************************/
void PipeCom_ReceiveP2L(void)
{
    uint8_t data = 0;
    while (Serial.available() > 0)
    {
        data = Serial.read();
        Stream_PutSample(get_P2L_StreamChannel(), &data, 1);
        size_to_receive++;
        if (data == '\r')
        {
            PipeBuffer_AllocP2LTask(size_to_receive);
            size_to_receive = 0;
        }
    }
}
/******************************************************************************
 * @brief PipeCom_SendL2P
 * @param None
 * @return None
 ******************************************************************************/
void PipeCom_SendL2P(uint8_t *data, uint16_t size)
{
    is_sending     = true;
    size_to_send   = size;
    char character = 0;
    while (size_to_send != 0)
    {
        character = *data;
        Serial.write(&character, 1);
        data++;
        size_to_send--;
    }
    is_sending = false;
}
