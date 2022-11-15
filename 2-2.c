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
#define color_sensor EV3_PORT_

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

void go(int power,int steer,uint32_t t){
  ev3_motor_steer(L_motor,R_motor,power,steer); //モータ操作
  tslp_tsk(t);
}

void stop(){
  ev3_motor_stop(L_motor,true);
  ev3_motor_stop(R_motor,true);
}

void run_task(intptr_t unused) {
  char str[20];
  while(1){
    colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
    sprintf(str,"color -> %s",now_color)
    ev3_lcd_draw_string(now_color, 0, 0);
    tslp_tsk(pen_time);
    }
  }




}

void main_task(intptr_t unused) {
  ev3_motor_config(L_motor, LARGE_MOTOR);
  ev3_motor_config(R_motor, LARGE_MOTOR); 
  ev3_motor_config(PEN_motor, MEDIUM_MOTOR);
  ev3_sensor_config(gyro_sensor, gyro_sensor);
  ev3_sensor_config(color_sensor, COLOR_SENSOR);
  
  act_tsk(RUN_TASK);
}