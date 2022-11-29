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
#define color_sensor EV3_PORT_2

const uint32_t pen_time = 200;
const uint32_t curve_time = 1500;
const uint32_t steer_time = 1000;
const uint32_t wait_time = 100;

// down->true up->false
void pen(bool move){
  int d,s=40;
  if(move==true)d=-30;
  else if(move==false)d=30;

  ev3_motor_rotate(PEN_motor,d,s,false); //ペン操作
  tslp_tsk(pen_time);
}

void go(int power,int steer,uint32_t t){
  ev3_motor_steer(L_motor,R_motor,power,steer); //モータ操作
  tslp_tsk(t);
}

void stop(){
  ev3_motor_stop(L_motor,true);
  ev3_motor_stop(R_motor,true);
}

void circle(){
  go(20,0,500);
  stop();
  pen(true);
  go(30,100,2400);
  stop();
  pen(false);  
}

void triangle(){
  go(20,100,400);
  stop();
  go(-20,0,600);
  stop();
  go(-20,100,400);
  stop();
  go(-20,0,1000);
  stop();
  for(int i=0;i<3;i++){
  
  //ペン出し
  pen(true);

  go(20,0,1800);
  stop();

  //ペン戻し
  pen(false);


  ev3_motor_rotate(L_motor,220,20,false);
	ev3_motor_rotate(R_motor,220,20,false);
  tslp_tsk(curve_time);
  stop();

  go(20,100,1100);
  stop();
  
  ev3_motor_rotate(L_motor,-270,20,false);
	ev3_motor_rotate(R_motor,-270,20,false);
  tslp_tsk(curve_time);
  stop();

  }
  
}

void star() {
  go(20,0,1000);
	static int16_t now_val;

	for(int i=0;i<5;i++){
	
	pen(true);
  go(-20,0,1700);
	stop();
	pen(false);
	ev3_motor_rotate(L_motor,220,20,false);
	ev3_motor_rotate(R_motor,220,20,false);
	tslp_tsk(curve_time);
	stop();
	ev3_gyro_sensor_reset(gyro_sensor);
	
	// curve
	while(1){
		go(20,100,100);
		now_val = ev3_gyro_sensor_get_angle(gyro_sensor);
	
		if(now_val>130){
			stop();
			break;
		}
	}
	ev3_motor_rotate(L_motor,-270,20,false);
	ev3_motor_rotate(R_motor,-270,20,false);
	tslp_tsk(curve_time);
	stop();

	}
}




void run_task(intptr_t unused) {

  while(true){
    colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
    if(now_color==COLOR_BLUE || now_color==COLOR_GREEN || now_color==COLOR_RED || now_color==COLOR_YELLOW){
      go(20,0,100);
      stop();
      now_color = ev3_color_sensor_get_color(color_sensor);
 

	    switch(now_color){
	      case COLOR_GREEN: //三角
	        triangle();
          break;
	      case COLOR_RED: //円
	        ev3_led_set_color(LED_RED);
            circle();
	    	break;
	      case COLOR_YELLOW: //四角
	        ev3_led_set_color(LED_ORANGE);
	    	break;
	      case COLOR_BLUE: //星形
	        ev3_led_set_color(LED_RED);
          star();
	        break;
	      default:
	        ev3_led_set_color(LED_OFF);
	        break;
      }
      break;
    }
    // go(20,0,100);
    ev3_motor_rotate(L_motor,180,20,false);
	  ev3_motor_rotate(R_motor,180,20,false);
    tslp_tsk(100);
    stop();
  }



}

void main_task(intptr_t unused) {
  colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
  ev3_motor_config(L_motor, LARGE_MOTOR);
  ev3_motor_config(R_motor, LARGE_MOTOR); 
  ev3_motor_config(PEN_motor, MEDIUM_MOTOR);
  ev3_sensor_config(gyro_sensor, gyro_sensor);
  ev3_sensor_config(color_sensor, COLOR_SENSOR);
  
  act_tsk(RUN_TASK);
}
