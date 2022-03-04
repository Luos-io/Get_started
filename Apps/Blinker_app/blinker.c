#include "blinker.h"
#include "luos.h"

#define DEFAULTBLINKTIME_MS 500

// Create a new custom service type for this blink example
typedef enum
{
    BLINKER_APP = LUOS_LAST_TYPE
} App_type_t;

service_t *blinker_app; // This will be our blinker service
volatile time_luos_t blinktime;
unsigned long my_time; // Used to keep track of time
volatile control_t control_app;

static void Blinker_MsgHandler(service_t *service, msg_t *msg);

void Blinker_Init(void)
{
    revision_t revision = {.major = 1, .minor = 0, .build = 0};
    blinker_app         = Luos_CreateService(Blinker_MsgHandler, BLINKER_APP, "blinker", revision);

    // Detect all service of your network and create a routing_table
    Luos_Detect(blinker_app);

    // Set the default blink duration
    blinktime = TimeOD_TimeFrom_ms(DEFAULTBLINKTIME_MS);

    // Set our next timeout to now plus BLINKTIME milliseconds
    my_time = Luos_GetSystick() + (unsigned long)TimeOD_TimeTo_ms(blinktime);

    // Set the blinker to play
    control_app.flux = PLAY;
}

void Blinker_Loop(void)
{
    static uint8_t led_last_state = 0; // Is the LED on or off?

    // Check to see if we have overshot our counter
    if ((my_time < Luos_GetSystick()) && (control_app.flux == PLAY))
    {
        // Reset the counter
        // blinktime can be updated on Blinker_MsgHandler function.
        my_time = Luos_GetSystick() + (unsigned long)TimeOD_TimeTo_ms(blinktime);

        // Invert the LED state
        led_last_state = (led_last_state == 1 ? 0 : 1);

        // Now send a message
        msg_t led_msg;

        // Get the ID of our LED from the routing table
        search_result_t led_result;
        RTFilter_Alias(RTFilter_Reset(&led_result), "led");

        if (led_result.result_nbr > 0)
        {
            led_msg.header.target      = led_result.result_table[0]->id; // We are sending this to the LED
            led_msg.header.cmd         = IO_STATE;                       // We are specifying an IO state (on or off)
            led_msg.header.target_mode = IDACK;                          // We are asking for an acknowledgement

            led_msg.header.size = sizeof(char);   // Our message only contains one character, the IO state
            led_msg.data[0]     = led_last_state; // The I/O state of the LED to be sent
                                                  // (1 or 0, on or off)
            Luos_SendMsg(blinker_app,
                         &led_msg); // Now that we have the elements, send the message
        }
    }
}

static void Blinker_MsgHandler(service_t *service, msg_t *msg)
{
    if (msg->header.cmd == CONTROL)
    {
        ControlOD_ControlFromMsg((control_t *)&control_app, msg);
        return;
    }
    if (msg->header.cmd == TIME)
    {
        // Save transition time
        TimeOD_TimeFromMsg((time_luos_t *)&blinktime, msg);
        return;
    }
}