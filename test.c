//#include <stc15w.h>
#include <89c52.H>

sbit BSW=P3^1;
sbit ASW=P3^2;
sbit Aout=P3^3;
unsigned int fenzhong,xx;
void Timer0Init(void)       //1000??@12.000MHz
{
    AUXR &= 0x7F;       //定时器0为12T模式
    TMOD &= 0xF0;       //定时模式, 16位自动重装
    //TMOD |= 0x01;       //选择定时器0为模式1，16位不可重装载模式，TL0、TH0全用
    //时钟频率12MHz，12分频，1MHz，1/10^6=1us
    //计算初值  1ms=1000us的初值 ：2的16次方(65535)-1000+1 = 64536 = FC18H  （工具）
    TL0 = 0x18;     //给定时器赋初值，定时1ms
    TH0 = 0xFC;     //??????
    TF0 = 0;        //T0溢出中断标志，清零
    TR0 = 1;        //打开定时器0
}

void main()
{
    Aout=0;
    Timer0Init();
    EA=1;
    ET0=1;//允许T0中断
    while(1);
}

void Int2() interrupt 1 //???????0
{
    xx++;
    if(xx==1000){
        xx=0;P30=!P30;
    }
}
