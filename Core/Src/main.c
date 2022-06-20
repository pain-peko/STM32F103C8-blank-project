/* ---------- Includes ---------- */
#include "main.h"

/* ---------- Macros ---------- */
#define NICE_TASK_PRIORITY    		( tskIDLE_PRIORITY + 1 )

/* ---------- Declaration ---------- */
void SystemClock_Config(void);
void initGPIO();
void prvNiceTask(void* pvParameters);

/* ---------- Functions ---------- */
int main(void)
{
	SystemClock_Config();

	initGPIO();

	xTaskCreate(prvNiceTask,             		/* The function that implements the task. */
			    "Nice_Task",                  	/* The text name assigned to the task - for debug only as it is not used by the kernel. */
			    configMINIMAL_STACK_SIZE,     	/* The size of the stack to allocate to the task. */
			    NULL,                         	/* The parameter passed to the task - not used in this case. */
			    NICE_TASK_PRIORITY, 			/* The priority assigned to the task. */
			    NULL);                        	/* The task handle is not required, so NULL is passed. */

	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	 * line will never be reached.  If the following line does execute, then
	 * there was insufficient FreeRTOS heap memory available for the Idle and/or
	 * timer tasks to be created.  See the memory management section on the
	 * FreeRTOS web site for more details on the FreeRTOS heap
	 * http://www.freertos.org/a00111.html. */
	for(;;);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 8000000
  *            PLL_MUL                        = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	/* Enable HSI oscillator */
	LL_RCC_HSI_Enable();
	while(LL_RCC_HSI_IsReady() != 1)
	{
	};

	/* Set FLASH latency */
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	/* Main PLL configuration and activation */
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_9);
	LL_RCC_PLL_Enable();
	while(LL_RCC_PLL_IsReady() != 1)
	{
	};

	/* Sysclk activation on the main PLL */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	};

	/* Set APB1 & APB2 prescaler */
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	/* Set systick to 1ms */
	SysTick_Config(72000000 / 1000);

	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	SystemCoreClock = 72000000;
}

void initGPIO()
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void prvNiceTask(void* pvParameters)
{
	for(;;)
	{
		LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_13);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}
