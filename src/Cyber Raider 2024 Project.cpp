/*        Zexuan Wang | zwang577@ucr.edu 

 *         Discussion section: 24

 *        Assignment: FinalProject Final

 *        

 *        I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *        Demo Link: https://youtu.be/a301NQ2I5ck

 */

#include "timerISR.h"
#include "helper.h"
#include "periph.h"
#include "serialATmega.h"
#include "spiAVR.h"
#include "stdlib.h"

void HardwareReset(){
    PORTB=SetBit(PORTB,0,0);
    _delay_ms(200);
    PORTB=SetBit(PORTB,0,1);
    _delay_ms(200);
}

void ST7735_init(){
    HardwareReset();
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB PIN1 or PORTC PIN5
    PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send
    SPI_SEND(0x01);//SWRESET
    _delay_ms(150);
    SPI_SEND(0x11);//SLPOUT
    _delay_ms(200);
    SPI_SEND(0x36);//COLMOD
    SPI_SEND(0x06);//18bit color mode??
    _delay_ms(10);
    SPI_SEND(0x29);//DISPON
    _delay_ms(200);
    PORTB=SetBit(PORTB,2,1);
}

void setBlackBackground(){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(0x02);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(129);//xe7-0
    //_delay_ms(150);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(0x01);
    SPI_SEND(0);
    SPI_SEND(128);
    //_delay_ms(150);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);
    for(unsigned int i=0; i<128*128; i++){
            SPI_SEND(0);
            SPI_SEND(0);
            SPI_SEND(0);
    }

    PORTB=SetBit(PORTB,2,1);
}

void buildRectangle(int XS,int XE, int YS, int YE, int R, int G, int B){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(XS);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(XE);//xe7-0

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(YS);
    SPI_SEND(0);
    SPI_SEND(YE);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);
    for(int i=0; i<(XS-XE)*(YS-YE)-1; i++){
            SPI_SEND(B);
            SPI_SEND(G);
            SPI_SEND(R);
    }

    PORTB=SetBit(PORTB,2,1);
}

void SetBitBlack(){
    SPI_SEND(0);
    SPI_SEND(0);
    SPI_SEND(0);
}

void SetBitWhite(){
    SPI_SEND(255);
    SPI_SEND(255);
    SPI_SEND(255);
}
void SetBitRed(){
    SPI_SEND(0);
    SPI_SEND(0);
    SPI_SEND(255);
}

void SetBitBlue(){
    SPI_SEND(255);
    SPI_SEND(0);
    SPI_SEND(0);
}

void SetBitYellow(){
    SPI_SEND(0);
    SPI_SEND(255);
    SPI_SEND(255);
}

void SetBitColor(int R, int G, int B){
    SPI_SEND(B);
    SPI_SEND(G);
    SPI_SEND(R);
}

void buildPlayer(int XS,int XE, int YS, int YE,int R, int G, int B){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(XS);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(XE);//xe7-0

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(YS);
    SPI_SEND(0);
    SPI_SEND(YE);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);

    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack();
    SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitColor(R,G,B);
    SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitWhite(); SetBitWhite(); SetBitWhite(); SetBitColor(R,G,B);
    SetBitBlack(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitWhite(); SetBitColor(R,G,B); SetBitWhite(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitBlack();
    SetBitBlack(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitBlack();


    PORTB=SetBit(PORTB,2,1);
}

void buildEnemy(int XS,int XE, int YS, int YE, int R, int G, int B){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(XS);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(XE);//xe7-0

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(YS);
    SPI_SEND(0);
    SPI_SEND(YE);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);

    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlue(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlue(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlue();
    SetBitBlack(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlue(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlue(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlue(); SetBitBlack(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlue(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlue(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlue(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlue(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack();  SetBitBlue(); SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlack(); SetBitBlue(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlue(); SetBitBlack(); SetBitBlack();  SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlue(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack();
    SetBitBlue(); SetBitBlack(); SetBitBlack(); SetBitBlack();  SetBitColor(R,G,B); SetBitColor(R,G,B); SetBitBlue(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlue();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitColor(R,G,B); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();

    PORTB=SetBit(PORTB,2,1);
}

void buildBullet(int XS,int XE, int YS, int YE){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(XS);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(XE);//xe7-0

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(YS);
    SPI_SEND(0);
    SPI_SEND(YE);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);

    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitRed(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitRed(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();



    PORTB=SetBit(PORTB,2,1);
}

void buildFuel(int XS,int XE, int YS, int YE){
    PORTB=SetBit(PORTB,2,0);//sets CS pin to 0 before SPI_SEND()
    //PORTB=SetBit(PORTB,1,0);//sets AO to 0 for command send

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2A);//CASET(sets x column range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);//xs15-8
    SPI_SEND(XS);//xs7-0
    SPI_SEND(0);//xe15-8
    SPI_SEND(XE);//xe7-0

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2B);//RASET(sets y row range)
    PORTB=SetBit(PORTB,1,1);
    SPI_SEND(0x00);
    SPI_SEND(YS);
    SPI_SEND(0);
    SPI_SEND(YE);

    PORTB=SetBit(PORTB,1,0);
    SPI_SEND(0x2C);//RAMWR(sends color/pixel?)
    PORTB=SetBit(PORTB,1,1);

    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitYellow(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();
    SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitYellow(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack(); SetBitBlack();


    PORTB=SetBit(PORTB,2,1);
}

int randomGen(int min, int max){

	int range = max-min + 1;
	return rand()%range + min;
}

                // B,Y,S,S,U,D,L,R,A,X,L,R,0,0,0,0
int SNESArray[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dropSpeed=0;
int enemy1XS = 60;
int enemy1XE = 70;
int enemy1YS = -20;
int enemy1YE = -10;
bool enemy1Hit=0;
int enemy2XS = 20;
int enemy2XE = 30;
int enemy2YS = -75;
int enemy2YE = -65;
bool enemy2Hit=0;
int enemy3XS = 105;
int enemy3XE = 115;
int enemy3YS = -110;
int enemy3YE = -100;
bool enemy3Hit=0;
int playerXS = 60;
int playerXE = 70;
int playerYS = 119;
int playerYE = 129;
int playerR =255;
int playerG =255;
int playerB =255;
int bullet1XS = 0;
int bullet1XE = 0;
int bullet1YS = 0;
int bullet1YE = 0;
bool bullet1RDY = 1;
int Score=0;
int fuelXS = 20;
int fuelXE = 30;
int fuelYS = -50;
int fuelYE = -40;
bool fuelRDY = 1;
int fuelTimer = 0;
int enemy4XS = 90;
int enemy4XE = 100;
int enemy4YS = -100;
int enemy4YE = -90;
bool enemy4Hit=0;
int enemy5XS = 20;
int enemy5XE = 30;
int enemy5YS = -100;
int enemy5YE = -90;
bool enemy5Hit=0;
bool gameOn=0;

// void readSNES(){

//     //sets latch to on then off
//     PORTD=SetBit(PORTD,3,1);
//     _delay_us(12);
//     PORTD=SetBit(PORTD,3,0);
//     _delay_us(6);

//     for(int z=0; z<16; ++z){
//         //sets clock to 0;
//         PORTD=SetBit(PORTD,4,0);
//         //reads from data pin
//         SNESArray[z]=(!GetBit(PIND,5));
//         _delay_us(6);
//         //sets clock to 1
//         PORTD=SetBit(PORTD,4,1);
//         _delay_us(6);
//     }
// }



//Task struct for concurrent synchSMs implmentations
typedef struct _task{
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;


//TODO: Define Periods for each task
// e.g. const unsined long TASK1_PERIOD = <PERIOD>
const unsigned long GCD_PERIOD = 20;//TODO:Set the GCD Period
//enemy
const unsigned long task1Period=500;
//snes
const unsigned long task2Period=20;
//player
const unsigned long task3Period=100;
//sandevistan
const unsigned long task4Period=100;
//enemy
const unsigned long task5Period=500;
//enemy
const unsigned long task6Period=500;
//bullet
const unsigned long task7Period=40;
//fuel
const unsigned long task8Period=500;
//enemy
const unsigned long task9Period=500;
//enemy
const unsigned long task10Period=500;
//gameManager
const unsigned long task11Period=20;

#define NUM_TASKS 11 //TODO: Change to the number of tasks being used

task tasks[NUM_TASKS]; // declared task array with 2 tasks



void TimerISR() {
	for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
		if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
			tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
			tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
		}
		tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
	}
}


//TODO: Create your tick functions for each task
//middle enemy cords
// int enemy1XS = 60;
// int enemy1XE = 70;
// int enemy1YS = -20;
// int enemy1YE = -10;
enum EnemyMiddle{EnemyMiddle};
int TickFct1EnemyMiddle(int state){
    
    switch(state){
        case EnemyMiddle:
            state=EnemyMiddle;
            buildRectangle(enemy1XS,enemy1XE,enemy1YS,enemy1YE,0,0,0);
            enemy1YS+=dropSpeed;
            enemy1YE+=dropSpeed;
            if(enemy1YS>128){
                enemy1XS = randomGen(45,75);
                enemy1XE = enemy1XS + 10;
                enemy1YS = -(randomGen(20,80));
                enemy1YE = enemy1YS + 10;
                if(!enemy1Hit){
                    Score--;
                }
                enemy1Hit=0;
            }
        break;

        default:
            state=EnemyMiddle;
        break;

    }


    switch (state){
        case EnemyMiddle:
            if(!enemy1Hit){
            buildEnemy(enemy1XS,enemy1XE,enemy1YS,enemy1YE,0,0,255);
            }else{
            buildEnemy(enemy1XS,enemy1XE,enemy1YS,enemy1YE,255,0,0);
            }
        break;
    
        default:
        break;
    }
    return state;
}

// int enemy2XS = 20;
// int enemy2XE = 30;
// int enemy2YS = -55;
// int enemy2YE = -45;
enum EnemyLeft{EnemyLeft};
int TickFct5EnemyLeft(int state){

    
    switch(state){
        case EnemyLeft:
            state=EnemyLeft;
            buildRectangle(enemy2XS,enemy2XE,enemy2YS,enemy2YE,0,0,0);
            enemy2YS+=dropSpeed;
            enemy2YE+=dropSpeed;
            if(enemy2YS>128){
                enemy2XS = randomGen(5,35);
                enemy2XE = enemy2XS + 10;
                enemy2YS = -(randomGen(20,80));
                enemy2YE = enemy2YS + 10;
                if(!enemy2Hit){
                    Score--;
                }
                enemy2Hit=0;
            }
        break;

        default:
            state=EnemyLeft;
        break;

    }


    switch (state){
        case EnemyLeft:
            if(!enemy2Hit){
            buildEnemy(enemy2XS,enemy2XE,enemy2YS,enemy2YE,0,0,255);
            }else{
            buildEnemy(enemy2XS,enemy2XE,enemy2YS,enemy2YE,255,0,0);
            }
        break;
    
        default:
        break;
    }
    return state;
}

// int enemy3XS = 105;
// int enemy3XE = 115;
// int enemy3YS = -76;
// int enemy3YE = -66;
enum EnemyRight{EnemyRight};
int TickFct6EnemyRight(int state){
    
    switch(state){
        case EnemyRight:
            state=EnemyRight;
            buildRectangle(enemy3XS,enemy3XE,enemy3YS,enemy3YE,0,0,0);
            enemy3YS+=dropSpeed;
            enemy3YE+=dropSpeed;
            if(enemy3YS>128){
                enemy3XS = randomGen(85,115);
                enemy3XE = enemy3XS + 10;
                enemy3YS = -(randomGen(20,80));
                enemy3YE = enemy3YS + 10;
                if(!enemy3Hit){
                    Score--;
                }
                enemy3Hit=0;
            }
        break;

        default:
            state=EnemyRight;
        break;

    }


    switch (state){
        case EnemyRight:
            if(!enemy3Hit){
            buildEnemy(enemy3XS,enemy3XE,enemy3YS,enemy3YE,0,0,255);
            }else{
            buildEnemy(enemy3XS,enemy3XE,enemy3YS,enemy3YE,255,0,0);
            }
        break;
    
        default:
        break;
    }
    return state;
}

enum EnemyRandom{EnemyRandom};
int TickFct9EnemyRandom(int state){
    
    switch(state){
        case EnemyRandom:
            state=EnemyRandom;
            buildRectangle(enemy4XS,enemy4XE,enemy4YS,enemy4YE,0,0,0);
            enemy4YS+=dropSpeed;
            enemy4YE+=dropSpeed;
            if(enemy4YS>128){
                enemy4XS = randomGen(5,115);
                enemy4XE = enemy4XS + 10;
                enemy4YS = -(randomGen(50,120));
                enemy4YE = enemy4YS + 10;
                if(!enemy4Hit){
                    Score--;
                }
                enemy4Hit=0;
            }
        break;

        default:
            state=EnemyRandom;
        break;

    }


    switch (state){
        case EnemyRandom:
            if(!enemy4Hit){
            buildEnemy(enemy4XS,enemy4XE,enemy4YS,enemy4YE,0,0,255);
            }else{
            buildEnemy(enemy4XS,enemy4XE,enemy4YS,enemy4YE,255,0,0);
            }
        break;
    
        default:
        break;
    }
    return state;
}

enum EnemyRandom2{EnemyRandom2};
int TickFct9EnemyRandom2(int state){
    
    switch(state){
        case EnemyRandom2:
            state=EnemyRandom2;
            buildRectangle(enemy5XS,enemy5XE,enemy5YS,enemy5YE,0,0,0);
            enemy5YS+=dropSpeed;
            enemy5YE+=dropSpeed;
            if(enemy5YS>128){
                enemy5XS = randomGen(5,115);
                enemy5XE = enemy5XS + 10;
                enemy5YS = -(randomGen(50,120));
                enemy5YE = enemy5YS + 10;
                if(!enemy5Hit){
                    Score--;
                }
                enemy5Hit=0;
            }
        break;

        default:
            state=EnemyRandom;
        break;

    }


    switch (state){
        case EnemyRandom:
            if(!enemy5Hit){
            buildEnemy(enemy5XS,enemy5XE,enemy5YS,enemy5YE,0,0,255);
            }else{
            buildEnemy(enemy5XS,enemy5XE,enemy5YS,enemy5YE,255,0,0);
            }
        break;
    
        default:
        break;
    }
    return state;
}

enum ReadSNES{ReadSNES};
int TickFct2ReadSNES(int state){
     static unsigned char i;

    switch(state){
        case ReadSNES:
            state=ReadSNES;
        break;

        default:
            state=ReadSNES;
        break;

    }


    switch (state){
        case ReadSNES:
            //sets latch to on then off
            PORTD=SetBit(PORTD,3,1);
            _delay_us(12);
            PORTD=SetBit(PORTD,3,0);
            _delay_us(6);

            for(i=0; i<16; ++i){
                //sets clock to 0;
                PORTD=SetBit(PORTD,4,0);
                //reads from data pin
                SNESArray[i]=(!GetBit(PIND,5));
                //serial_println(SNESArray[i]);
                _delay_us(6);
                //sets clock to 1
                PORTD=SetBit(PORTD,4,1);
                _delay_us(6);
            }
        break;
    
        default:
        break;
    }
    return state;
}

    // int playerXS = 60;
    // int playerXE = 70;
    // int playerYS = 119;
    // int playerYE = 129;
enum TestPlayer{TestPlayer};
int TickFct3TestPlayer(int state){

    switch(state){
        case TestPlayer:
            state=TestPlayer;
            if(SNESArray[10]){
                buildRectangle(playerXS,playerXE,playerYS,playerYE,0,0,0);
                playerXS+=-1;
                playerXE+=-1;
                if(playerXS<0){
                    playerXS = 0;
                    playerXE = 10;
                }
            }else if(SNESArray[11]){
                buildRectangle(playerXS,playerXE,playerYS,playerYE,0,0,0);
                playerXS+=1;
                playerXE+=1;
                if(playerXE>131){
                    playerXS = 121;
                    playerXE = 131;
                }
            }
        break;

        default:
            state=TestPlayer;
        break;

    }


    switch (state){
        case TestPlayer:
            buildPlayer(playerXS,playerXE,playerYS,playerYE,playerR,playerG,playerB);
        break;
    
        default:
        break;
    }
    return state;
}

enum BulletTime{WaitBT,BulletTime};
int TickFct4BulletTime(int state){
    static int b = 0;

    switch(state){
        case WaitBT:
            if(b>100 && SNESArray[5]&&gameOn){
                state=BulletTime;
                b=0;
            }else{
                state=WaitBT;
            }
        break;

        case BulletTime:
            if(b>50){
                state=WaitBT;
                b=0;
            }else{
                state=BulletTime;
            }
        break;

        default:
            state=WaitBT;
        break;

    }


    switch (state){
        case WaitBT:
            dropSpeed=3;
            playerR=255;
            playerG=255;
            playerB=255;
            if(b>100){
                if(gameOn){
                    PORTD=SetBit(PORTD,2,1);
                }else{
                    PORTD=SetBit(PORTD,2,0);
                }
            }else{
                PORTD=SetBit(PORTD,2,0);
            }
            b++;
        break;

        case BulletTime:
            if(gameOn){
                dropSpeed=1;
                PORTD=SetBit(PORTD,2,0);
                playerR=0;
                playerG=255;
                playerB=0;
                b++;
            }
        break;
    
        default:
        break;
    }
    return state;
}

// int bullet1XS = 0;
// int bullet1XE = 0;
// int bullet1YS = 0;
// int bullet1YE = 0;
// bool bullet1RDY = 1;
// int bullet2XS = 0;
// int bullet2XE = 0;
// int bullet2YS = 0;
// int bullet2YE = 0;
// bool bullet2RDY = 1;
enum PlayerBullet{PlayerBullet};
int TickFct7PlayerBullet(int state){
    
    switch(state){
        case PlayerBullet:
            if(SNESArray[8] && (bullet1RDY)&&gameOn){
                if(bullet1RDY){
                    bullet1XE=playerXE;
                    bullet1XS=playerXS;
                    bullet1YE=playerYE;
                    bullet1YS=playerYS;
                    bullet1RDY=0;
                }
            }

            if(!bullet1RDY){
                buildRectangle(bullet1XS,bullet1XE,bullet1YS,bullet1YE,0,0,0);

                if(((bullet1XS<=enemy1XE && bullet1XS>=enemy1XS ) || (bullet1XE<=enemy1XE && bullet1XE>=enemy1XS))
                    && 
                    ((bullet1YS<=enemy1YE && bullet1YS>=enemy1YS) || (bullet1YE<=enemy1YE && bullet1YE>=enemy1YS))
                    &&
                    (enemy1Hit==0)&&gameOn){
                    //serial_println("hit");
                    enemy1Hit=1;
                    Score++;
                }
                if(((bullet1XS<=enemy3XE && bullet1XS>=enemy3XS ) || (bullet1XE<=enemy3XE && bullet1XE>=enemy3XS))
                    && 
                    ((bullet1YS<=enemy3YE && bullet1YS>=enemy3YS) || (bullet1YE<=enemy3YE && bullet1YE>=enemy3YS))
                    &&
                    (enemy3Hit==0)&&gameOn){
                    //serial_println("hit");
                    enemy3Hit=1;
                    Score++;
                }
                if(((bullet1XS<=enemy2XE && bullet1XS>=enemy2XS ) || (bullet1XE<=enemy2XE && bullet1XE>=enemy2XS))
                    && 
                    ((bullet1YS<=enemy2YE && bullet1YS>=enemy2YS) || (bullet1YE<=enemy2YE && bullet1YE>=enemy2YS))
                    &&
                    (enemy2Hit==0)){
                    //serial_println("hit");
                    enemy2Hit=1;
                    Score++;
                }
                if(((bullet1XS<=enemy4XE && bullet1XS>=enemy4XS ) || (bullet1XE<=enemy4XE && bullet1XE>=enemy4XS))
                    && 
                    ((bullet1YS<=enemy4YE && bullet1YS>=enemy4YS) || (bullet1YE<=enemy4YE && bullet1YE>=enemy4YS))
                    &&
                    (enemy4Hit==0)&&gameOn){
                    //serial_println("hit");
                    enemy4Hit=1;
                    Score++;
                }
                if(((bullet1XS<=enemy5XE && bullet1XS>=enemy5XS ) || (bullet1XE<=enemy5XE && bullet1XE>=enemy5XS))
                    && 
                    ((bullet1YS<=enemy5YE && bullet1YS>=enemy5YS) || (bullet1YE<=enemy5YE && bullet1YE>=enemy5YS))
                    &&
                    (enemy5Hit==0)&&gameOn){
                    //serial_println("hit");
                    enemy5Hit=1;
                    Score++;
                }

                bullet1YS+=-1;
                bullet1YE+=-1;
                if(bullet1YS<0){
                    bullet1XS = 0;
                    bullet1XE = 0;
                    bullet1YS = 0;
                    bullet1YE = 0;
                    bullet1RDY = 1;
                }
            }
        break;

        default:
            state=PlayerBullet;
        break;

    }


    switch (state){
        case PlayerBullet:
            if(!bullet1RDY){
                buildBullet(bullet1XS,bullet1XE,bullet1YS,bullet1YE);
                PORTD=SetBit(PORTD,6,0);
            }else{
                if(gameOn){
                    PORTD=SetBit(PORTD,6,1);
                }else{
                    PORTD=SetBit(PORTD,6,0);
                }
            }
        break;
    
        default:
        break;
    }
    return state;
}

enum FuelCan{FuelCan};
int TickFct8PFuelCan(int state){
    
    switch(state){
        case FuelCan:
                buildRectangle(fuelXS,fuelXE,fuelYS,fuelYE,0,0,0);

                if(((fuelXS<=playerXE && fuelXS>=playerXS ) || (fuelXE<=playerXE && fuelXE>=playerXS))
                    && 
                    ((fuelYS<=playerYE && fuelYS>=playerYS) || (fuelYE<=playerYE && fuelYE>=playerYS))
                    &&
                    (fuelRDY==1)&&gameOn){
                    //serial_println("hit");
                    fuelRDY=0;
                    fuelTimer+=60;
                    if(fuelTimer>200){
                        fuelTimer=200;
                    }
                }


                fuelYS+=dropSpeed;
                fuelYE+=dropSpeed;
            if(fuelYS>128){
                fuelXS = randomGen(5,115);
                fuelXE = fuelXS + 10;
                fuelYS = -(randomGen(50,75));
                fuelYE = fuelYS + 10;
                fuelRDY=1;
            }
        break;

        default:
            state=FuelCan;
        break;

    }


    switch (state){
        case FuelCan:
            buildFuel(fuelXS,fuelXE,fuelYS,fuelYE);
            fuelTimer--;
            if(fuelTimer>160){
                PORTC=SetBit(PORTC,1,1);
                PORTC=SetBit(PORTC,2,1);
                PORTC=SetBit(PORTC,3,1);
                PORTC=SetBit(PORTC,4,1);
                PORTC=SetBit(PORTC,5,1);
            }else if(fuelTimer>120){
                PORTC=SetBit(PORTC,1,0);
                PORTC=SetBit(PORTC,2,1);
                PORTC=SetBit(PORTC,3,1);
                PORTC=SetBit(PORTC,4,1);
                PORTC=SetBit(PORTC,5,1);
            }else if(fuelTimer>80){
                PORTC=SetBit(PORTC,1,0);
                PORTC=SetBit(PORTC,2,0);
                PORTC=SetBit(PORTC,3,1);
                PORTC=SetBit(PORTC,4,1);
                PORTC=SetBit(PORTC,5,1);
            }else if(fuelTimer>40){
                PORTC=SetBit(PORTC,1,0);
                PORTC=SetBit(PORTC,2,0);
                PORTC=SetBit(PORTC,3,0);
                PORTC=SetBit(PORTC,4,1);
                PORTC=SetBit(PORTC,5,1);
            }else if(fuelTimer>0){
                PORTC=SetBit(PORTC,1,0);
                PORTC=SetBit(PORTC,2,0);
                PORTC=SetBit(PORTC,3,0);
                PORTC=SetBit(PORTC,4,0);
                PORTC=SetBit(PORTC,5,1);
            }else{
                PORTC=SetBit(PORTC,1,0);
                PORTC=SetBit(PORTC,2,0);
                PORTC=SetBit(PORTC,3,0);
                PORTC=SetBit(PORTC,4,0);
                PORTC=SetBit(PORTC,5,0);
                if(gameOn==1){
                serial_println("Your score was");
                serial_println(Score);
                serial_println("Press start to restart");
                }
            }
        break;
    
        default:
        break;
    }
    return state;
}

enum ManageGame{OffDisp,Off, On};
int TickFct11ManageGame(int state){

    switch(state){
        case OffDisp:
            serial_println("Press start to start");
            state=Off;
        break;

        case Off:
            if(gameOn==0 && SNESArray[3] && fuelTimer<=0){
                state=On;
                setBlackBackground();
                enemy1YS = -20;
                enemy1YE = -10;
                enemy1Hit=0;
                enemy2YS = -75;
                enemy2YE = -65;
                enemy2Hit=0;
                enemy3YS = -110;
                enemy3YE = -100;
                enemy3Hit=0;
                playerYS = 119;
                playerYE = 129;
                bullet1XS = 0;
                bullet1XE = 0;
                bullet1YS = 0;
                bullet1YE = 0;
                bullet1RDY = 1;
                Score=0;
                fuelYS = -50;
                fuelYE = -40;
                fuelRDY = 1;
                enemy4YS = -100;
                enemy4YE = -90;
                enemy4Hit=0;
                enemy5YS = -100;
                enemy5YE = -90;
                enemy5Hit=0;
                fuelTimer=210;
                gameOn=1;
            }
        break;
        
        case On:
            if(gameOn==1 && fuelTimer<=0){
                state=Off;
                gameOn=0;
            }
        break;

        default:
            state=Off;
        break;

    }


    switch (state){
        case Off:
        break;
        
        case On:
        break;
    
        default:
        break;
    }
    return state;
}

int main(void) {
    //TODO: initialize all your inputs and ouputs
    DDRC=0b11111111; PORTC=0b00000000;
    DDRD=0b11011111; PORTD=0b00100000;
    DDRB=0b11111111; PORTB=0b00000000;
    ADC_init();   // initializes ADC
    serial_init(9600);   // initializes ADC
    SPI_INIT(); //initalizes SPI
    ST7735_init();
    setBlackBackground();
    


    //TODO: Initialize tasks here
    // e.g. 
    // tasks[0].period = ;
    // tasks[0].state = ;
    // tasks[0].elapsedTime = ;
    // tasks[0].TickFct = ;
    unsigned char i=0;
    tasks[i].period=task1Period;
    tasks[i].state=EnemyMiddle;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct1EnemyMiddle;
    i++;
    tasks[i].period=task2Period;
    tasks[i].state=ReadSNES;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct2ReadSNES;
    i++;
    tasks[i].period=task3Period;
    tasks[i].state=TestPlayer;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct3TestPlayer;
    i++;
    tasks[i].period=task4Period;
    tasks[i].state=WaitBT;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct4BulletTime;
    i++;
    tasks[i].period=task5Period;
    tasks[i].state=EnemyLeft;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct5EnemyLeft;
    i++;
    tasks[i].period=task6Period;
    tasks[i].state=EnemyRight;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct6EnemyRight;
    i++;
    tasks[i].period=task7Period;
    tasks[i].state=PlayerBullet;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct7PlayerBullet;
    i++;
    tasks[i].period=task8Period;
    tasks[i].state=FuelCan;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct8PFuelCan;
    i++;
    tasks[i].period=task9Period;
    tasks[i].state=EnemyRandom;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct9EnemyRandom;
    i++;
    tasks[i].period=task10Period;
    tasks[i].state=EnemyRandom2;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct9EnemyRandom2;
    i++;
    tasks[i].period=task11Period;
    tasks[i].state=OffDisp;
    tasks[i].elapsedTime=tasks[i].period;
    tasks[i].TickFct= &TickFct11ManageGame;

    TimerSet(GCD_PERIOD);
    TimerOn();

    while (1) {}

    return 0;
}
