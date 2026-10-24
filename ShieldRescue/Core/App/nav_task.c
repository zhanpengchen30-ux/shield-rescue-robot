#include "cmsis_os.h"
#include "global_state.h"
#include "robot_action.h"
#include "bsp_motor.h"
#include "pid.h" 
#include "bsp_laser.h"

extern float target_speed[4];  
float g_current_speed[4] = {0};
extern void Chassis_SetVelocity(int16_t vx, int16_t vy, int16_t wz);  
extern int16_t Get_Motor_Speed(uint8_t motor_id);

PID_Controller pid_motor[4]; 
PID_Controller pid_align;    


void Nav_Task_Init(void) 
{
    int i; 
    for(i = 0; i < 4; i++) { 
        PID_Init(&pid_motor[i],30.0f, 0.5f, 0.5f, 500.0f, 1000.0f);
    }
    PID_Init(&pid_align, 0.5f, 0.01f, 0.05f, 300.0f, 150.0f);
}

/**
 * @brief  判断当前机械臂状态是否需要强制锁死底盘
 */
bool Is_Chassis_Locked(ArmState_t state)
{
    switch(state)
    {
        case ARM_STATE_SCAN_QR:           
        case ARM_STATE_GRAB_EXPLOSIVE:    
        case ARM_STATE_PLACE_EXPLOSIVE:   
        case ARM_STATE_HIT_TARGET:        
        case ARM_STATE_GRAB_HOSTAGE:      
            return true; 
        
        case ARM_STATE_IDLE:              
        case ARM_STATE_LINE_TRACK_CENTER: 
        case ARM_STATE_LINE_TRACK_LEFT:   
        case ARM_STATE_LINE_TRACK_RIGHT:  
        case ARM_STATE_DETECT_EXPLOSIVE:  
        case ARM_STATE_DETECT_BUCKET:     
        case ARM_STATE_DETECT_HOSTAGE:    
        default:
            return false; 
    }
}

/**
 * @brief  导航控制任务（主运行线程）
 */
void StartNavTask(void * argument) 
{
    Nav_Task_Init(); 
    
    float vx = 0, vy = 0, w = 0;          
    ArmState_t last_arm_state = ARM_STATE_IDLE; 
    uint32_t laser_countdown = 0;
	
    for(;;) 
    {

        if (g_laser_trigger)
        {
            g_laser_trigger = false; 
            Laser_On();
            laser_countdown = 100;   
        }
        if (laser_countdown > 0)
        {
            laser_countdown--;
            if (laser_countdown == 0)
            {
                Laser_Off(); 
            }
        }

        /* ================== 通道一：机械臂/云台动作独立运行 ================== */
        if (g_arm_state != last_arm_state)
        {
            switch(g_arm_state)
            {
                case ARM_STATE_IDLE:              Robot_Action(POSE_IDLE);              break;
                case ARM_STATE_LINE_TRACK_CENTER: Robot_Action(POSE_LINE_TRACK_CENTER); break; 
                case ARM_STATE_LINE_TRACK_LEFT:   Robot_Action(POSE_LINE_TRACK_LEFT);   break; 
                case ARM_STATE_LINE_TRACK_RIGHT:  Robot_Action(POSE_LINE_TRACK_RIGHT);  break; 
                case ARM_STATE_SCAN_QR:           Robot_Action(POSE_SCAN_QR);           break; 
                case ARM_STATE_DETECT_EXPLOSIVE:  Robot_Action(POSE_DETECT_EXPLOSIVE);  break; 
                case ARM_STATE_GRAB_EXPLOSIVE:    Robot_Action(POSE_GRAB_EXPLOSIVE);    break; 
                case ARM_STATE_DETECT_BUCKET:     Robot_Action(POSE_DETECT_BUCKET);     break; 
                case ARM_STATE_PLACE_EXPLOSIVE:   Robot_Action(POSE_PLACE_EXPLOSIVE);   break; 
                case ARM_STATE_HIT_TARGET:        Robot_Action(POSE_HIT_TARGET);        break; 
                case ARM_STATE_DETECT_HOSTAGE:    Robot_Action(POSE_DETECT_HOSTAGE);    break; 
                case ARM_STATE_GRAB_HOSTAGE:      Robot_Action(POSE_GRAB_HOSTAGE);      break; 
                default: break;
            }
            last_arm_state = g_arm_state; 
        }

/* ================== 通道二：底盘运动独立控制 ================== */
        bool chassis_locked = Is_Chassis_Locked(g_arm_state);

        if (chassis_locked || (g_chassis_state == CHASSIS_STATE_IDLE))
        {
            vx = 0; vy = 0; w = 0;
            for(int i = 0; i < 4; i++) {
                pid_motor[i].integral = 0;
                pid_motor[i].prev_error = 0;
            }
        }
        else
        {
            switch (g_chassis_state) 
            {
                case CHASSIS_STATE_IDLE:
                    vx = 0; vy = 0; w = 0; 
                    break;
                case CHASSIS_STATE_FORWARD:
                    vx = 50; vy = 0; w = 0; 
                    break;
                case CHASSIS_STATE_FORWARD_SLOW:
                    vx = 30; vy = 0; w = 0;  
                    break;
                case CHASSIS_STATE_MOVE_LEFT:
                    vx = 0; vy = -100; w = 0; 
                    break;
                case CHASSIS_STATE_MOVE_RIGHT:
                    vx = 0; vy = 100; w = 0;  
                    break;
                case CHASSIS_STATE_SPIN_LEFT:
                    vx = 0; vy = 0; w = -50;  
                    break;
                case CHASSIS_STATE_SPIN_RIGHT:
                    vx = 0; vy = 0; w = 50; 
                    break;
            }
        }

        Chassis_SetVelocity(vx, vy, w);

        float current_speed[4] = {0};
        for(int i = 0; i < 4; i++) {
            current_speed[i] = (float)Get_Motor_Speed(i);
            g_current_speed[i] = current_speed[i];
        }
        
        float target_pulse[4] = {0};
        for(int i = 0; i < 4; i++) {
            target_pulse[i] = target_speed[i] * (18.0f / 1000.0f); 
        }

        int16_t output[4] = {0};
        for(int i = 0; i < 4; i++) {

            output[i] = (int16_t)PID_Compute(&pid_motor[i], target_pulse[i], current_speed[i]);
        }
        
//        Motor_SetSpeed(output[0], output[1], output[2], output[3]);
				osDelay(10); 
			}
		}
	