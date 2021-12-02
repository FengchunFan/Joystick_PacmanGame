#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <util/delay.h>
#include "nokia5110.h"
//#include "pwm.h"
#endif

void ADC_init() {
    ADMUX = (1 << REFS0);
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

// http://maxembedded.com/2011/06/the-adc-of-the-avr/
unsigned short readADC(uint8_t ch) {
    ch &= 0x07;
    ADMUX = (ADMUX & 0xF8) | ch;
    ADCSRA |= (1 << ADSC);
    while ( (ADCSRA) & (1 << ADSC) );
    return ADC;
}

enum States {Start, Init, Game, Release, Score, Win};
unsigned char scores = 10;

int Tick(int state){
switch(state){
        case Start:
                state = Init;
                break;
        case Init:
                if((~PINA & 0xC0) == 0x40){
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
                        scores = 0;
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

enum Joystick_States {JS_START, JS_WAIT, JS_UP, JS_DOWN, JS_LEFT, JS_RIGHT};
unsigned char pos;

int JS_Tick(int state) {
    unsigned short X;
    unsigned short Y;

    X = readADC(0);
    Y = readADC(1);

    switch (state) {
        case JS_START:
                        state = JS_WAIT;
                        break;
        case JS_WAIT:
            if (Y >= 800) {
                        state = JS_RIGHT;
            } else if (Y <= 80) {
                        state = JS_LEFT;
            } else if (X <= 80) {
                        state = JS_UP;
            } else if (X >= 800) {
                        state = JS_DOWN;
            } else {
                        state = JS_WAIT;
            }
            break;

        case JS_RIGHT:
            if (Y >= 800) {
                        state = JS_RIGHT;
            } else {
                        state = JS_WAIT;
            }
            break;

        case JS_LEFT:
            if (Y <= 80) {
                        state = JS_LEFT;
            } else {
                        state = JS_WAIT;
            }
            break;

        case JS_UP:
            if (X <= 80) {
                        state = JS_UP;
            } else {
                        state = JS_WAIT;
            }
            break;

        case JS_DOWN:
            if (X >= 800) {
                        state = JS_DOWN;
            } else {
                        state = JS_WAIT;
            }
            break;

        default:
            state = JS_START;;
            break;
    }
            
    switch (state) {
        case JS_START:
            break;

        case JS_WAIT:
            pos = 0x00;
            break;

        case JS_UP:
            pos = 0x01;
            break;

        case JS_DOWN:
            pos = 0x02;
            break;

        case JS_LEFT:
            pos = 0x03;
            break;

        case JS_RIGHT:
            pos = 0x04;
            break;

        default:
            break;
    }
    return state;
}


enum Cursor_States {C_START, C_CHECK, C_UP, C_DOWN, C_LEFT, C_RIGHT, C_RELEASE};

int Cursor_Tick(int state) {
            switch(state){
                        case C_START:
                                if((~PINA & 0xC0) == 0x40){
                                    state = C_CHECK;           
                                }else{
                                    state = C_START;
                                }
                                break;
                                    
                        case C_CHECK:
                                if (pos == 0x00) { 
                                            state = C_CHECK; 
                                } else if (pos == 0x01) { 
                                            state = C_UP; 
                                } else if (pos == 0x02) { 
                                            state = C_DOWN; 
                                } else if (pos == 0x03) { 
                                            state = C_LEFT; 
                                } else if (pos == 0x04) { 
                                            state = C_RIGHT; 
                                } else { 
                                            state = C_CHECK; 
                                }
                                break;
                                    
                        case C_UP:
                                if (pos != 0x00){
                                    state = C_RELEASE;
                                }else{
                                    state = C_CHECK;
                                }
                                break;
                        
                        case C_DOWN:
                                if (pos != 0x00){
                                    state = C_RELEASE;
                                }else{
                                    state = C_CHECK;
                                }
                                break;
                                
                        case C_LEFT:
                                if (pos != 0x00){
                                    state = C_RELEASE;
                                }else{
                                    state = C_CHECK;
                                }
                                break;
                                 
                        case C_RIGHT:
                                if (pos != 0x00){
                                    state = C_RELEASE;
                                }else{
                                    state = C_CHECK;
                                }
                                break;
                                    
                        case C_RELEASE:
                                if (pos == 0x00){
                                    state = C_CHECK;
                                }else{
                                    state = C_RELEASE;
                                }
                                break;
                                 
                        default:
                                state = C_START;
                                break;
            }
            
            switch(state){
                        //update cursor information
                        case C_START:
                                break;
                                    
                        case C_CHECK:
                                break;
                                    
                        case C_UP:
                                    LCD_DisplayString(1, "Up");
                                break;
                                    
                        case C_DOWN:
                                    LCD_DisplayString(1, "Down");
                                break;
                                    
                        case C_RELEASE:
                                break;
                                    
                        case C_LEFT:
                                    LCD_DisplayString(1, "Left");
                                break;
                                    
                        case C_RIGHT:
                                    LCD_DisplayString(1, "Right");
                                break;
                                    
                        default:
                                break;
            }
   return state;
}

enum NOKIA_States{NOKIA_WAIT, NOKIA_START, NOKIA_RESET, NOKIA_SCORE};
int Nokia_Tick(int state){
   switch(state){
               case NOKIA_WAIT:
                           nokia_lcd_clear();
                           nokia_lcd_set_cursor(10, 1);
                           nokia_lcd_write_string("THE PACMAN", 1);

                           nokia_lcd_set_cursor(0, 21);
                           nokia_lcd_write_string("Press Start Button to play", 1);

                           nokia_lcd_render();

                           if((~PINA & 0xC0) == 0x40){
                                state = NOKIA_START;
                           }else{
                                state = NOKIA_WAIT;
                           }
                           break;
               case NOKIA_START:
                           nokia_lcd_clear();
                           nokia_lcd_set_cursor(10, 1);
                           nokia_lcd_write_string("THE PACMAN", 1);

                           nokia_lcd_set_cursor(0, 21);
                           nokia_lcd_write_string("Here is the game stage :D", 1);

                           nokia_lcd_render();
           
                           if((~PINA & 0xC0) == 0x80){
                                state = NOKIA_RESET;
                            }else if((~PINA & 0xC0) == 0xC0){
                                state = NOKIA_SCORE;
                            }else{
                                state = NOKIA_START;
                            }
                            break;
               case NOKIA_RESET:
                           nokia_lcd_clear();
                           nokia_lcd_set_cursor(10, 1);
                           nokia_lcd_write_string("THE PACMAN", 1);

                           nokia_lcd_set_cursor(0, 21);
                           nokia_lcd_write_string("Your score will become 0 if reset", 1);
                           nokia_lcd_render(); 
           
                           if((~PINA & 0xC0) == 0x80){
                                state = NOKIA_START;
                            }else{
                                state = NOKIA_RESET;
                            }
                            break;
               case NOKIA_SCORE:
                           nokia_lcd_clear();
                           nokia_lcd_set_cursor(10, 1);
                           nokia_lcd_write_string("THE PACMAN", 1);

                           nokia_lcd_set_cursor(0, 21);
                           nokia_lcd_write_string("Check LCD for current score", 1);
                           nokia_lcd_render(); 
                           
                           if((~PINA & 0xC0) == 0x40){
                                state = NOKIA_START;
                            }else if((~PINA & 0xC0) == 0x80){
                                state = NOKIA_RESET;
                            }else{
                                state = NOKIA_SCORE;
                            }
                            break;
               default:
                           state = NOKIA_WAIT;
                           break;
   }
            
   switch(state){
               case NOKIA_START:
                           break;
           
               case NOKIA_WAIT:
                           break;
           
               case NOKIA_SCORE:
                           break;
           
               case NOKIA_RESET:
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
    
    ADC_init();
    nokia_lcd_init();
    LCD_init();
    LCD_DisplayString(1, "Welcome to the  PACMAN");

    static task task1, task2, task3, task4;
    task *tasks[] = { &task1, &task2, &task3, &task4};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = -1;

    task1.state = start;
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick;

    task2.state = JS_START;
    task2.period = 10;
    task2.elapsedTime = task2.period;
    task2.TickFct = &JS_Tick;
            
    task3.state = C_START;
    task3.period = 10;
    task3.elapsedTime = task3.period;
    task3.TickFct = &Cursor_Tick;       
     
    task4.state = NOKIA_WAIT;
    task4.period = 10;
    task4.elapsedTime = task4.period;
    task4.TickFct = &Nokia_Tick;           
    
    TimerSet(10);
    TimerOn();

    unsigned short i;
    while (1) {
        for (i = 0; i < numTasks; ++i) {
                if (tasks[i]->elapsedTime == tasks[i]->period) {
                        tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                        tasks[i]->elapsedTime = 0;
                }
                tasks[i]->elapsedTime += 10;
        }

        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
