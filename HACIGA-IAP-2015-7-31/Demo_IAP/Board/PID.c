
#include "PID.h"

struct _pid{
    float SetV;            //�����趨ֲ
    float ActualV;        //����ʵ��ֵ
    float err;                //����ƫ��ֵ
    float err_last;            //����һ��ƫ��ֵ
    float Kp,Ki,Kd;            //������������֡�΢��ϵ��
    float voltage;            //�����ѹֵ������ִ�����ı�����
    float integral;            //�������ֵ
    float umax;
    float umin;
}pid;

void PID_init(void)
{
//    printf("PID_init begin \n");
    pid.SetV=0.0;
    pid.ActualV=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.voltage=0.0;
    pid.integral=0.0;
    pid.Kp=0.2;
   pid.Ki=0.1;       //??,??????,???????????
    pid.Kd=0.2;
//    pid.umax=400;
//    pid.umin=-200;
//    printf("PID_init end \n");
}
float PID_realize(float V){
    float index;
    pid.SetV=V;
    pid.err=pid.SetV-pid.ActualV;

    if(abs(pid.err)>200)           //?????
    {
    index=0.0;
    }else if(abs(pid.err)<180){
    index=1.0;
    pid.integral+=pid.err;
    }else{
    index=(200-abs(pid.err))/20;
    pid.integral+=pid.err;
    }
    pid.voltage=pid.Kp*pid.err+index*pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);

    pid.err_last=pid.err;
    pid.ActualV=pid.voltage*1.0;
    return pid.ActualV;
}
