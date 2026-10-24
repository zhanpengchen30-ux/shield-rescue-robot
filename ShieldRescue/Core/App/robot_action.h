// robot_action.h
#ifndef __ROBOT_ACTION_H
#define __ROBOT_ACTION_H

#include "main.h"

// 机械臂一键动作/姿态 ID
#define POSE_IDLE               0
#define POSE_SCAN_QR            1
#define POSE_DETECT_EXPLOSIVE   2  // 识别爆炸物姿态
#define POSE_GRAB_EXPLOSIVE     3
#define POSE_DETECT_BUCKET      4  // 识别防爆桶姿态
#define POSE_PLACE_EXPLOSIVE    5
#define POSE_HIT_TARGET         6
#define POSE_DETECT_HOSTAGE     7  // 识别人质姿态
#define POSE_GRAB_HOSTAGE       8
// 寻路/循迹时云台偏转姿态 ID
#define POSE_LINE_TRACK_CENTER  9  // 云台居中寻路
#define POSE_LINE_TRACK_LEFT    10 // 云台偏左寻路
#define POSE_LINE_TRACK_RIGHT   11 // 云台偏右寻路

void Robot_Action(uint8_t pose_id);

#endif
