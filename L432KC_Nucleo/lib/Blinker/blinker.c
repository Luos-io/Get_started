#include "main.h"
#include "blinker.h"
#include "string.h"
#include "luos.h"
#include <routing_table.h>

#define REV     \
	{           \
		1, 0, 0 \
	}

service_t *blinker_app;

typedef enum
{
	BLINKER_APP = LUOS_LAST_TYPE
} App_type_t;

uint16_t blinktime = 500;

int8_t id_led;
unsigned long my_time;		//Used to keep track of time
uint8_t led_last_state = 0; //Is the LED on or off?

static void Blinker_MsgHandler(service_t *service, msg_t *msg);

void Blinker_Init(void)
{
	revision_t revision = {.unmap = REV};
	blinker_app = Luos_CreateService(Blinker_MsgHandler, BLINKER_APP, "blinker_old", revision);

	//Detect all service of your network and create a routing_table
	RoutingTB_DetectServices(blinker_app);

	//Set our next timeout to now plus BLINKTIME milliseconds
	my_time = HAL_GetTick() + blinktime;
}

void Blinker_Loop(void)
{
	//Check to see if we have overshot our counter
	if (my_time < HAL_GetTick())
	{
		//Reset the counter
		my_time = HAL_GetTick() + blinktime;

		//Invert the LED state
		led_last_state = (led_last_state == 1 ? 0 : 1);

		//Now send a message
		msg_t led_msg;

		//Get the ID of our LED from the routing table
		id_led = RoutingTB_IDFromAlias("led_mod");

		led_msg.header.target = id_led;		//We are sending this to the LED
		led_msg.header.cmd = IO_STATE;		//We are specifying an IO state (on or off)
		led_msg.header.target_mode = IDACK; //We are asking for an acknowledgement

		led_msg.header.size = sizeof(char);	 //Our message only contains one character, the IO state
		led_msg.data[0] = led_last_state;	 //The I/O state of the LED to be sent (1 or 0, on or off)
		Luos_SendMsg(blinker_app, &led_msg); //Now that we have the elements, send the message
	}
}

static void Blinker_MsgHandler(service_t *service, msg_t *msg)
{
}