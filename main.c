/*      Author: ffan005
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 *      Demo: 
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Init, Release};

unsigned char game[] = "Welcome to the PACMAN";
unsigned char lose[] = "you lost heheheh";
unsigned char j = 0; //scroll the game message

int Tick(int state){
switch(state){
        case Start:
                state = LCD_Init;
                break;
        case Init:
                if((~PINA & 0x01) == 0x01){
                        state = LCD_Pause;
                }else if(game[i + j] == '#'){
                        state = LCD_Lose;
                }else{
                        state = LCD_Init;
                }
                break;
        case Release:
                if((~PINA & 0x01) == 0x00){
                        state = LCD_Init;
                }else{
                        state = LCD_Release;
                }
                break;
        default:
                state = LCD_Start;
                break;

}

  switch(state){
        case Start:
                break;
        case Init:
                LCD_Cursor(i);
                for(int a = 1; a <= 32; ++a){ //evil #
                        LCD_Cursor(a);
                        LCD_WriteData(game[(a+j)%32]);
                }

                if(j == 32){
                        j = 0;
                }else{
                        j++;
                }
                break;
        case Pause:
                break;
        case Lose:
                LCD_DisplayString(1,lose);
                i = 1;
                j = 0;
                break;
        case Release:
                break;
        default:
                break;

}

return state;
}

int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    LCD_init();

    static task task1, task2;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 1;
    task1.elapsedTime = task1.period;
    task1.TickFct = Tick;

    TimerSet(1);
    TimerOn();

    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; ++i) {
                if (tasks[i]->elapsedTime == tasks[i]->period) {
                        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                        tasks[i]->elapsedTime = 0;
                }
                tasks[i]->elapsedTime += 1;
        }
      
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
