/*      Author: ffan005
 *  Partner(s) Name: 
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 *      Demo: https://www.youtube.com/watch?v=XgfSjpKShss (Complexity 1: Motion Sensor)
            : https://www.youtube.com/watch?v=_ntX2DG7sG8 (General)
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <util/delay.h>
#include "nokia5110.h"
#endif

void ADC_init() {
    ADMUX = (1 << REFS0);
    
    // ADC Enable and prescalar 128
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

enum States {Start, Init, Game, Release, Score, Win};
unsigned char scores = 17;

int Tick(int state){
switch(state){
        case Start:
                state = Init;
                break;
        case Init:
                if((~PINA & 0xC0) == 0x40){
                        scores = scores + 1;
                        state = Game;
                }else if((~PINA & 0xC0) == 0x80){
                        state = Release;
                }else if((~PINA & 0xC0) == 0xC0){
                        state = Score;
                }else{
                        state = Init;
                }
                break;
        case Game:
                LCD_DisplayString(1, "Let's Play            ");
                if((~PINA & 0xC0) == 0x80){
                        state = Release;
                }else if((~PINA & 0xC0) == 0xC0){
                        state = Score;
                }else{
                        state = Game;
                }
                break;
        case Release:
                LCD_DisplayString(1, "Game Reset?           ");
                if((~PINA & 0xC0) == 0x80){
                        scores = scores + 1;
                        state = Game;
                }else{
                        state = Release;
                }
                break;
        case Score:
                LCD_DisplayString(1, "Score: ");

                if (scores < 10) {
                LCD_Cursor(8);
                LCD_WriteData(scores + '0');
                LCD_Cursor(9);
                LCD_WriteData(' ');
                LCD_Cursor(10);
                LCD_WriteData(' ');
                LCD_Cursor(11);
                LCD_WriteData(' ');
                LCD_Cursor(12);
                LCD_WriteData(' ');
                } else if (scores < 25) {
                unsigned char a = scores / 10;
                unsigned char b = scores % 10;
                LCD_Cursor(8);
                LCD_WriteData(a + '0');
                LCD_Cursor(9);
                LCD_WriteData(b + '0');
                LCD_Cursor(10);
                LCD_WriteData(' ');
                LCD_Cursor(11);
                LCD_WriteData(' ');
                LCD_Cursor(12);
                LCD_WriteData(' ');
                }

                if((~PINA & 0xC0) == 0x40){
                        scores = 0;
                        state = Game;
                }else if((~PINA & 0xC0) == 0x80){
                        state = Release;
                }else{
                        state = Score;
                }
                break;
        case Win:
                LCD_DisplayString(1, "You win, click anything to reset");
                if((~PINA & 0xC0) == 0x40){
                        state = Init;
                }else if((~PINA & 0xC0) == 0x80){
                        state = Init;
                }else if((~PINA & 0xC0) == 0xC0){
                        state = Init;
                }else{
                        state = Win;
                }
                break;
        default:
                state = Start;
                break;
}

  switch(state){
        case Start:
                break;
        case Init:
                break;
        case Game:
                break;
        case Release:
                break;
        case Score:
                break;
        case Win:
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
    LCD_DisplayString(1, "Welcome to the  PACMAN");

    static task task1;
    task *tasks[] = { &task1 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = Tick;

    TimerSet(50);
    TimerOn();

    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; ++i) {
                if (tasks[i]->elapsedTime == tasks[i]->period) {
                        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                        tasks[i]->elapsedTime = 0;
                }
                tasks[i]->elapsedTime += 50;
        }

        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
