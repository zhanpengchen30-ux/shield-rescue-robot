#include "bsp_usart.h"
#include "usart.h"        
#include "global_state.h"

uint8_t rx_byte = 0; 
extern UART_HandleTypeDef huart2;

void BSP_UART_Init(void)
{
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

/**
 * @brief  完全物理隔离的单字符命令解析
 */
void UART_Parse_Data(uint8_t data)
{
    switch(data) {
        //  纯底盘运动控制（仅改变底盘状态，机械臂保持原样不动）
        case 'S': Set_Chassis_State(CHASSIS_STATE_IDLE);         break; // 'S' -> Stop 停车
        case 'W': Set_Chassis_State(CHASSIS_STATE_FORWARD);      break; // 'W' -> Forward 纯快速直走
        case 'V': Set_Chassis_State(CHASSIS_STATE_FORWARD_SLOW); break; // 'V' -> Forward Slow 纯慢速直走
        case 'L': Set_Chassis_State(CHASSIS_STATE_MOVE_LEFT);    break; // 'L' -> Left 纯底盘左平移
        case 'R': Set_Chassis_State(CHASSIS_STATE_MOVE_RIGHT);   break; // 'R' -> Right 纯底盘右平移
        case 'J': Set_Chassis_State(CHASSIS_STATE_SPIN_LEFT);    break; // 'J' -> Spin Left 左自旋
        case 'K': Set_Chassis_State(CHASSIS_STATE_SPIN_RIGHT);   break; // 'K' -> Spin Right 右自旋

        // 纯机械臂/云台控制（仅改变机械臂状态，底盘保持原样不动）
        case 'F': Set_Arm_State(ARM_STATE_LINE_TRACK_CENTER);    break; // 'F' -> 云台归正前方（保障正前视野）
        case 'X': Set_Arm_State(ARM_STATE_LINE_TRACK_LEFT);      break; // 'X' -> 云台固定偏左（保障左侧视野）
        case 'Y': Set_Arm_State(ARM_STATE_LINE_TRACK_RIGHT);     break; // 'Y' -> 云台固定偏右（保障右侧视野）
        
        case 'Q': Set_Arm_State(ARM_STATE_SCAN_QR);              break; // 'Q' -> 扫码姿态
        case 'E': Set_Arm_State(ARM_STATE_DETECT_EXPLOSIVE);     break; // 'E' -> 识别爆炸物姿态
        case 'G': Set_Arm_State(ARM_STATE_GRAB_EXPLOSIVE);       break; // 'G' -> 夹取爆炸物动作
        case 'B': Set_Arm_State(ARM_STATE_DETECT_BUCKET);        break; // 'B' -> 识别防爆桶姿态
        case 'P': Set_Arm_State(ARM_STATE_PLACE_EXPLOSIVE);      break; // 'P' -> 放置爆炸物动作
        case 'T': Set_Arm_State(ARM_STATE_HIT_TARGET);           break; // 'T' -> 打靶姿态
        case 'M': Set_Arm_State(ARM_STATE_DETECT_HOSTAGE);       break; // 'M' -> 识别人质姿态
        case 'H': Set_Arm_State(ARM_STATE_GRAB_HOSTAGE);         break; // 'H' -> 夹取人质动作
        case 'I': Set_Arm_State(ARM_STATE_IDLE);                 break; // 'I' -> 恢复默认安全姿态
        case 'U': g_laser_trigger = true;                        break; // 'U' -> 独立触发 1 秒激光

        default:  break;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        UART_Parse_Data(rx_byte);                  
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1); 
    }
}

/**
 * @brief  向协同开发板发送控制指令
 * @param  cmd: 发送的控制字符（如 'O' 开启，'C' 关闭）
 */
void Send_Cmd_To_Sub_Board(uint8_t cmd)
{
    // 通过 huart2（串口2）发送 1 个字节
    HAL_UART_Transmit(&huart2, &cmd, 1, 100);
}
