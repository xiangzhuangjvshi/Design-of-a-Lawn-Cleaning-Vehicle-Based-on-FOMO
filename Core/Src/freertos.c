#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>

#include "ai_platform.h"  // ����AIƽ̨��ص�ͷ�ļ�
#include "network.h"
#include "network_data.h"
#include <stdio.h>
#include "usart.h"
#include "stm32746g_discovery_camera.h"
#include "stm32746g_discovery_lcd.h"

#include <compute_crc.h>
#include <math.h> // ������ѧ��ͷ�ļ�
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// ��������ڲξ���
typedef struct {
    double fx, fy, cx, cy;
} CameraIntrinsics;

// ���û�ж��� PI������ PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double calculate_angle_with_yoz_plane(CameraIntrinsics intrinsics, int pixel_x, int pixel_y);
CameraIntrinsics intrinsics = { 0.001087, 0.001081, 250.9719 , 97.9172};

int recognition_flag1;
int target_x, target_y;
void post_process(float* out_data, int* recognition_flag, int* target_x, int* target_y);

/*���ƴ���*/
void Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
void Draw_Text(uint16_t x, uint16_t y, uint8_t* text, uint32_t color);
void Draw_Hollow_Square(uint16_t x, uint16_t y, uint16_t side_length, uint32_t color);

osSemaphoreId_t imageReadySemaphore;
uint8_t bufferReady = 0;


ai_handle network;  // ������ľ������

ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];  // ��һ��������ļ�������
ai_buffer* ai_input;  // AI�������ݻ�����
ai_buffer* ai_output;  // AI������ݻ�����
float in_data[AI_NETWORK_IN_1_SIZE];  // ��һ�����������������
float out_data[AI_NETWORK_OUT_1_SIZE];  // ��һ����������������


#define GRID_SIZE 7
#define NUM_CLASSES 2

// ��ӡ�ڶ�����������������
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


// �Ѿ���ϺõĲ���
float popt[] = {-1.82204070e-07, 1.96926650e-05, 4.73758907e-04, -1.22931100e-01, 2.26574694e+01, 2.65264304e+02};
// �����ӳ�ʱ��ĺ���
float calculate_delay_time(float angle_deviation) {
    float abs_angle_deviation = fabs(angle_deviation);
    float delay_sec = (popt[0] * pow(abs_angle_deviation, 5) + 
                      popt[1] * pow(abs_angle_deviation, 4) + 
                      popt[2] * pow(abs_angle_deviation, 3) + 
                      popt[3] * pow(abs_angle_deviation, 2) + 
                      popt[4] * abs_angle_deviation + 
                      popt[5]);
    //float delay_sec = delay_ms / 1000; // ������ת��Ϊ��
    return delay_sec;
}

/*���廥����*/
osMutexId_t recognitionMutex;
const osMutexAttr_t recognitionMutex_attr = {
    "recognitionMutex",   // ��������
    osMutexRecursive,     // �ݹ���
    NULL,                 // ���ڴ����
    0                     // ����С
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
    int recognition; // ʶ��Ŀ��ı�־λ��1Ϊʶ��Ŀ�꣬0Ϊû��ʶ��Ŀ��
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
	// ����������
	recognitionMutex = osMutexNew(&recognitionMutex_attr);
}


void Identify_Task_entry(void *argument)
{
    DataPacket_t dataPacket;
    osStatus_t status;
    AI_Init();  // ��ʼ����һ��������

    for(;;)
    {
        osSemaphoreAcquire(imageReadySemaphore, osWaitForever);

        prepare_yolo_data(in_data);
        AI_Run(in_data, out_data);  // ���е�һ�������磬�����������ݣ���ȡ�������
        post_process(out_data, &recognition_flag1, &target_x, &target_y);

        // ʹ�û���������ȫ�ֱ�������
        osMutexAcquire(recognitionMutex, osWaitForever);

        // �����Ϣ�����Ƿ�Ϊ�գ������Ϊ���������Ϣ����
        while (osMessageQueueGetCount(DataQueueHandle) > 0) {
            status = osMessageQueueGet(DataQueueHandle, &dataPacket, NULL, osFlagsWaitAny);
            if (status != osOK) {
                break; // �����ȡ��Ϣʧ�ܣ�����ѭ��
            }
        }

        dataPacket.Target_X = target_x;
        dataPacket.Target_Y = target_y;
        dataPacket.recognition = recognition_flag1;
				// ������YOZ��ļн�
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
        // ��ӡ angle �� direction
        printf("Angle: %f\n", dataPacket.angle);
        printf("Direction: %c\n", dataPacket.direction);

        // ���µ����ݰ����͵���Ϣ����
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



// ������ں���
void Control_tasks_entry(void *argument) {
	  printf("Control_tasks_entry\n"); // ���������Ϣ
    DataPacket_t receivedData; // �洢�������ݵĽṹ��
    int positions = 0, positions_before = 0; // Ŀ��λ��״̬����
    float angle_c = 0.0f; // �Ƕȱ�����ʼ��Ϊ0
    char direction_c; // ���������ʼ��Ϊ"None"
    uint32_t last_zero_time = 0; // ��һ��ʶ��Ϊ0��ʱ��
    bool is_servo_lifted = true; // ���״̬��־λ����ʼΪ̧��״̬
	
		printf("bee000\n"); // ���������Ϣ
		send_servo_information();
    // ��ʼ�����Ϊ̧��״̬
    control_servo(true);

    // ����ѭ��
    for (;;) {
			
				osMutexAcquire(recognitionMutex, osWaitForever);
        // ����Ϣ�����л�ȡ����
        if (osMessageQueueGet(DataQueueHandle, &receivedData, NULL, 0) == osOK) {
					  // ʹ�û���������ȫ�ֱ�������
            positions_before = positions; // ����ǰһʱ��λ��״̬
            positions = receivedData.recognition; // ���µ�ǰʶ��λ��״̬
            angle_c = receivedData.angle; // ���½Ƕ���Ϣ
            direction_c = receivedData.direction;
        }
				osMutexRelease(recognitionMutex);

        // ��Ŀ��ʶ���߼�
        if (positions_before == 0 && positions == 0) 
				{
            uint32_t current_time = osKernelGetTickCount(); // ��ȡ��ǰϵͳʱ��
            send_motor_speed(0x00, 0.4f, 0x02, -0.4f); // ���͵���ٶ�ָ��
            printf("No detect, go straight\n"); // ���������Ϣ

            if (last_zero_time == 0) {
                // �����һ��ʶ��Ϊ0��ʱ��δ��ʼ��
                last_zero_time = current_time; // ��ʼ��Ϊ��ǰʱ��
            } else if (current_time - last_zero_time > 5000) {
                // �����ǰʱ�������һ��ʶ��Ϊ0��ʱ�䳬��5000ms
                if (!is_servo_lifted) {
                    control_servo(true); // ̧����
                    is_servo_lifted = true; // ���¶��״̬Ϊ̧��
                    printf("LIFT after more than 5s\n"); // ���������Ϣ
                }
            }
						// ��ӡcurrent_time�ľ�����ֵ
						printf("current_time: %u\n", current_time - last_zero_time);
        }
				else 
				{
            // ��Ŀ��ʶ���߼�
            if (positions == 0) 
						{
                send_motor_speed(0x00, 0, 0x02, 0); // ֹͣ���
								osDelay(500); 
            } 
						else 
						{
							
								//send_beep_information();//������_______________________________________________
								//printf("send_beep_information\n"); // ���������Ϣ
                float delay_time = calculate_delay_time(angle_c) * 0.3f; // �����ӳ�ʱ��
                printf("Delay time: %f\n", delay_time); // ���������Ϣ

                last_zero_time = 0; // ������һ��ʶ��Ϊ0��ʱ��
							
                if (is_servo_lifted) {
                    control_servo(false); // ���¶��
                    is_servo_lifted = false; // ���¶��״̬Ϊ̧��
                    printf("put down servo\n"); // ���������Ϣ
                }
                if (direction_c == 'L' && angle_c > 4.5f) {
                    // �������Ϊ���ҽǶȴ���3
                    send_motor_speed(0x00, -0.1f, 0x02, -0.1f); // ������תָ��
                    osDelay((uint32_t)(delay_time)); // �ӳ�ָ��ʱ��
										angle_c = 0.0f;
                    send_motor_speed(0x00, 0, 0x02, 0); // ֹͣ���
                    printf("Turn left\n"); // ���������Ϣ
                } else if (direction_c == 'R' && angle_c > 4.5f) {
                    // �������Ϊ���ҽǶȴ���3
                    send_motor_speed(0x00, 0.1f, 0x02, 0.1f); // ������תָ��
                    osDelay((uint32_t)(delay_time)); // �ӳ�ָ��ʱ��
										angle_c = 0.0f;
                    send_motor_speed(0x00, 0, 0x02, 0); // ֹͣ���
                    printf("Turn right\n"); // ���������Ϣ
                } else {
                    send_motor_speed(0x00, 0.1f, 0x02, -0.1f); // ����ֱ��ָ��
                    osDelay(500); // �ӳ�1��
                    printf("Identify and go straight\n"); // ���������Ϣ
                }
            }
        }

        osDelay(1); // ÿ��ѭ�����ӳ�1ms����ֹ����һֱռ��CPU
    }
}



void Display_Task_entry(void *argument)
{
		// �л��ص� 0 ������ʾ����ͷ����
    BSP_LCD_SelectLayer(0);
    BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);  // ��ʼ������ͷ��DMA
	

    BSP_LCD_LayerDefaultInit(1, CAMERA_FRAME_BUFFER + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 2));  // ��ʼ��LCD�ĵ�1�㣬���ڻ����ο�
    BSP_LCD_SetTransparency(1, 80);  // ����͸����Ϊ80����͸����
    BSP_LCD_SelectLayer(1);  // ѡ��LCD�ĵ�1��
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);  // ��յ�1����ʾΪ��ɫ����

    DataPacket_t data;  // �������ݰ�
    /* USER CODE BEGIN Display_Task_entry */
    /* Infinite loop */
    for (;;)
    {

			  // ��յ�1��
        BSP_LCD_SelectLayer(1); // ȷ���ڵ�1����в���
        // ��յ�1��
        BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
				osMutexAcquire(recognitionMutex, osWaitForever);
        // ����Ϣ�����л�ȡ����
        if (osMessageQueueGet(DataQueueHandle, &data, NULL, 0) == osOK) {
            // ����Ƿ�ʶ��Ŀ��
            if (data.recognition == 1) {
                // ��ͼ���Ϸ���ʾ�ı���Ϣ "Recognized"
                Draw_Text(10, 10, (uint8_t*)"Recognized", LCD_COLOR_RED);
								Draw_Hollow_Square(data.Target_X, data.Target_Y, 8, LCD_COLOR_RED);
                // ��ָ������λ�û�һ��СԲ
                //BSP_LCD_FillCircle(data.Target_X, data.Target_Y, 5);  // ��һ���뾶Ϊ5��Բ
            } else {
                // ��ͼ���Ϸ���ʾ�ı���Ϣ "Not recognized"
                Draw_Text(10, 10, (uint8_t*)"Not recognized", LCD_COLOR_BLUE);
            }
        }
				osMutexRelease(recognitionMutex);
				
        osDelay(10);  // ����֡�ʣ��϶̵���ʱ����ʹ����������
    }
    /* USER CODE END Display_Task_entry */
}


/*��������������غ���*/


// ��ʼ����һ��������
 void AI_Init(void)
{

    ai_error err;  // ����һ������ṹ��

    // ����һ���������������ַ�ı�������
    const ai_handle act_addr[] = { activations };
    // ����ģ��ʵ��
    err = ai_network_create_and_init(&network, act_addr, NULL);
    if (err.type != AI_ERROR_NONE) {
        printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);  // ��ӡ������Ϣ
        Error_Handler();  // ���ô�������
    }
    ai_input = ai_network_inputs_get(network, NULL);  // ��ȡ��������뻺����
    ai_output = ai_network_outputs_get(network, NULL);  // ��ȡ��������������
}

// ���е�һ��������
void AI_Run(float* pIn, float* pOut)
{
    ai_i32 batch;  // ����һ�����������
    ai_error err;  // ����һ������ṹ��

    // ʹ�����ݸ����������������
    ai_input[0].data = AI_HANDLE_PTR(pIn);
    ai_output[0].data = AI_HANDLE_PTR(pOut);

    batch = ai_network_run(network, ai_input, ai_output);  // ����������
    if (batch != 1) {
        err = ai_network_get_error(network);  // ��ȡ������Ϣ
        printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);  // ��ӡ������Ϣ
        Error_Handler();  // ���ô�������
    }
}


// ׼��YOLO���ݣ�����Ϊ������ָ��
void prepare_yolo_data(float* in_data) {
    // ���ڴ��ַ��ȡͼ������
    uint32_t* img_argb8888 = (uint32_t*)image_data;

    // �� ARGB8888 ת��Ϊ�Ҷ�ͼ�񣬵�����СΪ 65x65�������й�һ��
    argb8888_to_gray_and_resize(img_argb8888, in_data, 480, 272, 65, 65);

    // ��ӡԤ�������������ݣ������ã�
    /*
    for (int i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
        printf("in_data[%d] = %f\n", i, in_data[i]);
    }
    */
}

// ��ARGB8888��ʽת��Ϊ�Ҷ�ͼ�񣬲�������СΪĿ���С
void argb8888_to_gray_and_resize(uint32_t* argb, float* resized, int src_width, int src_height, int dst_width, int dst_height) {
    // �������ű���
    float x_scale = (float)src_width / dst_width;
    float y_scale = (float)src_height / dst_height;

    // ����Ŀ��ͼ��ÿ������
    for (int y = 0; y < dst_height; y++) {
        for (int x = 0; x < dst_width; x++) {
            // ������ԭʼͼ���е�����
            int src_x = (int)(x * x_scale);
            int src_y = (int)(y * y_scale);

            // ȷ����������Ч��Χ��
            if (src_x < 0) src_x = 0;
            if (src_x >= src_width) src_x = src_width - 1;
            if (src_y < 0) src_y = 0;
            if (src_y >= src_height) src_y = src_height - 1;

            // ������ԭʼͼ�������е�����
            int src_idx = src_y * src_width + src_x;

            // ��ȡARGB8888�е�R��G��B����
            uint32_t pixel = argb[src_idx];
            float r = ((pixel >> 16) & 0xFF);
            float g = ((pixel >> 8) & 0xFF);
            float b = (pixel & 0xFF);

            // ���ݻҶ�ת����ʽ����Ҷ�ֵ������һ���� [0, 1] ��Χ
            float gray = (0.21f * r + 0.72f * g + 0.07f * b) / 255.0f;

            // ���Ҷ�ֵ����������ͼ��������
            int dst_idx = y * dst_width + x;
            resized[dst_idx] = gray;
        }
    }
}







// �����������������������ȡĿ����Ϣ
void post_process(float* out_data, int* recognition_flag1, int* target_x, int* target_y) {
    const int grid_size = 9; // �������Ĵ�С (9x9)
    const int num_classes = 2; // ������� (����+Ŀ��)
		float table[9] = {0 , 0.7998 , 1.7712 , 2.857 , 4 , 5.143 , 6.2287 , 7.2 , 8};
    // ��ʼ�������ʺ�Ŀ������
    float max_prob = 0.0;
    int max_x = -1, max_y = -1;

    // ������������ҵ������ʵ�Ŀ��
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            // ��ȡ��ǰ�����Ŀ�����
            float object_prob = out_data[(i * grid_size + j) * num_classes + 1]; // �������1��Ŀ��

            //printf("Grid (%d, %d): Prob = %f\n", i, j, object_prob);

            // �����ǰ�����Ŀ����ʴ��������ʣ����������ʺ�Ŀ������
            if (object_prob > max_prob) {
                max_prob = object_prob;
                max_x = j;
                max_y = i;
            }
        }
    }

    //printf("Max Prob = %f at Grid (%d, %d)\n", max_prob, max_x, max_y);

    // �ж��Ƿ�ʶ��Ŀ��
    if (max_prob > 0.5f) { // ������ֵΪ0.5
        *recognition_flag1 = 1;
				
        // ����������ӳ���ԭʼͼ������
        *target_x = (table[max_x] * 480) / (grid_size - 1);
        *target_y = (table[max_y] * 272) / (grid_size - 1);
    } else {
        *recognition_flag1 = 0;
        *target_x = -1;
        *target_y = -1;
    }

    //printf("Recognition Flag = %d, Target = (%d, %d)\n", *recognition_flag1, *target_x, *target_y);
}




// �����ı��ĺ���
void Draw_Text(uint16_t x, uint16_t y, uint8_t* text, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DisplayStringAt(x, y, text, LEFT_MODE);
}

// ����СԲ�ĺ���
void Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_FillCircle(x, y, radius);
}

// ���ƿ���С����ĺ���
void Draw_Hollow_Square(uint16_t x, uint16_t y, uint16_t side_length, uint32_t color) {
    BSP_LCD_SetTextColor(color);
    BSP_LCD_DrawRect(x, y, side_length, side_length);
}



// ����ĳһ�����ص�����������ϵ����YOZ��нǵĺ���
double calculate_angle_with_yoz_plane(CameraIntrinsics intrinsics, int pixel_x, int pixel_y) {
    // ��Ŀ����������ת��Ϊ�������
    double x_camera = (pixel_x - intrinsics.cx) * intrinsics.fx;
    /*double y_camera = (pixel_y - intrinsics.cy) * intrinsics.fy;*/
    double z_camera = 1.0; // ����z=1

    // ���������ϵ�м����ĽǶ�
    double angle = atan2(x_camera, z_camera);

    // ������ת��Ϊ��
    double angle_degrees = angle * (180.0 / M_PI);

    return angle_degrees;
}

