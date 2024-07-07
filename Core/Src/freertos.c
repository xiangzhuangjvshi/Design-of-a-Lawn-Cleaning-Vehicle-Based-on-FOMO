#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>

#include "ai_platform.h"  // 包含AI平台相关的头文件
#include "network.h"
#include "network_data.h"
#include <stdio.h>
#include "usart.h"
#include "stm32746g_discovery_camera.h"
#include "stm32746g_discovery_lcd.h"

#include <compute_crc.h>
#include <math.h> // 包含数学库头文件
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// 定义相机内参矩阵
typedef struct {
    double fx, fy, cx, cy;
} CameraIntrinsics;

// 如果没有定义 PI，则定义 PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double calculate_angle_with_yoz_plane(CameraIntrinsics intrinsics, int pixel_x, int pixel_y);
CameraIntrinsics intrinsics = { 0.001087, 0.001081, 250.9719 , 97.9172};

int recognition_flag1;
int target_x, target_y;
void post_process(float* out_data, int* recognition_flag, int* target_x, int* target_y);

/*绘制代码*/
void Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void Draw_Text(uint16_t x, uint16_t y, uint8_t* text, uint32_t color);
void Draw_Hollow_Square(uint16_t x, uint16_t y, uint16_t side_length, uint32_t color);

osSemaphoreId_t imageReadySemaphore;
uint8_t bufferReady = 0;


ai_handle network;  // 神经网络的句柄定义

ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];  // 第一个神经网络的激活数据
ai_buffer* ai_input;  // AI输入数据缓冲区
ai_buffer* ai_output;  // AI输出数据缓冲区
float in_data[AI_NETWORK_IN_1_SIZE];  // 第一个神经网络的输入数据
float out_data[AI_NETWORK_OUT_1_SIZE];  // 第一个神经网络的输出数据


#define GRID_SIZE 7
#define NUM_CLASSES 2

// 打印第二个神经网络的输出数据
void print_nn_output(float out_data[]) {
    printf("Neural Network Output:\n");
    int index = 0;
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("Grid (%d, %d): Prob1 = %f, Prob2 = %f\n", i, j, out_data[index], out_data[index + 1]);
            index += NUM_CLASSES;
        }
    }
}


// Function prototypes
void prepare_yolo_data(float* in_data);
void argb8888_to_gray_and_resize(uint32_t* argb, float* resized, int src_width, int src_height, int dst_width, int dst_height);

void AI_Init(void);
void AI_Run(float* pIn, float* pOut);



void Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
void Draw_Text(uint16_t x, uint16_t y, uint8_t *text, uint32_t color);


// 已经拟合好的参数
float popt[] = {-1.82204070e-07, 1.96926650e-05, 4.73758907e-04, -1.22931100e-01, 2.26574694e+01, 2.65264304e+02};
// 计算延迟时间的函数
float calculate_delay_time(float angle_deviation) {
    float abs_angle_deviation = fabs(angle_deviation);
    float delay_sec = (popt[0] * pow(abs_angle_deviation, 5) + 
                      popt[1] * pow(abs_angle_deviation, 4) + 
                      popt[2] * pow(abs_angle_deviation, 3) + 
                      popt[3] * pow(abs_angle_deviation, 2) + 
                      popt[4] * abs_angle_deviation + 
                      popt[5]);
    //float delay_sec = delay_ms / 1000; // 将毫秒转换为秒
    return delay_sec;
}

/*定义互斥锁*/
osMutexId_t recognitionMutex;
const osMutexAttr_t recognitionMutex_attr = {
    "recognitionMutex",   // 锁的名称
    osMutexRecursive,     // 递归锁
    NULL,                 // 锁内存分配
    0                     // 锁大小
};

osThreadId_t Identify_TaskHandle;
const osThreadAttr_t Identify_Task_attributes = {
  .name = "Identify_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Control_tasks */
osThreadId_t Control_tasksHandle;
const osThreadAttr_t Control_tasks_attributes = {
  .name = "Control_tasks",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Display_Task */
osThreadId_t Display_TaskHandle;
const osThreadAttr_t Display_Task_attributes = {
  .name = "Display_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for DataQueue */
osMessageQueueId_t DataQueueHandle;
const osMessageQueueAttr_t DataQueue_attributes = {
  .name = "DataQueue"
};

osSemaphoreId_t imageReadySemaphore;
const osSemaphoreAttr_t imageReadySemaphore_attributes = {
  .name = "imageReadySemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
typedef struct {
    double angle;
    int recognition; // 识别到目标的标志位，1为识别到目标，0为没有识别到目标
    char direction; 
		int Target_X;
		int Target_Y;
} DataPacket_t;
/* USER CODE END FunctionPrototypes */

void Identify_Task_entry(void *argument);
void Control_tasks_entry(void *argument);
void Display_Task_entry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {

  DataQueueHandle = osMessageQueueNew (5, sizeof(DataPacket_t), &DataQueue_attributes);

  Identify_TaskHandle = osThreadNew(Identify_Task_entry, NULL, &Identify_Task_attributes);

  /* creation of Control_tasks */
  Control_tasksHandle = osThreadNew(Control_tasks_entry, NULL, &Control_tasks_attributes);

  /* creation of Display_Task */
  Display_TaskHandle = osThreadNew(Display_Task_entry, NULL, &Display_Task_attributes);

  imageReadySemaphore = osSemaphoreNew(1, 1, &imageReadySemaphore_attributes);
	// 创建互斥锁
	recognitionMutex = osMutexNew(&recognitionMutex_attr);
}


void Identify_Task_entry(void *argument)
{
    DataPacket_t dataPacket;
    osStatus_t status;
    AI_Init();  // 初始化第一个神经网络

    for(;;)
    {
        osSemaphoreAcquire(imageReadySemaphore, osWaitForever);

        prepare_yolo_data(in_data);
        AI_Run(in_data, out_data);  // 运行第一个神经网络，传入输入数据，获取输出数据
        post_process(out_data, &recognition_flag1, &target_x, &target_y);

        // 使用互斥锁保护全局变量访问
        osMutexAcquire(recognitionMutex, osWaitForever);

        // 检查消息队列是否为空，如果不为空则清空消息队列
        while (osMessageQueueGetCount(DataQueueHandle) > 0) {
            status = osMessageQueueGet(DataQueueHandle, &dataPacket, NULL, osFlagsWaitAny);
            if (status != osOK) {
                break; // 如果获取消息失败，跳出循环
            }
        }

        dataPacket.Target_X = target_x;
        dataPacket.Target_Y = target_y;
        dataPacket.recognition = recognition_flag1;
				// 计算与YOZ面的夹角
				dataPacket.angle = calculate_angle_with_yoz_plane(intrinsics, target_x, target_y);
				if(dataPacket.angle > 0 )
				{
					dataPacket.direction = 'R';
				}
				else
				{
					dataPacket.direction = 'L';
				}
				dataPacket.angle = fabs(dataPacket.angle);
        if (recognition_flag1) {
            printf("Target detected at (%d, %d)\n", target_x, target_y);
					
        } else {
            printf("No target detected\n");
        }
        // 打印 angle 和 direction
        printf("Angle: %f\n", dataPacket.angle);
        printf("Direction: %c\n", dataPacket.direction);

        // 将新的数据包发送到消息队列
        status = osMessageQueuePut(DataQueueHandle, &dataPacket, 0, 0);
        if (status != osOK) {
            printf("Failed to send data to DataQueue\n");
        }

        osMutexRelease(recognitionMutex);

        osSemaphoreRelease(imageReadySemaphore);
        bufferReady = 0;
        osDelay(10);
    }
}



// 任务入口函数
void Control_tasks_entry(void *argument) {
	  printf("Control_tasks_entry\n"); // 输出调试信息
    DataPacket_t receivedData; // 存储接收数据的结构体
    int positions = 0, positions_before = 0; // 目标位置状态变量
    float angle_c = 0.0f; // 角度变量初始化为0
    char direction_c; // 方向变量初始化为"None"
    uint32_t last_zero_time = 0; // 上一次识别为0的时间
    bool is_servo_lifted = true; // 舵机状态标志位，初始为抬起状态
	
		printf("bee000\n"); // 输出调试信息
		send_servo_information();
    // 初始化舵机为抬起状态
    control_servo(true);

    // 无限循环
    for (;;) {
			
				osMutexAcquire(recognitionMutex, osWaitForever);
        // 从消息队列中获取数据
        if (osMessageQueueGet(DataQueueHandle, &receivedData, NULL, 0) == osOK) {
					  // 使用互斥锁保护全局变量访问
            positions_before = positions; // 更新前一时刻位置状态
            positions = receivedData.recognition; // 更新当前识别位置状态
            angle_c = receivedData.angle; // 更新角度信息
            direction_c = receivedData.direction;
        }
				osMutexRelease(recognitionMutex);

        // 无目标识别逻辑
        if (positions_before == 0 && positions == 0) 
				{
            uint32_t current_time = osKernelGetTickCount(); // 获取当前系统时间
            send_motor_speed(0x00, 0.4f, 0x02, -0.4f); // 发送电机速度指令
            printf("No detect, go straight\n"); // 输出调试信息

            if (last_zero_time == 0) {
                // 如果上一次识别为0的时间未初始化
                last_zero_time = current_time; // 初始化为当前时间
            } else if (current_time - last_zero_time > 5000) {
                // 如果当前时间距离上一次识别为0的时间超过5000ms
                if (!is_servo_lifted) {
                    control_servo(true); // 抬起舵机
                    is_servo_lifted = true; // 更新舵机状态为抬起
                    printf("LIFT after more than 5s\n"); // 输出调试信息
                }
            }
						// 打印current_time的具体数值
						printf("current_time: %u\n", current_time - last_zero_time);
        }
				else 
				{
            // 有目标识别逻辑
            if (positions == 0) 
						{
                send_motor_speed(0x00, 0, 0x02, 0); // 停止电机
								osDelay(500); 
            } 
						else 
						{
							
								//send_beep_information();//蜂鸣器_______________________________________________
								//printf("send_beep_information\n"); // 输出调试信息
                float delay_time = calculate_delay_time(angle_c) * 0.3f; // 计算延迟时间
                printf("Delay time: %f\n", delay_time); // 输出调试信息

                last_zero_time = 0; // 重置上一次识别为0的时间
							
                if (is_servo_lifted) {
                    control_servo(false); // 放下舵机
                    is_servo_lifted = false; // 更新舵机状态为抬起
                    printf("put down servo\n"); // 输出调试信息
                }
                if (direction_c == 'L' && angle_c > 4.5f) {
                    // 如果方向为左且角度大于3
                    send_motor_speed(0x00, -0.1f, 0x02, -0.1f); // 发送左转指令
                    osDelay((uint32_t)(delay_time)); // 延迟指定时间
										angle_c = 0.0f;
                    send_motor_speed(0x00, 0, 0x02, 0); // 停止电机
                    printf("Turn left\n"); // 输出调试信息
                } else if (direction_c == 'R' && angle_c > 4.5f) {
                    // 如果方向为右且角度大于3
                    send_motor_speed(0x00, 0.1f, 0x02, 0.1f); // 发送右转指令
                    osDelay((uint32_t)(delay_time)); // 延迟指定时间
										angle_c = 0.0f;
                    send_motor_speed(0x00, 0, 0x02, 0); // 停止电机
                    printf("Turn right\n"); // 输出调试信息
                } else {
                    send_motor_speed(0x00, 0.1f, 0x02, -0.1f); // 发送直行指令
                    osDelay(500); // 延迟1秒
                    printf("Identify and go straight\n"); // 输出调试信息
                }
            }
        }

        osDelay(1); // 每次循环后延迟1ms，防止任务一直占用CPU
    }
}



void Display_Task_entry(void *argument)
{
		// 切换回第 0 层以显示摄像头数据
    BSP_LCD_SelectLayer(0);
    BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);  // 初始化摄像头和DMA
	

    BSP_LCD_LayerDefaultInit(1, CAMERA_FRAME_BUFFER + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 2));  // 初始化LCD的第1层，用于画矩形框
    BSP_LCD_SetTransparency(1, 80);  // 设置透明度为80（半透明）
    BSP_LCD_SelectLayer(1);  // 选择LCD的第1层
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);  // 清空第1层显示为白色背景

    DataPacket_t data;  // 定义数据包
    /* USER CODE BEGIN Display_Task_entry */
    /* Infinite loop */
    for (;;)
    {

			  // 清空第1层
        BSP_LCD_SelectLayer(1); // 确保在第1层进行操作
        // 清空第1层
        BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
				osMutexAcquire(recognitionMutex, osWaitForever);
        // 从消息队列中获取数据
        if (osMessageQueueGet(DataQueueHandle, &data, NULL, 0) == osOK) {
            // 检查是否识别到目标
            if (data.recognition == 1) {
                // 在图像上方显示文本信息 "Recognized"
                Draw_Text(10, 10, (uint8_t*)"Recognized", LCD_COLOR_RED);
								Draw_Hollow_Square(data.Target_X, data.Target_Y, 8, LCD_COLOR_RED);
                // 在指定坐标位置画一个小圆
                //BSP_LCD_FillCircle(data.Target_X, data.Target_Y, 5);  // 画一个半径为5的圆
            } else {
                // 在图像上方显示文本信息 "Not recognized"
                Draw_Text(10, 10, (uint8_t*)"Not recognized", LCD_COLOR_BLUE);
            }
        }
				osMutexRelease(recognitionMutex);
				
        osDelay(10);  // 控制帧率，较短的延时可以使动画更流畅
    }
    /* USER CODE END Display_Task_entry */
}


/*以下是神经网络相关函数*/


// 初始化第一个神经网络
 void AI_Init(void)
{

    ai_error err;  // 定义一个错误结构体

    // 创建一个包含激活缓冲区地址的本地数组
    const ai_handle act_addr[] = { activations };
    // 创建模型实例
    err = ai_network_create_and_init(&network, act_addr, NULL);
    if (err.type != AI_ERROR_NONE) {
        printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);  // 打印错误信息
        Error_Handler();  // 调用错误处理函数
    }
    ai_input = ai_network_inputs_get(network, NULL);  // 获取网络的输入缓冲区
    ai_output = ai_network_outputs_get(network, NULL);  // 获取网络的输出缓冲区
}

// 运行第一个神经网络
void AI_Run(float* pIn, float* pOut)
{
    ai_i32 batch;  // 定义一个批处理变量
    ai_error err;  // 定义一个错误结构体

    // 使用数据更新输入输出处理器
    ai_input[0].data = AI_HANDLE_PTR(pIn);
    ai_output[0].data = AI_HANDLE_PTR(pOut);

    batch = ai_network_run(network, ai_input, ai_output);  // 运行神经网络
    if (batch != 1) {
        err = ai_network_get_error(network);  // 获取错误信息
        printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);  // 打印错误信息
        Error_Handler();  // 调用错误处理函数
    }
}


// 准备YOLO数据，输入为浮点型指针
void prepare_yolo_data(float* in_data) {
    // 从内存地址读取图像数据
    uint32_t* img_argb8888 = (uint32_t*)image_data;

    // 将 ARGB8888 转换为灰度图像，调整大小为 65x65，并进行归一化
    argb8888_to_gray_and_resize(img_argb8888, in_data, 480, 272, 65, 65);

    // 打印预处理后的输入数据（调试用）
    /*
    for (int i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
        printf("in_data[%d] = %f\n", i, in_data[i]);
    }
    */
}

// 将ARGB8888格式转换为灰度图像，并调整大小为目标大小
void argb8888_to_gray_and_resize(uint32_t* argb, float* resized, int src_width, int src_height, int dst_width, int dst_height) {
    // 计算缩放比例
    float x_scale = (float)src_width / dst_width;
    float y_scale = (float)src_height / dst_height;

    // 遍历目标图像每个像素
    for (int y = 0; y < dst_height; y++) {
        for (int x = 0; x < dst_width; x++) {
            // 计算在原始图像中的坐标
            int src_x = (int)(x * x_scale);
            int src_y = (int)(y * y_scale);

            // 确保坐标在有效范围内
            if (src_x < 0) src_x = 0;
            if (src_x >= src_width) src_x = src_width - 1;
            if (src_y < 0) src_y = 0;
            if (src_y >= src_height) src_y = src_height - 1;

            // 计算在原始图像数组中的索引
            int src_idx = src_y * src_width + src_x;

            // 提取ARGB8888中的R、G、B分量
            uint32_t pixel = argb[src_idx];
            float r = ((pixel >> 16) & 0xFF);
            float g = ((pixel >> 8) & 0xFF);
            float b = (pixel & 0xFF);

            // 根据灰度转换公式计算灰度值，并归一化到 [0, 1] 范围
            float gray = (0.21f * r + 0.72f * g + 0.07f * b) / 255.0f;

            // 将灰度值存入调整后的图像数组中
            int dst_idx = y * dst_width + x;
            resized[dst_idx] = gray;
        }
    }
}







// 后处理函数，从神经网络输出中提取目标信息
void post_process(float* out_data, int* recognition_flag1, int* target_x, int* target_y) {
    const int grid_size = 9; // 输出网格的大小 (9x9)
    const int num_classes = 2; // 类别数量 (背景+目标)
		float table[9] = {0 , 0.7998 , 1.7712 , 2.857 , 4 , 5.143 , 6.2287 , 7.2 , 8};
    // 初始化最大概率和目标坐标
    float max_prob = 0.0;
    int max_x = -1, max_y = -1;

    // 遍历输出网格，找到最大概率的目标
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            // 获取当前网格的目标概率
            float object_prob = out_data[(i * grid_size + j) * num_classes + 1]; // 假设类别1是目标

            //printf("Grid (%d, %d): Prob = %f\n", i, j, object_prob);

            // 如果当前网格的目标概率大于最大概率，更新最大概率和目标坐标
            if (object_prob > max_prob) {
                max_prob = object_prob;
                max_x = j;
                max_y = i;
            }
        }
    }

    //printf("Max Prob = %f at Grid (%d, %d)\n", max_prob, max_x, max_y);

    // 判断是否识别到目标
    if (max_prob > 0.5f) { // 假设阈值为0.5
        *recognition_flag1 = 1;
				
        // 将网格坐标映射回原始图像坐标
        *target_x = (table[max_x] * 480) / (grid_size - 1);
        *target_y = (table[max_y] * 272) / (grid_size - 1);
    } else {
        *recognition_flag1 = 0;
        *target_x = -1;
        *target_y = -1;
    }

    //printf("Recognition Flag = %d, Target = (%d, %d)\n", *recognition_flag1, *target_x, *target_y);
}




// 绘制文本的函数
void Draw_Text(uint16_t x, uint16_t y, uint8_t* text, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, y, text, LEFT_MODE);
}

// 绘制小圆的函数
void Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillCircle(x, y, radius);
}

// 绘制空心小方框的函数
void Draw_Hollow_Square(uint16_t x, uint16_t y, uint16_t side_length, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DrawRect(x, y, side_length, side_length);
}



// 计算某一个像素点在世界坐标系中与YOZ面夹角的函数
double calculate_angle_with_yoz_plane(CameraIntrinsics intrinsics, int pixel_x, int pixel_y) {
    // 将目标像素坐标转换为相机坐标
    double x_camera = (pixel_x - intrinsics.cx) * intrinsics.fx;
    /*double y_camera = (pixel_y - intrinsics.cy) * intrinsics.fy;*/
    double z_camera = 1.0; // 假设z=1

    // 在相机坐标系中计算点的角度
    double angle = atan2(x_camera, z_camera);

    // 将弧度转换为度
    double angle_degrees = angle * (180.0 / M_PI);

    return angle_degrees;
}

