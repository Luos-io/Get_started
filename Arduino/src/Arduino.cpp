#include <Arduino.h>
#include "luos_engine.h"
#include "led.h"
#include "blinker.h"
#include "pipe.h"
#include "gate.h"
#include "robus_network.h"

/******************************************************************************
 * @brief Setup ardiuno
 * @param None
 * @return None
 ******************************************************************************/
void setup()
{
    Luos_Init();
    Robus_Init();
    Led_Init();
    Pipe_Init();
    Gate_Init();
    Blinker_Init();
}
/******************************************************************************
 * @brief Loop Arduino
 * @param None
 * @return None
 ******************************************************************************/
void loop()
{
    Luos_Loop();
    Robus_Loop();
    Led_Loop();
    Pipe_Loop();
    Gate_Loop();
    Blinker_Loop();
}
