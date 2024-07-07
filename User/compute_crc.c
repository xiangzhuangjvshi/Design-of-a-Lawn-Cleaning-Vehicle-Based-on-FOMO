#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "usart.h"  // ��������˴���ͨ�ŵ�ͷ�ļ�������������ʵ����Ŀ�޸�
#include <compute_crc.h>
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include <math.h>
#include <string.h>
// CRC8�����ڼ���CRCУ��
const uint8_t crc8_table[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

// ��������ת��ΪС���ֽ����ʮ�������ַ���
void float_to_little_endian_hex(float f, uint8_t* hex_array) {
    uint8_t* ptr = (uint8_t*)&f;  // ��ȡ���������ֽڱ�ʾ
    for (int i = 0; i < sizeof(float); i++) {
        hex_array[i] = ptr[i];  // ��ÿ���ֽڸ��Ƶ�������
    }
}

// ����CRC8У���
uint8_t checksum_crc8(const uint8_t* buf, size_t len) {
    uint8_t check = 0;  // ��ʼ��У���
    for (size_t i = 0; i < len; i++) {
        check = crc8_table[check ^ buf[i]];  // ����У���
    }
    return check & 0xFF;  // ����У��͵ĵ�8λ
}

// ����˫�����������
void two_motor_control_coding(uint8_t motor_id1, float v1, uint8_t motor_id2, float v2, uint8_t* output) {
    uint8_t frame_head[] = { 0xAA, 0x55 };  // ֡ͷ
    uint8_t v1_float_bytes[4];  // �洢��һ�����������ֽ�
    uint8_t v2_float_bytes[4];  // �洢�ڶ������������ֽ�
    float_to_little_endian_hex(v1, v1_float_bytes);  // ת����һ��������
    float_to_little_endian_hex(v2, v2_float_bytes);  // ת���ڶ���������

    uint8_t mul_motor_para[10];  // �洢�������
    mul_motor_para[0] = motor_id1;  // ���õ�һ�����ID
    memcpy(mul_motor_para + 1, v1_float_bytes, 4);  // ���Ƶ�һ������ٶ�
    mul_motor_para[5] = motor_id2;  // ���õڶ������ID
    memcpy(mul_motor_para + 6, v2_float_bytes, 4);  // ���Ƶڶ�������ٶ�

    uint8_t mul_data1[] = { 0x03, 0x12, 0x01, 0x02 };  // ����ͷ����
    uint8_t mul_data2[] = { 0xF4, 0x6B, 0x92, 0x0A, 0x7B, 0xD2 };  // ����β����
    uint8_t mul_data[20];  // ���������ݰ�
    memcpy(mul_data, mul_data1, 4);  // ��������ͷ
    memcpy(mul_data + 4, mul_motor_para, 10);  // ���Ƶ������
    memcpy(mul_data + 14, mul_data2, 6);  // ��������β

    uint8_t mul_crc_result = checksum_crc8(mul_data, 20);  // ����CRCУ���

    memcpy(output, frame_head, 2);  // ����֡ͷ�����������
    memcpy(output + 2, mul_data, 20);  // �������ݵ����������
    output[22] = mul_crc_result;  // ���CRCУ���

}

// ���ƶ������
void control_servo(bool is_lifted) {
		//taskENTER_CRITICAL();  // �����ٽ����������ж�-----------------------------
    const uint8_t lifted_command[] = {0xAA, 0x55, 0x04, 0x06, 0x03, 0xE8, 0x03, 0x01, 0xF4, 0x01, 0xF9};  // ���̧������
    const uint8_t lowered_command[] = {0xAA, 0x55, 0x04, 0x06, 0x03, 0xE8, 0x03, 0x01, 0xE6, 0x05, 0xE5};  // �����������

    uint8_t command[sizeof(lifted_command)];  // ���������ڴ洢����
    if (is_lifted) {
        memcpy(command, lifted_command, sizeof(lifted_command));  // ����̧������
    } else {
        memcpy(command, lowered_command, sizeof(lowered_command));  // ���Ʒ�������
    }
		HAL_UART_Init(&huart6);
    // ��������ʹ��HAL�ⷢ�ʹ������ݣ����庯�����Ͳ�����Ҫ����ʵ���������
    //HAL_UART_Transmit(&huart6, command, sizeof(command), HAL_MAX_DELAY); // ��������
		HAL_UART_Transmit_IT(&huart6, command, sizeof(command)); // ��������
		osDelay(100);
		HAL_UART_DeInit(&huart6);
		//taskEXIT_CRITICAL();   // �˳��ٽ����������ж�------------------------------
}

// ���ͷ�������Ϣ����
void send_beep_information() {
		//taskENTER_CRITICAL();  // �����ٽ����������ж�-----------------------------
    const uint8_t hexData[] = {0xAA, 0x55, 0x02, 0x08, 0x78, 0x05, 0x64, 0x00, 0x64, 0x00, 0x05, 0x00, 0xF0};  // ����������

    // ��������ʹ��HAL�ⷢ�ʹ������ݣ����庯�����Ͳ�����Ҫ����ʵ���������
		HAL_UART_Init(&huart6);
    //HAL_UART_Transmit(&huart6, hexData, sizeof(hexData), HAL_MAX_DELAY);  // ���ͷ���������
		HAL_UART_Transmit_IT(&huart6, hexData, sizeof(hexData));
		osDelay(100);
		
		HAL_UART_DeInit(&huart6);
		//taskEXIT_CRITICAL();   // �˳��ٽ����������ж�------------------------------
}

// ���͵���ٶ���Ϣ����
void send_motor_speed(uint8_t motor_id1, float speed1, uint8_t motor_id2, float speed2) {
		//taskENTER_CRITICAL();  // �����ٽ����������ж�-----------------------------
    uint8_t result[23];  // �洢����Ļ�����
		HAL_UART_Init(&huart6);
    two_motor_control_coding(motor_id1, speed1, motor_id2, speed2, result);  // ��������������
    // ��������ʹ��HAL�ⷢ�ʹ������ݣ����庯�����Ͳ�����Ҫ����ʵ���������
    //HAL_UART_Transmit(&huart6, result, sizeof(result), HAL_MAX_DELAY);  // ���͵����������
		HAL_UART_Transmit_IT(&huart6, result, sizeof(result));  // ���͵����������
		osDelay(100);
		HAL_UART_DeInit(&huart6);
		//taskEXIT_CRITICAL();   // �˳��ٽ����������ж�------------------------------
}

// ���ͷ�������Ϣ����
void send_servo_information() {
		//taskENTER_CRITICAL();  // �����ٽ����������ж�-----------------------------
    const uint8_t command2[] = {0xAA, 0x55, 0x04, 0x06, 0x03, 0xE8, 0x03, 0x04, 0x78, 0x05, 0xCF};  // �̶����Ŷ��

    // ��������ʹ��HAL�ⷢ�ʹ������ݣ����庯�����Ͳ�����Ҫ����ʵ���������
		HAL_UART_Init(&huart6);
    //HAL_UART_Transmit(&huart6, hexData, sizeof(hexData), HAL_MAX_DELAY);  // ���ͷ���������
		HAL_UART_Transmit_IT(&huart6, command2, sizeof(command2));
		osDelay(100);
		
		HAL_UART_DeInit(&huart6);
		//taskEXIT_CRITICAL();   // �˳��ٽ����������ж�------------------------------
}
		

/*
// ��������ʾ���÷�
int main() {
    // ʾ���÷�
    float motor_speed1 = 0.5;  // �����ٶ�
    float motor_speed2 = 0.7;  // �ҵ���ٶ�
    uint8_t motor_id1 = 1;  // ���1��ID
    uint8_t motor_id2 = 2;  // ���2��ID

    // ���͵���ٶ���Ϣ
    send_motor_speed(motor_id1, motor_speed1, motor_id2, motor_speed2);

    // ���ƶ��̧������
    bool is_lifted = true;  // �Ƿ�̧����
    control_servo(is_lifted);

    // ���ͷ�������Ϣ
    send_beep_information();

    return 0;
}
*/


