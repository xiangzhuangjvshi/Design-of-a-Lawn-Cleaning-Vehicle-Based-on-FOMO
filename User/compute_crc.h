#ifndef COMPUTE_CRC_H
#define COMPUTE_CRC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// 定义相机内参矩阵

void send_beep_information(void);
void send_motor_speed(uint8_t motor_id1, float speed1, uint8_t motor_id2, float speed2);
void control_servo(bool is_lifted);
void send_servo_information(void);
//AngleDirection calculate_angle_with_yoz_plane(CameraIntrinsics intrinsics, int pixel_x, int pixel_y);
#endif // COMPUTE_CRC_H
