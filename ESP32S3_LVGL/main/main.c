#include <stdio.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"  // 看门狗头文件
#include "lcd.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_demo_widgets.h"
#include "lv_demo_music.h"
#include "lv_demo_stress.h"
#include "lv_demo_benchmark.h"



static const char *TAG = "MAIN";

// 【1】超大堆栈 + 绑定CPU核心 + 最高优先级
#define TASK_LVGL_STACK      8192    // LVGL极限堆栈
#define TASK_LVGL_PRIORITY   4       // 最高优先级
TaskHandle_t lvgl_TaskHandle;

#define TASK2_STACK          4096  // 增加堆栈大小避免溢出
#define TASK2_PRIORITY       3
TaskHandle_t task2_TaskHandle;

#define TASK3_STACK          4096  // 增加堆栈大小避免溢出
#define TASK3_PRIORITY       2
TaskHandle_t task3_TaskHandle;




extern lv_disp_drv_t disp_drv; // 引用lv_port_disp.c中的静态变量
// ------------------- LVGL核心任务（最简初始化，不跑复杂Demo） -------------------
void lvgl_Task(void * pvParameters)
{
    
    lv_init();
    lv_port_disp_init();  // 只初始化显示驱动


	//lv_demo_widgets();
	//lv_demo_music();
	lv_demo_benchmark();
	//lv_demo_stress();




	

    while (1) {
        lv_timer_handler();   // LVGL循环

		
        UBaseType_t stack = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI("lvgl","任务1 最小剩余栈: %u 字, 堆空闲: %u 字节, 历史最小: %u 字节\r\n",
                  stack, xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());
		
        vTaskDelay(pdMS_TO_TICKS(10));  // 延时+自动喂狗
    }
}

// ------------------- 测试任务2 -------------------
void task2(void * pvParameters)
{
    while (1) {
        ESP_LOGI("task2", "运行中...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ------------------- 测试任务3 -------------------
void task3(void * pvParameters)
{
    while (1) {
        ESP_LOGI("task3", "运行中...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}





// ------------------- 主函数 -------------------
void app_main(void)
{
    ESP_LOGI(TAG, "=== 系统启动 ===");

    //创建LVGL任务 + 绑定CPU0（避免双核紊乱）
    xTaskCreatePinnedToCore(
        lvgl_Task, 
        "lvgl_Task", 
        TASK_LVGL_STACK, 
        NULL, 
        TASK_LVGL_PRIORITY, 
        &lvgl_TaskHandle,
        0  // 固定运行在CPU0
    );

    // 创建普通任务
    xTaskCreate(task2, "task2", TASK2_STACK, NULL, TASK2_PRIORITY, &task2_TaskHandle);
    xTaskCreate(task3, "task3", TASK3_STACK, NULL, TASK3_PRIORITY, &task3_TaskHandle);

    ESP_LOGI(TAG, "=== 所有任务创建完成 ===");
}