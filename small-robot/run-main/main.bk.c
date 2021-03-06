#include <kipr/botball.h>
#include <math.h>
//#0 right + open
//#1 left - close
//analog(0)>=3500 black
//low : low servo position f
//high : high servo position
//disc arm: + => front
//disc arm: - => back

//starter pos: 1.5cm from parellel black line
void followline(int d)
{
    clear_motor_position_counter(0);
    clear_motor_position_counter(1);
    while (get_motor_position_counter(0)<d)
    {
        if (analog(0)>=3500)
        {
            mav(0,1000);
            mav(1,-1500);
            msleep(8);
        }
        if (analog(0)<=3500)
        {
            mav(0,1500);
            mav(1,-1000);
            msleep(8);
        }
    }
}

void followlineReverse(int d)
{
    clear_motor_position_counter(0);
    clear_motor_position_counter(1);
    while (get_motor_position_counter(0)<d)
    {
        if (analog(0)<=3500)
        {
            mav(0,1000);
            mav(1,-1500);
            msleep(8);
        }
        if (analog(0)>=3500)
        {
            mav(0,1500);
            mav(1,-1000);
            msleep(8);
        }
    }
}


void forward(int velocity,int distance)
{
    clear_motor_position_counter(0);
    clear_motor_position_counter(1);
    mtp(0,velocity,distance);
    mtp(1,velocity,-distance);
    msleep(abs((float)distance/((float)velocity/1000)));
    //forward(velocity,distance as tik);
    //1cm = 100tik
}

void turn(int direction)
{
    clear_motor_position_counter(0);
    clear_motor_position_counter(1);
    mtp(direction,1000,920*pow(-1,direction));
    mtp(pow(0.5,direction-1)-1,1000,920*pow(-1,direction));
    msleep(1000);
    //turn(n);
    //right: n = 0
    //left: n = 1
    //1300ms
}
void turna(int direction)
{
    clear_motor_position_counter(0);
    clear_motor_position_counter(1);
    /*
    mtp(pow(2,direction)-1,1000,2300*pow(-1,direction));
    msleep(2400);
    //turn(n);
    //left: n = 0
    //right: n = 1
    //1300ms
    */
    if (direction == 0){
        mtp(1,1000,-1900);
    }
    else{
        mtp(0,1000,1900);
    }
    msleep(2000);
}

void arm(int stat)
{
    if (stat == 0) {
        set_servo_position(1,0);
    }
    else{
        set_servo_position(1,975);
    }
    enable_servo(0);
    msleep(500);
    disable_servo(0);
}

void blackring()    //Arrive at the disc
{
    freeze(0);
    freeze(1);
    
    //lift the first ring
    clear_motor_position_counter(0);
    //arm:+580,go:2900
    clear_motor_position_counter(1);
    clear_motor_position_counter(2);
    mav(0,1500);
    mav(1,-1500);
    mav(2,-400);
    while(get_motor_position_counter(0)<2900){
        msleep(100);
    }
    ao();
    
    //take the arm off
    forward(1500,1500);
    mav(2,1000);
    msleep(360);
    freeze(2);
    
    //go to the second arm
    mav(0,700);
    mav(1,400);
    msleep(600);
    followlineReverse(10000);
    freeze(0);
    freeze(1);
    
    //arrive at the second arm
    forward(1500,-2800);
    freeze(0);
    freeze(2);
    
    //take it down
    mav(2,-1000);
    msleep(150);
    mav(2,-550);
    mav(0,-550);
    mav(1,500);
    msleep(1000);
}

/*
void blueballs(){
    
}
*/
int main()
{
    //preperation
    /*wait_for_light(1);
    shut_down_in(120);
    */
    enable_servos();

    //start
    turna(0);
    forward(1500,2200);
    mav(2,1000);
    msleep(1200);
    freeze(2);
    turn(0);
    forward(1000,-700);
    turn(1);
    
    //Arrive at the disc
    ao();
    msleep(100);
    blackring();

    turn(0);
    forward(1000,100);
    while(analog(0)<=3500)
    {
        mav(0,1000);
        mav(1,-1000);
    }
    forward(1000,50);
    while(analog(0)<=3500)
    {
        mav(0,1500);
        mav(1,-1500);
    }

   
    return 0;
}