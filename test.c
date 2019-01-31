//#include <stc15w.h>
#include <89c52.H>

sbit BSW = P3^1;
sbit ASW = P3^2;
sbit Aout = P3^3;
unsigned int millisecond;//毫秒
unsigned int second;//秒
int state_old;//老状态
int state_new;//新状态
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
    Aout = 0;
    Timer0Init();
    EA = 1;
    ET0 = 1;//允许T0中断
    while(1);
}

//1ms执行一次中断
void Int2() interrupt 1 //???????0
{
    millisecond++;
    if(millisecond == 1000){
        millisecond = 0;
        second++;//+1秒
        // P30 =! P30;
    }
    //保存之前的状态
    state_old = state_new;
    //获取新状态
    if(ASW == 1 && BSW == 0){
        state_new = 1;
    }
    else if(ASW == 1 && BSW == 1){
        state_new = 2;
    }
    else if(ASW == 0 && BSW == 1){
        state_new = 3;
    }
    else if(ASW == 0 && BSW == 0){
        state_new = 4;
    }
    //现在的状态，和之前相同，则继续计数。否则清空计数，切换到新状态，重新计数
    if(state_new == state_old){
        switch(state_new)
        {
            case 1:
                if(second <= 5){
                    //[0秒, 5秒]
                    Aout = 1;
                }
                else if(second <= 20*60*60){
                    //(5秒, 20小时]
                    Aout = 0;
                }
                else if(second <= 20*60*60+5){
                    //(20小时, 20小时+5秒]
                    Aout = 1;
                }
                else{
                    //(20小时+5秒, +∞)
                    Aout = 0;
                    second = 20*60*60+5;//使输出不变
                }
                break;
            case 2:
                Aout = 0;
                break;
            case 3:
                Aout = 0;
                break;
            case 4:
                if(second <= 5){
                    Aout = 1;
                }else{
                    Aout = 0;
                    second = 5;//使输出不变
                }
                break;
            default:
                break;
        }
    }else{
        millisecond = 0;
        second = 0;
    }
}
