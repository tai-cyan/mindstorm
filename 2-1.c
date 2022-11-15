
#include <stdbool.h>
#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include"module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define L_motor EV3_PORT_A
#define R_motor EV3_PORT_B
#define PEN_motor EV3_PORT_C

#define gyro_sensor EV3_PORT_3

const uint32_t pen_time = 200;
const uint32_t curve_time = 1500;

// down->true up->false
void pen(bool move){
  int d,s=40;
  if(move==true)d=-30;
  else if(move==false)d=30;

  ev3_motor_rotate(PEN_motor,d,s,false); //ペン操作
  tslp_tsk(pen_time);
}

void go(int p,int s,uint32_t t){
  ev3_motor_steer(L_motor,R_motor,p,s); //モータ操作
  tslp_tsk(t);
}

void stop(){
  ev3_motor_stop(L_motor,true);
  ev3_motor_stop(R_motor,true);
}


void run_task(intptr_t unused) {
	static int16_t now_val;

	for(int i=0;i<5;i++){
	
	pen(true);
	switch(i){
	  case 0:
	  case 4:
	    go(-20,0,2300);
	    break;
	  default:
	    go(-20,0,1800);
		break;
	}
	    
	stop();
	pen(false);
	ev3_motor_rotate(L_motor,210,20,false);
	ev3_motor_rotate(R_motor,210,20,false);
	tslp_tsk(curve_time);
	stop();
	ev3_gyro_sensor_reset(gyro_sensor);
	
	// curve
	while(1){
		go(20,100,100);
		now_val = ev3_gyro_sensor_get_angle(gyro_sensor);
		if(i<4){
		if(now_val>125){
			stop();
			break;
		}
		}else{
		if(now_val>139){
			stop();
			break;
		}	
		}
	}
	ev3_motor_rotate(L_motor,-270,20,false);
	ev3_motor_rotate(R_motor,-270,20,false);
	tslp_tsk(curve_time);
	stop();

	}
}

void main_task(intptr_t unused) {
  ev3_motor_config(L_motor, LARGE_MOTOR);
  ev3_motor_config(R_motor, LARGE_MOTOR); 
  ev3_motor_config(PEN_motor, MEDIUM_MOTOR);
  ev3_sensor_config(gyro_sensor, gyro_sensor);
  
  act_tsk(RUN_TASK);
}

