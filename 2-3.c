
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
#define ultraSonic_sensor EV3_PORT_1

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
int16_t now_val_1=100;
int16_t now_val_2=0;
char str[20];


while(1){
  ev3_motor_rotate(L_motor,90,20,false);
  ev3_motor_rotate(R_motor,90,20,false);
  tslp_tsk(100);

  now_val_1=(int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);

  if(now_val_1==0){
    continue;
  }else if(now_val_1<=13){
  ev3_led_set_color(LED_RED);
  break;
  }

  }

now_val_1=(int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
  sprintf(str,"distance:%4d",now_val_1);
  ev3_lcd_draw_string(str,0,0);


for(int i=0;i<3;i++){
  pen(true);
  go(-20,0,1000);
    
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
  	now_val_2 = ev3_gyro_sensor_get_angle(gyro_sensor);
  
  	if(now_val_2>120){
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

void main_task(intptr_t unused) {
  ev3_motor_config(L_motor, LARGE_MOTOR);
  ev3_motor_config(R_motor, LARGE_MOTOR); 
  ev3_motor_config(PEN_motor, MEDIUM_MOTOR);
  ev3_sensor_config(gyro_sensor, gyro_sensor);
  ev3_sensor_config(ultraSonic_sensor,ULTRASONIC_SENSOR);

  act_tsk(RUN_TASK);
}

