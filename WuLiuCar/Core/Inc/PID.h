#ifndef PID_H
#define PID_H
#include <main.h>
enum PID_MODE
{
  PID_POSITION = 0,
  PID_DELTA
};

typedef struct
{
  //PID 三参数
  float Kp;
  float Ki;
  float Kd;
  
  float max_iout; //最大积分输出
  float max_out;  //最大输出
  
  float Integral_Separation_Threshold;
  float Dead_Zone;

  float set;
  float fdb;

  float out;
  float Pout;
  float Iout;
  float Dout;
  float Dbuf[3];  //微分项 0最新 1上一次 2上上次
  float error[3]; //误差项 0最新 1上一次 2上上次

  uint8_t mode;
  float debug_set;
  bool debug_sign;
  bool BLE_debug_sign;
	
}PID_;

void PID_init(PID_ *pid, uint8_t mode, float Kp, float Ki, float Kd, float max_out, float max_iout, float Integral_Separation_Threshold, float Dead_Zone);
float PID_calc(PID_ *pid, float ref, float set);
void PID_clear(PID_ *pid);

extern PID_ PID[2];

#endif

