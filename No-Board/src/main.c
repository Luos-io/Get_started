/******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************/
#include "blinker.h"
#include "gate.h"
#include "led.h"
#include "luos_engine.h"
#include "pipe.h"

/*****************************************
 * @brief  The application entry point.
 * @retval int 
*****************************************/
int main(void)
{
    Luos_Init();
    Led_Init();
    Pipe_Init();
    Gate_Init();
    Blinker_Init();

    while (1)
    {
        Luos_Loop();
        Led_Loop();
        Pipe_Loop();
        Gate_Loop();
        Blinker_Loop();
    }
}

/*****************************************************************
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 *****************************************************************/
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/*****************************************************************************
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 *****************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
