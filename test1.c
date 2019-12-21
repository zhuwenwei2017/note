#include <stc15w.h>
/*
in1     in2     out
0   X   0
1   0   No Change
1   1   1
*/
// #include <89c52.H>
sbit ASW = P3^1;
sbit BSW = P3^2;
sbit Aout = P3^3;
unsigned int millisecond;//毫秒
unsigned int second;//秒
#define delay_second 5//延迟5秒
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
    TR0 = 1;        //打开定时器0T
}

void main()
{
    Aout = 0;
    Timer0Init();
    EA = 1;
    ET0 = 1;//允许T0中断
    while(1){
        if(second >= delay_second){
            if(ASW == 0){
                Aout = 0;
            }else{
                if(BSW == 1){
                    Aout = 1;
                }
            }
        }
    }
}

//1ms执行一次中断
void Int2() interrupt 1 //???????0
{
    millisecond++;
    if(millisecond == 1000){
        millisecond = 0;
        second++;//+1秒
    }
    if(second >= delay_second){
        TR0 = 0;        //关闭定时器0T
        EA = 0;//关中断
        ET0 = 0;//关闭T0中断
    }
}
