/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sun Jun 30 18:34:38 2024
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "0xca5cfeeb471d368162d4d48836ad81ea"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Sun Jun 30 18:34:38 2024"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_1_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4225, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  Conv1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  Conv1_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_add_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8712, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  block_1_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 52272, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  block_1_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 52272, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  block_1_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  block_1_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  block_1_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2312, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  block_2_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  block_2_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  block_2_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  block_2_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  block_2_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2312, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  block_2_add_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2312, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  block_3_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  block_3_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 13872, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  block_3_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  block_3_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  block_3_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  block_4_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  block_4_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  block_4_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  block_4_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#27 */
AI_ARRAY_OBJ_DECLARE(
  block_4_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#28 */
AI_ARRAY_OBJ_DECLARE(
  block_4_add_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#29 */
AI_ARRAY_OBJ_DECLARE(
  block_5_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#30 */
AI_ARRAY_OBJ_DECLARE(
  block_5_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#31 */
AI_ARRAY_OBJ_DECLARE(
  block_5_depthwise_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#32 */
AI_ARRAY_OBJ_DECLARE(
  block_5_depthwise_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#33 */
AI_ARRAY_OBJ_DECLARE(
  block_5_project_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#34 */
AI_ARRAY_OBJ_DECLARE(
  block_5_add_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 648, AI_STATIC)

/* Array#35 */
AI_ARRAY_OBJ_DECLARE(
  block_6_expand_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#36 */
AI_ARRAY_OBJ_DECLARE(
  block_6_expand_relu_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 3888, AI_STATIC)

/* Array#37 */
AI_ARRAY_OBJ_DECLARE(
  head_conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#38 */
AI_ARRAY_OBJ_DECLARE(
  head_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2592, AI_STATIC)

/* Array#39 */
AI_ARRAY_OBJ_DECLARE(
  logits_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 162, AI_STATIC)

/* Array#40 */
AI_ARRAY_OBJ_DECLARE(
  softmax_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 162, AI_STATIC)

/* Array#41 */
AI_ARRAY_OBJ_DECLARE(
  Conv1_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#42 */
AI_ARRAY_OBJ_DECLARE(
  Conv1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#43 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 72, AI_STATIC)

/* Array#44 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#45 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#46 */
AI_ARRAY_OBJ_DECLARE(
  expanded_conv_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#47 */
AI_ARRAY_OBJ_DECLARE(
  block_1_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#48 */
AI_ARRAY_OBJ_DECLARE(
  block_1_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#49 */
AI_ARRAY_OBJ_DECLARE(
  block_1_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#50 */
AI_ARRAY_OBJ_DECLARE(
  block_1_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#51 */
AI_ARRAY_OBJ_DECLARE(
  block_1_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#52 */
AI_ARRAY_OBJ_DECLARE(
  block_1_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#53 */
AI_ARRAY_OBJ_DECLARE(
  block_2_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#54 */
AI_ARRAY_OBJ_DECLARE(
  block_2_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#55 */
AI_ARRAY_OBJ_DECLARE(
  block_2_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#56 */
AI_ARRAY_OBJ_DECLARE(
  block_2_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#57 */
AI_ARRAY_OBJ_DECLARE(
  block_2_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#58 */
AI_ARRAY_OBJ_DECLARE(
  block_2_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#59 */
AI_ARRAY_OBJ_DECLARE(
  block_3_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#60 */
AI_ARRAY_OBJ_DECLARE(
  block_3_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#61 */
AI_ARRAY_OBJ_DECLARE(
  block_3_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#62 */
AI_ARRAY_OBJ_DECLARE(
  block_3_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#63 */
AI_ARRAY_OBJ_DECLARE(
  block_3_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#64 */
AI_ARRAY_OBJ_DECLARE(
  block_3_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#65 */
AI_ARRAY_OBJ_DECLARE(
  block_4_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#66 */
AI_ARRAY_OBJ_DECLARE(
  block_4_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#67 */
AI_ARRAY_OBJ_DECLARE(
  block_4_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#68 */
AI_ARRAY_OBJ_DECLARE(
  block_4_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#69 */
AI_ARRAY_OBJ_DECLARE(
  block_4_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#70 */
AI_ARRAY_OBJ_DECLARE(
  block_4_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#71 */
AI_ARRAY_OBJ_DECLARE(
  block_5_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#72 */
AI_ARRAY_OBJ_DECLARE(
  block_5_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#73 */
AI_ARRAY_OBJ_DECLARE(
  block_5_depthwise_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 432, AI_STATIC)

/* Array#74 */
AI_ARRAY_OBJ_DECLARE(
  block_5_depthwise_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#75 */
AI_ARRAY_OBJ_DECLARE(
  block_5_project_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#76 */
AI_ARRAY_OBJ_DECLARE(
  block_5_project_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#77 */
AI_ARRAY_OBJ_DECLARE(
  block_6_expand_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 384, AI_STATIC)

/* Array#78 */
AI_ARRAY_OBJ_DECLARE(
  block_6_expand_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/* Array#79 */
AI_ARRAY_OBJ_DECLARE(
  head_conv2d_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1536, AI_STATIC)

/* Array#80 */
AI_ARRAY_OBJ_DECLARE(
  head_conv2d_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 32, AI_STATIC)

/* Array#81 */
AI_ARRAY_OBJ_DECLARE(
  logits_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 64, AI_STATIC)

/* Array#82 */
AI_ARRAY_OBJ_DECLARE(
  logits_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 2, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_project_weights, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 8), AI_STRIDE_INIT(4, 4, 32, 256, 256),
  1, &expanded_conv_project_weights_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_project_bias, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &expanded_conv_project_bias_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  block_1_expand_weights, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_1_expand_weights_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  block_1_expand_bias, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_1_expand_bias_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  block_1_depthwise_weights, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &block_1_depthwise_weights_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  block_1_depthwise_bias, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_1_depthwise_bias_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  block_1_project_weights, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 8), AI_STRIDE_INIT(4, 4, 192, 1536, 1536),
  1, &block_1_project_weights_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  block_1_project_bias, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &block_1_project_bias_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  block_2_expand_weights, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_2_expand_weights_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  block_2_expand_bias, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_2_expand_bias_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  block_2_depthwise_weights, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &block_2_depthwise_weights_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  block_2_depthwise_bias, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_2_depthwise_bias_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  block_2_project_weights, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 8), AI_STRIDE_INIT(4, 4, 192, 1536, 1536),
  1, &block_2_project_weights_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  block_2_project_bias, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &block_2_project_bias_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  block_3_expand_weights, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_3_expand_weights_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  block_3_expand_bias, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_3_expand_bias_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  block_3_depthwise_weights, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &block_3_depthwise_weights_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  block_3_depthwise_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_3_depthwise_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  block_3_project_weights, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 8), AI_STRIDE_INIT(4, 4, 192, 1536, 1536),
  1, &block_3_project_weights_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  block_3_project_bias, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &block_3_project_bias_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  block_4_expand_weights, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_4_expand_weights_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  block_4_expand_bias, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_4_expand_bias_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  block_4_depthwise_weights, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &block_4_depthwise_weights_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  block_4_depthwise_bias, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_4_depthwise_bias_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  block_4_project_weights, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 8), AI_STRIDE_INIT(4, 4, 192, 1536, 1536),
  1, &block_4_project_weights_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  block_4_project_bias, AI_STATIC,
  25, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &block_4_project_bias_array, NULL)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  block_5_expand_weights, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_5_expand_weights_array, NULL)

/* Tensor #27 */
AI_TENSOR_OBJ_DECLARE(
  block_5_expand_bias, AI_STATIC,
  27, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_5_expand_bias_array, NULL)

/* Tensor #28 */
AI_TENSOR_OBJ_DECLARE(
  block_5_depthwise_weights, AI_STATIC,
  28, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 48), AI_STRIDE_INIT(4, 1, 48, 48, 48),
  1, &block_5_depthwise_weights_array, NULL)

/* Tensor #29 */
AI_TENSOR_OBJ_DECLARE(
  block_5_depthwise_bias, AI_STATIC,
  29, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_5_depthwise_bias_array, NULL)

/* Tensor #30 */
AI_TENSOR_OBJ_DECLARE(
  block_5_project_weights, AI_STATIC,
  30, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 8), AI_STRIDE_INIT(4, 4, 192, 1536, 1536),
  1, &block_5_project_weights_array, NULL)

/* Tensor #31 */
AI_TENSOR_OBJ_DECLARE(
  block_5_project_bias, AI_STATIC,
  31, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &block_5_project_bias_array, NULL)

/* Tensor #32 */
AI_TENSOR_OBJ_DECLARE(
  block_6_expand_weights, AI_STATIC,
  32, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 48), AI_STRIDE_INIT(4, 4, 32, 1536, 1536),
  1, &block_6_expand_weights_array, NULL)

/* Tensor #33 */
AI_TENSOR_OBJ_DECLARE(
  block_6_expand_bias, AI_STATIC,
  33, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 1, 1), AI_STRIDE_INIT(4, 4, 4, 192, 192),
  1, &block_6_expand_bias_array, NULL)

/* Tensor #34 */
AI_TENSOR_OBJ_DECLARE(
  head_conv2d_weights, AI_STATIC,
  34, 0x0,
  AI_SHAPE_INIT(4, 48, 1, 1, 32), AI_STRIDE_INIT(4, 4, 192, 6144, 6144),
  1, &head_conv2d_weights_array, NULL)

/* Tensor #35 */
AI_TENSOR_OBJ_DECLARE(
  head_conv2d_bias, AI_STATIC,
  35, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &head_conv2d_bias_array, NULL)

/* Tensor #36 */
AI_TENSOR_OBJ_DECLARE(
  logits_weights, AI_STATIC,
  36, 0x0,
  AI_SHAPE_INIT(4, 32, 1, 1, 2), AI_STRIDE_INIT(4, 4, 128, 256, 256),
  1, &logits_weights_array, NULL)

/* Tensor #37 */
AI_TENSOR_OBJ_DECLARE(
  logits_bias, AI_STATIC,
  37, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &logits_bias_array, NULL)

/* Tensor #38 */
AI_TENSOR_OBJ_DECLARE(
  input_1_output, AI_STATIC,
  38, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 65, 65), AI_STRIDE_INIT(4, 4, 4, 4, 260),
  1, &input_1_output_array, NULL)

/* Tensor #39 */
AI_TENSOR_OBJ_DECLARE(
  Conv1_output, AI_STATIC,
  39, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &Conv1_output_array, NULL)

/* Tensor #40 */
AI_TENSOR_OBJ_DECLARE(
  Conv1_relu_output, AI_STATIC,
  40, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &Conv1_relu_output_array, NULL)

/* Tensor #41 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_depthwise_output, AI_STATIC,
  41, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &expanded_conv_depthwise_output_array, NULL)

/* Tensor #42 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_depthwise_relu_output, AI_STATIC,
  42, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &expanded_conv_depthwise_relu_output_array, NULL)

/* Tensor #43 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_project_output, AI_STATIC,
  43, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &expanded_conv_project_output_array, NULL)

/* Tensor #44 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_add_output, AI_STATIC,
  44, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 33, 33), AI_STRIDE_INIT(4, 4, 4, 32, 1056),
  1, &expanded_conv_add_output_array, NULL)

/* Tensor #45 */
AI_TENSOR_OBJ_DECLARE(
  block_1_expand_output, AI_STATIC,
  45, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 33, 33), AI_STRIDE_INIT(4, 4, 4, 192, 6336),
  1, &block_1_expand_output_array, NULL)

/* Tensor #46 */
AI_TENSOR_OBJ_DECLARE(
  block_1_expand_relu_output, AI_STATIC,
  46, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 33, 33), AI_STRIDE_INIT(4, 4, 4, 192, 6336),
  1, &block_1_expand_relu_output_array, NULL)

/* Tensor #47 */
AI_TENSOR_OBJ_DECLARE(
  block_1_depthwise_output, AI_STATIC,
  47, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_1_depthwise_output_array, NULL)

/* Tensor #48 */
AI_TENSOR_OBJ_DECLARE(
  block_1_depthwise_relu_output, AI_STATIC,
  48, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_1_depthwise_relu_output_array, NULL)

/* Tensor #49 */
AI_TENSOR_OBJ_DECLARE(
  block_1_project_output, AI_STATIC,
  49, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 17, 17), AI_STRIDE_INIT(4, 4, 4, 32, 544),
  1, &block_1_project_output_array, NULL)

/* Tensor #50 */
AI_TENSOR_OBJ_DECLARE(
  block_2_expand_output, AI_STATIC,
  50, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_2_expand_output_array, NULL)

/* Tensor #51 */
AI_TENSOR_OBJ_DECLARE(
  block_2_expand_relu_output, AI_STATIC,
  51, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_2_expand_relu_output_array, NULL)

/* Tensor #52 */
AI_TENSOR_OBJ_DECLARE(
  block_2_depthwise_output, AI_STATIC,
  52, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_2_depthwise_output_array, NULL)

/* Tensor #53 */
AI_TENSOR_OBJ_DECLARE(
  block_2_depthwise_relu_output, AI_STATIC,
  53, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_2_depthwise_relu_output_array, NULL)

/* Tensor #54 */
AI_TENSOR_OBJ_DECLARE(
  block_2_project_output, AI_STATIC,
  54, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 17, 17), AI_STRIDE_INIT(4, 4, 4, 32, 544),
  1, &block_2_project_output_array, NULL)

/* Tensor #55 */
AI_TENSOR_OBJ_DECLARE(
  block_2_add_output, AI_STATIC,
  55, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 17, 17), AI_STRIDE_INIT(4, 4, 4, 32, 544),
  1, &block_2_add_output_array, NULL)

/* Tensor #56 */
AI_TENSOR_OBJ_DECLARE(
  block_3_expand_output, AI_STATIC,
  56, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_3_expand_output_array, NULL)

/* Tensor #57 */
AI_TENSOR_OBJ_DECLARE(
  block_3_expand_relu_output, AI_STATIC,
  57, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 17, 17), AI_STRIDE_INIT(4, 4, 4, 192, 3264),
  1, &block_3_expand_relu_output_array, NULL)

/* Tensor #58 */
AI_TENSOR_OBJ_DECLARE(
  block_3_depthwise_output, AI_STATIC,
  58, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_3_depthwise_output_array, NULL)

/* Tensor #59 */
AI_TENSOR_OBJ_DECLARE(
  block_3_depthwise_relu_output, AI_STATIC,
  59, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_3_depthwise_relu_output_array, NULL)

/* Tensor #60 */
AI_TENSOR_OBJ_DECLARE(
  block_3_project_output, AI_STATIC,
  60, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 9, 9), AI_STRIDE_INIT(4, 4, 4, 32, 288),
  1, &block_3_project_output_array, NULL)

/* Tensor #61 */
AI_TENSOR_OBJ_DECLARE(
  block_4_expand_output, AI_STATIC,
  61, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_4_expand_output_array, NULL)

/* Tensor #62 */
AI_TENSOR_OBJ_DECLARE(
  block_4_expand_relu_output, AI_STATIC,
  62, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_4_expand_relu_output_array, NULL)

/* Tensor #63 */
AI_TENSOR_OBJ_DECLARE(
  block_4_depthwise_output, AI_STATIC,
  63, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_4_depthwise_output_array, NULL)

/* Tensor #64 */
AI_TENSOR_OBJ_DECLARE(
  block_4_depthwise_relu_output, AI_STATIC,
  64, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_4_depthwise_relu_output_array, NULL)

/* Tensor #65 */
AI_TENSOR_OBJ_DECLARE(
  block_4_project_output, AI_STATIC,
  65, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 9, 9), AI_STRIDE_INIT(4, 4, 4, 32, 288),
  1, &block_4_project_output_array, NULL)

/* Tensor #66 */
AI_TENSOR_OBJ_DECLARE(
  block_4_add_output, AI_STATIC,
  66, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 9, 9), AI_STRIDE_INIT(4, 4, 4, 32, 288),
  1, &block_4_add_output_array, NULL)

/* Tensor #67 */
AI_TENSOR_OBJ_DECLARE(
  block_5_expand_output, AI_STATIC,
  67, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_5_expand_output_array, NULL)

/* Tensor #68 */
AI_TENSOR_OBJ_DECLARE(
  block_5_expand_relu_output, AI_STATIC,
  68, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_5_expand_relu_output_array, NULL)

/* Tensor #69 */
AI_TENSOR_OBJ_DECLARE(
  block_5_depthwise_output, AI_STATIC,
  69, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_5_depthwise_output_array, NULL)

/* Tensor #70 */
AI_TENSOR_OBJ_DECLARE(
  block_5_depthwise_relu_output, AI_STATIC,
  70, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_5_depthwise_relu_output_array, NULL)

/* Tensor #71 */
AI_TENSOR_OBJ_DECLARE(
  block_5_project_output, AI_STATIC,
  71, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 9, 9), AI_STRIDE_INIT(4, 4, 4, 32, 288),
  1, &block_5_project_output_array, NULL)

/* Tensor #72 */
AI_TENSOR_OBJ_DECLARE(
  block_5_add_output, AI_STATIC,
  72, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 9, 9), AI_STRIDE_INIT(4, 4, 4, 32, 288),
  1, &block_5_add_output_array, NULL)

/* Tensor #73 */
AI_TENSOR_OBJ_DECLARE(
  block_6_expand_output, AI_STATIC,
  73, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_6_expand_output_array, NULL)

/* Tensor #74 */
AI_TENSOR_OBJ_DECLARE(
  block_6_expand_relu_output, AI_STATIC,
  74, 0x0,
  AI_SHAPE_INIT(4, 1, 48, 9, 9), AI_STRIDE_INIT(4, 4, 4, 192, 1728),
  1, &block_6_expand_relu_output_array, NULL)

/* Tensor #75 */
AI_TENSOR_OBJ_DECLARE(
  head_conv2d_output, AI_STATIC,
  75, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 9, 9), AI_STRIDE_INIT(4, 4, 4, 128, 1152),
  1, &head_conv2d_output_array, NULL)

/* Tensor #76 */
AI_TENSOR_OBJ_DECLARE(
  head_output, AI_STATIC,
  76, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 9, 9), AI_STRIDE_INIT(4, 4, 4, 128, 1152),
  1, &head_output_array, NULL)

/* Tensor #77 */
AI_TENSOR_OBJ_DECLARE(
  logits_output, AI_STATIC,
  77, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 9, 9), AI_STRIDE_INIT(4, 4, 4, 8, 72),
  1, &logits_output_array, NULL)

/* Tensor #78 */
AI_TENSOR_OBJ_DECLARE(
  softmax_output, AI_STATIC,
  78, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 9, 9), AI_STRIDE_INIT(4, 4, 4, 8, 72),
  1, &softmax_output_array, NULL)

/* Tensor #79 */
AI_TENSOR_OBJ_DECLARE(
  Conv1_weights, AI_STATIC,
  79, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 8), AI_STRIDE_INIT(4, 4, 4, 32, 96),
  1, &Conv1_weights_array, NULL)

/* Tensor #80 */
AI_TENSOR_OBJ_DECLARE(
  Conv1_bias, AI_STATIC,
  80, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &Conv1_bias_array, NULL)

/* Tensor #81 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_depthwise_weights, AI_STATIC,
  81, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 8), AI_STRIDE_INIT(4, 1, 8, 8, 8),
  1, &expanded_conv_depthwise_weights_array, NULL)

/* Tensor #82 */
AI_TENSOR_OBJ_DECLARE(
  expanded_conv_depthwise_bias, AI_STATIC,
  82, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &expanded_conv_depthwise_bias_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  softmax_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &logits_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &softmax_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  softmax_layer, 60,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &softmax_chain,
  NULL, &softmax_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  logits_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &head_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &logits_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &logits_weights, &logits_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  logits_layer, 59,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &logits_chain,
  NULL, &softmax_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  head_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &head_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &head_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  head_layer, 58,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &head_chain,
  NULL, &logits_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  head_conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_6_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &head_conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &head_conv2d_weights, &head_conv2d_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  head_conv2d_layer, 58,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &head_conv2d_chain,
  NULL, &head_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_6_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_6_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_6_expand_relu_nl_params_data, block_6_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_6_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_6_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_6_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_6_expand_relu_layer, 57,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_6_expand_relu_chain,
  NULL, &head_conv2d_layer, AI_STATIC, 
  .nl_params = &block_6_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_6_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_add_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_6_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_6_expand_weights, &block_6_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_6_expand_layer, 56,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_6_expand_chain,
  NULL, &block_6_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_add_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &block_4_add_output, &block_5_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_add_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_add_layer, 54,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &block_5_add_chain,
  NULL, &block_6_expand_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_5_project_weights, &block_5_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_project_layer, 53,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_5_project_chain,
  NULL, &block_5_add_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_5_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_5_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_5_depthwise_relu_nl_params_data, block_5_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_depthwise_relu_layer, 51,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_5_depthwise_relu_chain,
  NULL, &block_5_project_layer, AI_STATIC, 
  .nl_params = &block_5_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_5_depthwise_weights, &block_5_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_depthwise_layer, 50,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &block_5_depthwise_chain,
  NULL, &block_5_depthwise_relu_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_5_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_5_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_5_expand_relu_nl_params_data, block_5_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_expand_relu_layer, 48,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_5_expand_relu_chain,
  NULL, &block_5_depthwise_layer, AI_STATIC, 
  .nl_params = &block_5_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_5_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_add_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_5_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_5_expand_weights, &block_5_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_5_expand_layer, 47,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_5_expand_chain,
  NULL, &block_5_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_add_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &block_3_project_output, &block_4_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_add_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_add_layer, 45,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &block_4_add_chain,
  NULL, &block_5_expand_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_4_project_weights, &block_4_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_project_layer, 44,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_4_project_chain,
  NULL, &block_4_add_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_4_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_4_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_4_depthwise_relu_nl_params_data, block_4_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_depthwise_relu_layer, 42,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_4_depthwise_relu_chain,
  NULL, &block_4_project_layer, AI_STATIC, 
  .nl_params = &block_4_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_4_depthwise_weights, &block_4_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_depthwise_layer, 41,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &block_4_depthwise_chain,
  NULL, &block_4_depthwise_relu_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_4_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_4_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_4_expand_relu_nl_params_data, block_4_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_expand_relu_layer, 39,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_4_expand_relu_chain,
  NULL, &block_4_depthwise_layer, AI_STATIC, 
  .nl_params = &block_4_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_4_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_4_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_4_expand_weights, &block_4_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_4_expand_layer, 38,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_4_expand_chain,
  NULL, &block_4_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_3_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_3_project_weights, &block_3_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_3_project_layer, 36,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_3_project_chain,
  NULL, &block_4_expand_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_3_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_3_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_3_depthwise_relu_nl_params_data, block_3_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_3_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_3_depthwise_relu_layer, 34,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_3_depthwise_relu_chain,
  NULL, &block_3_project_layer, AI_STATIC, 
  .nl_params = &block_3_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_3_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_3_depthwise_weights, &block_3_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_3_depthwise_layer, 31,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &block_3_depthwise_chain,
  NULL, &block_3_depthwise_relu_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_3_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_3_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_3_expand_relu_nl_params_data, block_3_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_3_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_3_expand_relu_layer, 30,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_3_expand_relu_chain,
  NULL, &block_3_depthwise_layer, AI_STATIC, 
  .nl_params = &block_3_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_3_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_add_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_3_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_3_expand_weights, &block_3_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_3_expand_layer, 29,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_3_expand_chain,
  NULL, &block_3_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_add_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &block_1_project_output, &block_2_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_add_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_add_layer, 27,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &block_2_add_chain,
  NULL, &block_3_expand_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_2_project_weights, &block_2_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_project_layer, 26,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_2_project_chain,
  NULL, &block_2_add_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_2_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_2_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_2_depthwise_relu_nl_params_data, block_2_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_depthwise_relu_layer, 24,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_2_depthwise_relu_chain,
  NULL, &block_2_project_layer, AI_STATIC, 
  .nl_params = &block_2_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_2_depthwise_weights, &block_2_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_depthwise_layer, 23,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &block_2_depthwise_chain,
  NULL, &block_2_depthwise_relu_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_2_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_2_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_2_expand_relu_nl_params_data, block_2_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_expand_relu_layer, 21,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_2_expand_relu_chain,
  NULL, &block_2_depthwise_layer, AI_STATIC, 
  .nl_params = &block_2_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_2_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_2_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_2_expand_weights, &block_2_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_2_expand_layer, 20,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_2_expand_chain,
  NULL, &block_2_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_1_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_1_project_weights, &block_1_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_1_project_layer, 18,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_1_project_chain,
  NULL, &block_2_expand_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_1_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_1_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_1_depthwise_relu_nl_params_data, block_1_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_1_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_1_depthwise_relu_layer, 16,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_1_depthwise_relu_chain,
  NULL, &block_1_project_layer, AI_STATIC, 
  .nl_params = &block_1_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_1_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_expand_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_1_depthwise_weights, &block_1_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_1_depthwise_layer, 13,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &block_1_depthwise_chain,
  NULL, &block_1_depthwise_relu_layer, AI_STATIC, 
  .groups = 48, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float block_1_expand_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    block_1_expand_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    block_1_expand_relu_nl_params_data, block_1_expand_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_1_expand_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_expand_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_1_expand_relu_layer, 12,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &block_1_expand_relu_chain,
  NULL, &block_1_depthwise_layer, AI_STATIC, 
  .nl_params = &block_1_expand_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  block_1_expand_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_add_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &block_1_expand_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &block_1_expand_weights, &block_1_expand_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  block_1_expand_layer, 11,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &block_1_expand_chain,
  NULL, &block_1_expand_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  expanded_conv_add_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &Conv1_relu_output, &expanded_conv_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_add_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  expanded_conv_add_layer, 9,
  ELTWISE_TYPE, 0x0, NULL,
  eltwise, forward_eltwise,
  &expanded_conv_add_chain,
  NULL, &block_1_expand_layer, AI_STATIC, 
  .operation = ai_sum_f32, 
  .buffer_operation = ai_sum_buffer_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  expanded_conv_project_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_project_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &expanded_conv_project_weights, &expanded_conv_project_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  expanded_conv_project_layer, 8,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &expanded_conv_project_chain,
  NULL, &expanded_conv_add_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 0, 0, 0, 0), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float expanded_conv_depthwise_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    expanded_conv_depthwise_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    expanded_conv_depthwise_relu_nl_params_data, expanded_conv_depthwise_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  expanded_conv_depthwise_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_depthwise_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  expanded_conv_depthwise_relu_layer, 6,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &expanded_conv_depthwise_relu_chain,
  NULL, &expanded_conv_project_layer, AI_STATIC, 
  .nl_params = &expanded_conv_depthwise_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  expanded_conv_depthwise_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Conv1_relu_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &expanded_conv_depthwise_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &expanded_conv_depthwise_weights, &expanded_conv_depthwise_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  expanded_conv_depthwise_layer, 5,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_dw_if32of32wf32,
  &expanded_conv_depthwise_chain,
  NULL, &expanded_conv_depthwise_relu_layer, AI_STATIC, 
  .groups = 8, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


AI_STATIC_CONST ai_float Conv1_relu_nl_params_data[] = { 0, 6.0 };
AI_ARRAY_OBJ_DECLARE(
    Conv1_relu_nl_params, AI_ARRAY_FORMAT_FLOAT,
    Conv1_relu_nl_params_data, Conv1_relu_nl_params_data, 2, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  Conv1_relu_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Conv1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Conv1_relu_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Conv1_relu_layer, 3,
  NL_TYPE, 0x0, NULL,
  nl, forward_clip,
  &Conv1_relu_chain,
  NULL, &expanded_conv_depthwise_layer, AI_STATIC, 
  .nl_params = &Conv1_relu_nl_params, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  Conv1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &Conv1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &Conv1_weights, &Conv1_bias, NULL),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  Conv1_layer, 2,
  CONV2D_TYPE, 0x0, NULL,
  conv2d, forward_conv2d_if32of32wf32,
  &Conv1_chain,
  NULL, &Conv1_relu_layer, AI_STATIC, 
  .groups = 1, 
  .filter_stride = AI_SHAPE_2D_INIT(2, 2), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .in_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_SAME, 
  .out_ch_format = AI_LAYER_FORMAT_CHANNEL_LAST_VALID, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 35272, 1, 1),
    35272, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 224160, 1, 1),
    224160, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &softmax_output),
  &Conv1_layer, 0xdf64a17e, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 35272, 1, 1),
      35272, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 224160, 1, 1),
      224160, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_1_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &softmax_output),
  &Conv1_layer, 0xdf64a17e, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_1_output_array.data = AI_PTR(g_network_activations_map[0] + 137564);
    input_1_output_array.data_start = AI_PTR(g_network_activations_map[0] + 137564);
    Conv1_output_array.data = AI_PTR(g_network_activations_map[0] + 154464);
    Conv1_output_array.data_start = AI_PTR(g_network_activations_map[0] + 154464);
    Conv1_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 189312);
    Conv1_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 189312);
    expanded_conv_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 154464);
    expanded_conv_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 154464);
    expanded_conv_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 119616);
    expanded_conv_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 119616);
    expanded_conv_project_output_array.data = AI_PTR(g_network_activations_map[0] + 118560);
    expanded_conv_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 118560);
    expanded_conv_add_output_array.data = AI_PTR(g_network_activations_map[0] + 189312);
    expanded_conv_add_output_array.data_start = AI_PTR(g_network_activations_map[0] + 189312);
    block_1_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 3456);
    block_1_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 3456);
    block_1_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 3456);
    block_1_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 3456);
    block_1_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    block_1_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    block_1_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 55488);
    block_1_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 55488);
    block_1_project_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    block_1_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    block_2_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 64736);
    block_2_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 64736);
    block_2_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 64736);
    block_2_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 64736);
    block_2_project_output_array.data = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 9248);
    block_2_add_output_array.data = AI_PTR(g_network_activations_map[0] + 18496);
    block_2_add_output_array.data_start = AI_PTR(g_network_activations_map[0] + 18496);
    block_3_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 27744);
    block_3_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 27744);
    block_3_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 83232);
    block_3_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 83232);
    block_3_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    block_3_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    block_3_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 15552);
    block_3_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 15552);
    block_3_project_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    block_3_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    block_4_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 18144);
    block_4_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 18144);
    block_4_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 18144);
    block_4_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 18144);
    block_4_project_output_array.data = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 2592);
    block_4_add_output_array.data = AI_PTR(g_network_activations_map[0] + 5184);
    block_4_add_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5184);
    block_5_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 7776);
    block_5_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 7776);
    block_5_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 23328);
    block_5_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 23328);
    block_5_depthwise_output_array.data = AI_PTR(g_network_activations_map[0] + 7776);
    block_5_depthwise_output_array.data_start = AI_PTR(g_network_activations_map[0] + 7776);
    block_5_depthwise_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 23328);
    block_5_depthwise_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 23328);
    block_5_project_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    block_5_project_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    block_5_add_output_array.data = AI_PTR(g_network_activations_map[0] + 2592);
    block_5_add_output_array.data_start = AI_PTR(g_network_activations_map[0] + 2592);
    block_6_expand_output_array.data = AI_PTR(g_network_activations_map[0] + 5184);
    block_6_expand_output_array.data_start = AI_PTR(g_network_activations_map[0] + 5184);
    block_6_expand_relu_output_array.data = AI_PTR(g_network_activations_map[0] + 20736);
    block_6_expand_relu_output_array.data_start = AI_PTR(g_network_activations_map[0] + 20736);
    head_conv2d_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    head_conv2d_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    head_output_array.data = AI_PTR(g_network_activations_map[0] + 10368);
    head_output_array.data_start = AI_PTR(g_network_activations_map[0] + 10368);
    logits_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    logits_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    softmax_output_array.data = AI_PTR(g_network_activations_map[0] + 648);
    softmax_output_array.data_start = AI_PTR(g_network_activations_map[0] + 648);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    Conv1_weights_array.format |= AI_FMT_FLAG_CONST;
    Conv1_weights_array.data = AI_PTR(g_network_weights_map[0] + 0);
    Conv1_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    Conv1_bias_array.format |= AI_FMT_FLAG_CONST;
    Conv1_bias_array.data = AI_PTR(g_network_weights_map[0] + 288);
    Conv1_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 288);
    expanded_conv_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    expanded_conv_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 320);
    expanded_conv_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 320);
    expanded_conv_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    expanded_conv_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 608);
    expanded_conv_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 608);
    expanded_conv_project_weights_array.format |= AI_FMT_FLAG_CONST;
    expanded_conv_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 640);
    expanded_conv_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 640);
    expanded_conv_project_bias_array.format |= AI_FMT_FLAG_CONST;
    expanded_conv_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 896);
    expanded_conv_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 896);
    block_1_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_1_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 928);
    block_1_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 928);
    block_1_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_1_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 2464);
    block_1_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 2464);
    block_1_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    block_1_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 2656);
    block_1_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 2656);
    block_1_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    block_1_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 4384);
    block_1_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 4384);
    block_1_project_weights_array.format |= AI_FMT_FLAG_CONST;
    block_1_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 4576);
    block_1_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 4576);
    block_1_project_bias_array.format |= AI_FMT_FLAG_CONST;
    block_1_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 6112);
    block_1_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 6112);
    block_2_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_2_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 6144);
    block_2_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 6144);
    block_2_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_2_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 7680);
    block_2_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 7680);
    block_2_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    block_2_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 7872);
    block_2_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 7872);
    block_2_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    block_2_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 9600);
    block_2_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 9600);
    block_2_project_weights_array.format |= AI_FMT_FLAG_CONST;
    block_2_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 9792);
    block_2_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 9792);
    block_2_project_bias_array.format |= AI_FMT_FLAG_CONST;
    block_2_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 11328);
    block_2_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 11328);
    block_3_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_3_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 11360);
    block_3_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 11360);
    block_3_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_3_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 12896);
    block_3_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 12896);
    block_3_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    block_3_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 13088);
    block_3_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 13088);
    block_3_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    block_3_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 14816);
    block_3_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 14816);
    block_3_project_weights_array.format |= AI_FMT_FLAG_CONST;
    block_3_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 15008);
    block_3_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 15008);
    block_3_project_bias_array.format |= AI_FMT_FLAG_CONST;
    block_3_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 16544);
    block_3_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 16544);
    block_4_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_4_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 16576);
    block_4_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 16576);
    block_4_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_4_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 18112);
    block_4_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 18112);
    block_4_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    block_4_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 18304);
    block_4_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 18304);
    block_4_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    block_4_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 20032);
    block_4_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 20032);
    block_4_project_weights_array.format |= AI_FMT_FLAG_CONST;
    block_4_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 20224);
    block_4_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 20224);
    block_4_project_bias_array.format |= AI_FMT_FLAG_CONST;
    block_4_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 21760);
    block_4_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 21760);
    block_5_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_5_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 21792);
    block_5_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 21792);
    block_5_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_5_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 23328);
    block_5_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 23328);
    block_5_depthwise_weights_array.format |= AI_FMT_FLAG_CONST;
    block_5_depthwise_weights_array.data = AI_PTR(g_network_weights_map[0] + 23520);
    block_5_depthwise_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 23520);
    block_5_depthwise_bias_array.format |= AI_FMT_FLAG_CONST;
    block_5_depthwise_bias_array.data = AI_PTR(g_network_weights_map[0] + 25248);
    block_5_depthwise_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 25248);
    block_5_project_weights_array.format |= AI_FMT_FLAG_CONST;
    block_5_project_weights_array.data = AI_PTR(g_network_weights_map[0] + 25440);
    block_5_project_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 25440);
    block_5_project_bias_array.format |= AI_FMT_FLAG_CONST;
    block_5_project_bias_array.data = AI_PTR(g_network_weights_map[0] + 26976);
    block_5_project_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 26976);
    block_6_expand_weights_array.format |= AI_FMT_FLAG_CONST;
    block_6_expand_weights_array.data = AI_PTR(g_network_weights_map[0] + 27008);
    block_6_expand_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 27008);
    block_6_expand_bias_array.format |= AI_FMT_FLAG_CONST;
    block_6_expand_bias_array.data = AI_PTR(g_network_weights_map[0] + 28544);
    block_6_expand_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 28544);
    head_conv2d_weights_array.format |= AI_FMT_FLAG_CONST;
    head_conv2d_weights_array.data = AI_PTR(g_network_weights_map[0] + 28736);
    head_conv2d_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 28736);
    head_conv2d_bias_array.format |= AI_FMT_FLAG_CONST;
    head_conv2d_bias_array.data = AI_PTR(g_network_weights_map[0] + 34880);
    head_conv2d_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 34880);
    logits_weights_array.format |= AI_FMT_FLAG_CONST;
    logits_weights_array.data = AI_PTR(g_network_weights_map[0] + 35008);
    logits_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 35008);
    logits_bias_array.format |= AI_FMT_FLAG_CONST;
    logits_bias_array.data = AI_PTR(g_network_weights_map[0] + 35264);
    logits_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 35264);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2074480,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xdf64a17e,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2074480,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xdf64a17e,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_network_data_params_get(&params) != true) {
    err = ai_network_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_init(*network, &params) != true) {
    err = ai_network_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

