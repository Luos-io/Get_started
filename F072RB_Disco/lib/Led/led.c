#include "main.h"
#include "led.h"
#include "string.h"
#include "luos.h"

#define REV     \
	{           \
		1, 0, 0 \
	}

static void Led_MsgHandler(service_t *service, msg_t *msg);

void Led_Init(void)
{
	revision_t revision = {.unmap = REV};
	Luos_CreateService(Led_MsgHandler, STATE_TYPE, "Led", revision);
}

void Led_Loop(void)
{
}

static void Led_MsgHandler(service_t *service, msg_t *msg)
{
	if (msg->header.cmd == IO_STATE)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, msg->data[0]);
		return;
	}
}
