// robot_action.c
#include "robot_action.h"
#include "bsp_servo.h"
#include "cmsis_os.h" 
#include "bsp_usart.h"

//static void Gripper_Send_Cmd(char cmd) {
//    //  huart2 与副板通信，发送1个字节，超时设为100ms
//    HAL_UART_Transmit(&huart2, (uint8_t *)&cmd, 1, 100);
//}


void Robot_Action(uint8_t pose_id) {
    switch(pose_id) {
        case POSE_SCAN_QR:
            // 扫码姿态（云台保持正中，相机平视）
            Servo_SetAngle(1, 90.0);   // 基座舵机1
            Servo_SetAngle(2, 90.0);   // 大臂2
            Servo_SetAngle(3, 100.0);  // 小臂3
            Servo_SetAngle(4, 90.0);   // 【手腕舵机4】保持相机水平向前看
            Send_Cmd_To_Sub_Board('O');     // 【夹爪副板】发送 'O' 信号，使爪子打开不挡视线
            break;

        case POSE_DETECT_EXPLOSIVE:
            // 识别目标姿态（大臂小臂俯下，手腕折向下看地面）
            Servo_SetAngle(1, 90.0);  
            Servo_SetAngle(2, 60.0);   // 大臂前俯
            Servo_SetAngle(3, 110.0);  // 小臂微翘
            // 【手腕舵机4补偿】因为大臂低头，手腕需要向下折（例如120度），使 K230 垂直朝下看地面
            Servo_SetAngle(4, 120.0);  
            Send_Cmd_To_Sub_Board('O');    // 【夹爪副板】张开夹爪
            break;

        case POSE_GRAB_EXPLOSIVE:
            // 夹取动作：小臂俯下 -> 副板夹紧 -> 小臂抬起
            Servo_SetAngle(3, 45.0);   // 小臂下俯至抓取高度
            osDelay(500);              
            Send_Cmd_To_Sub_Board('G');     // 【夹爪副板】发送 'G' 信号，让副板驱动爪子死死夹紧
            osDelay(500);
            Servo_SetAngle(3, 90.0);   // 抬起小臂
            break;
            
        case POSE_DETECT_BUCKET:
            // 识别防爆桶姿态
            Servo_SetAngle(1, 90.0);  
            Servo_SetAngle(2, 100.0);  // 大臂稍微后仰
            Servo_SetAngle(3, 85.0);   
            Servo_SetAngle(4, 90.0);   // 【手腕舵机4】保持 K230 平视前方寻找防爆桶
            Send_Cmd_To_Sub_Board('O');     // 【夹爪副板】保持张开
            break;

        case POSE_PLACE_EXPLOSIVE:
            // 放置动作：探向桶口 -> 丢下 -> 收回
            Servo_SetAngle(3, 60.0);   // 小臂前探
            osDelay(600);
            Send_Cmd_To_Sub_Board('O');     // 【夹爪副板】发送 'O' 信号，松开爪子
            osDelay(400);
            Servo_SetAngle(3, 90.0);   // 收回小臂
            osDelay(500);
            break;

        case POSE_HIT_TARGET:
            // 打靶姿态
            Servo_SetAngle(1, 45.0);   
            Servo_SetAngle(2, 110.0); 
            Servo_SetAngle(3, 90.0);
            Servo_SetAngle(4, 90.0);   // 手腕保持平视
            osDelay(500);
            break;

        case POSE_DETECT_HOSTAGE:
            // 识别人质姿态
            Servo_SetAngle(1, 90.0);  
            Servo_SetAngle(2, 75.0);  
            Servo_SetAngle(3, 95.0);  
            Servo_SetAngle(4, 90.0);   // 【手腕舵机4】微俯视或平视人质
            Send_Cmd_To_Sub_Board('O');     // 【夹爪副板】打开爪子
            break;

        case POSE_GRAB_HOSTAGE:
            // 夹取人质动作：小臂俯下 -> 副板夹紧 -> 小臂抬起
            Servo_SetAngle(3, 30.0);   
            osDelay(500);
            Send_Cmd_To_Sub_Board('G');     // 【夹爪副板】发送 'G' 信号夹紧
            osDelay(500);
            Servo_SetAngle(3, 90.0);   
            break;
            
        case POSE_LINE_TRACK_CENTER:
            // 正中寻路姿态（大臂前倾，手腕向下倾斜使相机看向前方地面）
            Servo_SetAngle(1, 90.0);   // 基座：保持正中
            Servo_SetAngle(2, 70.0);   // 大臂：微低头
            Servo_SetAngle(3, 100.0);  // 小臂：配合角度
            // 使 K230 视线向下倾斜（例如110度），清晰看清地面循迹线
            Servo_SetAngle(4, 110.0);  
            Send_Cmd_To_Sub_Board('O');     // 【夹爪副板】保持张开，不阻挡循迹视线
            break;

        case POSE_LINE_TRACK_LEFT:
            // 左云台寻路姿态
            Servo_SetAngle(1, 135.0);  // 基座向左偏转
            Servo_SetAngle(2, 70.0);  
            Servo_SetAngle(3, 100.0); 
            Servo_SetAngle(4, 110.0);  // 手腕微低头看地
            Send_Cmd_To_Sub_Board('O');
            break;

        case POSE_LINE_TRACK_RIGHT:
            // 右云台寻路姿态
            Servo_SetAngle(1, 45.0);   // 基座向右偏转
            Servo_SetAngle(2, 70.0);  
            Servo_SetAngle(3, 100.0); 
            Servo_SetAngle(4, 110.0);  // 手腕微低头看地
            Send_Cmd_To_Sub_Board('O');
            break;
				
        case POSE_IDLE:
        default:
            // 初始默认/待机姿态
            Servo_SetAngle(1, 90.0);
            Servo_SetAngle(2, 90.0);
            Servo_SetAngle(3, 90.0);
            Servo_SetAngle(4, 90.0);
            Send_Cmd_To_Sub_Board('O');    // 待机时夹爪默认张开
            break;
    }
}
