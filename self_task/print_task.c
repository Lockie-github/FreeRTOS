#include "FreeRTOS.h"
#include "task.h"

#include "SEGGER_RTT.h"

#include "print_task.h"

static void fuckit(void  *pvParameters)
{

  for(;;)
  {
    myprintf("666\n");
    vTaskDelay(1000);

  }

}

void start_print_task(void)
{
    xTaskCreate(fuckit, "Start_led_yoggle", 100, NULL,1,NULL);
}