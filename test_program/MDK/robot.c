#include "robot.h"

_robot robot;
RC_ctrl_t* controller_callback(RC_ctrl_t* _rc);

void robot_init(void){
	float pid_params[5] = {
		10.0f, 0.5f, 1.0f, 2000.0f, 500.0f
	};	
	usart1_init(115200);
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,CAN_Mode_Normal);
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,CAN_Mode_Normal);
	ros_to_stm32_init();
	remote_control_init();

	robot.controller = controller_callback;

	robot.straight_max = chassis_straight_max;
	robot.spin_max = chassis_spin_max;
	
	PID_init(&robot.motor[0].pid_params, PID_POSITION, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[1].pid_params, PID_POSITION, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[2].pid_params, PID_POSITION, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);
	PID_init(&robot.motor[3].pid_params, PID_POSITION, pid_params[0], pid_params[1], pid_params[2], pid_params[3], pid_params[4], 500);

}
float* my_controller_cal(int _straight, int spin){
	float x,y=0;
	float vx,vy,vw;
	static float __ret[3];
	
	if(_straight == '3') 
		x = 1000;
	else if(_straight == '4') 
		x = -1000;
	else 
		x = 0;
	
	if(_straight == '1') 
		y = 1000;
	else if(_straight =='2') 
		y = -1000;
	else 
		y = 0;
	
	if(spin == '1') 
		vw = 1000;
	else if(spin == '2') 
		vw = -1000;
	else 
		vw = 0;
	
	vy = Low_pass_output(y, 0)*cos(PI/4)-Low_pass_output(x, 1)*cos(PI/4);
	vx = Low_pass_output(y, 0)*sin(PI/4)+Low_pass_output(x, 1)*sin(PI/4);
	
	__ret[0] = vy;
	__ret[1] = vx;
	__ret[2] = vw;
	return __ret;
}
float* chassis_cal(RC_ctrl_t* _rc, _robot* _bot){
	float x,y=0;
	float vx,vy,vw;
	static float ret[3];
	
	y = _rc->rc.ch[3]*_bot->straight_max/660;
	x = _rc->rc.ch[2]*_bot->straight_max/660;
	
	vy = y*cos(PI/4)-x*cos(PI/4);
	vx = y*sin(PI/4)+x*sin(PI/4);
	vw = _rc->rc.ch[0]*_bot->spin_max/660;
	
	ret[0] = vy;
	ret[1] = vx;
	ret[2] = vw;
	return ret;
}

float* ros_control_cal(float _linear, float _angle){
	// m/s תΪ rpm
	// y = ((30*a)/(pi*r))*x     y: rpm    x: m/s    a:���ٱ�   r:�־�    pi:Բ����
	float x, y;
	float vx,vy,vw;
	
	int ccc=1000;
	static float _ret[3];
	y = ((30*Motor_reduction_ratio)/(PI*Wheel_radio))*_linear*1;
 	x = 0;
	
	vy = y*cos(PI/4)-x*sin(PI/4);
	vx = y*sin(PI/4)+x*cos(PI/4);
	vw = -_angle * ccc;
	_ret[0] = vy;
	_ret[1] = vx;
	_ret[2] = vw;
	return _ret;
}


int chassis_drive(float _input[], _robot* _bot, char _switch){
	float vx,vy,vw;
	vy = _input[0];
	vx = _input[1];
	vw = _input[2];
	
	if(_switch==0){
		_bot->motor[0].target_speed=0;
		_bot->motor[2].target_speed=0;
		_bot->motor[1].target_speed=0;
		_bot->motor[3].target_speed=0;
	}
	else if(_switch==1){
		_bot->motor[0].target_speed=-vy	+ (vw);
		_bot->motor[2].target_speed=vy	+ (vw);
		_bot->motor[1].target_speed=vx	+ (vw);
		_bot->motor[3].target_speed=-vx	+ (vw);
	}
	
	PID_calc(&_bot->motor[0].pid_params, _bot->motor[0].feed_back.speed_rpm, _bot->motor[0].target_speed);
	PID_calc(&_bot->motor[1].pid_params, _bot->motor[1].feed_back.speed_rpm, _bot->motor[1].target_speed);
	PID_calc(&_bot->motor[2].pid_params, _bot->motor[2].feed_back.speed_rpm, _bot->motor[2].target_speed);
	PID_calc(&_bot->motor[3].pid_params, _bot->motor[3].feed_back.speed_rpm, _bot->motor[3].target_speed);
	
	motor_send_3508(_bot->motor[2].pid_params.out, _bot->motor[3].pid_params.out, _bot->motor[1].pid_params.out, _bot->motor[0].pid_params.out);
	return 0;
}

int chassis_control(RC_ctrl_t* _rc){

//	chassis_drive(my_controller_cal(robot.BlueTooth.straight, robot.BlueTooth.spin), &robot, 1);
	if(_rc->rc.s[0]==2 && _rc->rc.s[1]==2){
		chassis_drive(chassis_cal(robot.controller(&rc_ctrl), &robot), &robot, 0);
	}
	else if(_rc->rc.s[0]==3 && _rc->rc.s[1]==3){
		chassis_drive(chassis_cal(robot.controller(&rc_ctrl), &robot), &robot, 1);
	}
	else if(_rc->rc.s[0]==3 && _rc->rc.s[1]==1){
		chassis_drive(ros_control_cal(robot.twist_linear, robot.twist_angle), &robot, 1);
	}
	else if(_rc->rc.s[0]==1 && _rc->rc.s[1]==1){
		chassis_drive(my_controller_cal(robot.BlueTooth.straight, robot.BlueTooth.spin), &robot, 1);
	}
	else chassis_drive(chassis_cal(robot.controller(&rc_ctrl), &robot), &robot, 0);	
	
	return 0;
}

RC_ctrl_t* controller_callback(RC_ctrl_t* _rc){
	return _rc;
}





