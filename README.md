# CS120B-FinalProject

Pacman Game

Full Demo link: https://www.youtube.com/watch?v=EigdK7Y4cu4
Winning State of the Game: 
•	Complexity 1 (Motion Sensor): 
•	Complexity 2 (Nokia Screen): 
•	Complexity 3 (Joystick): 

Project Description:
The Pacman is a single player game built on breadboard. The game itself will be all displayed on the Nokia LCD screen, and the supporting messages (informing the player) will be displaying on the LCD screen. The player’s goal is to move the Pacman (cursor) by the Joysticks, help Pacman eating the potential fruits on the map. There will be two type of fruits, “+” and “?”, pluses will be plusing 5 pts and question mark may give or minus 5 pts away from the user. There will be button for the player to see his/her score at any time as they wish. The player is able to start or reset the game at any time as they wish as well. The game will be lost if the player hit the wall, and they must reset the game if they do so. There will be two methods resetting the game, either using motion sensor or just clicking reset button, but both ways require two clicking the button to reduce accidental resets. After player achieve 20 points for the Pacman, and they will receive a “thank message” from the Pacman.
User Guide:
Rules: 
•	The User controls the Pacman using a joystick. 
•	There will only be safe fruits without traps.
•	“+” fruits will be count as 5 points
•	“?” fruits will give or take away 5 points from the user
•	When the user collects 20 points the game will automatically end as a win
•	The game is lost if the player hit the wall
•	There is no time limit
•	Two methods resetting the game: motion sensor and reset button. Both need to click reset button a second time to reduce chance of accidental resets.

Controls: 
	Input:
•	Joystick: To control the Pacman, which also means this is how the user is able to move the cursor.
•	Reset Button: reset the game, and automatically change the current score back to 0
•	Start Button: Start the game and light up the LCDs to the game state
•	Motion Detector: reset the game (It is count as input because we eventually used its signal as inputting signal to trigger reset. But the component itself is outputting the signal detecting light, when that signal gets low, it will trigger that there is motion being detected)

Output:
•	Nokia LCD screen: display the game body
•	LCD screen: the score, inform the user about which state they are on
•	Score: the number of fruits the user collected

Special Considerations: 

•	Every button including joystick only need to be pressed once. Except for the reset button, the user has to click twice (or motion sensor + reset press) to reset the game. Because we don’t want an accidental reset clearing all the scores.
•	Press Start + Reset gets the user to view the score
•	DO NOT press down on the joystick, it will cut the power and then reset the game
•	Delays on button pressings

Source File Description:
•	main.c: main functions consisting all the tasks for the Pacman (Joystick, LCD, Nokia, Cursor, PIR) (Joysticks functions included and got reference from: http://maxembedded.com/2011/06/the-adc-of-the-avr/ and https://www.youtube.com/watch?v=6r3SHeN7F_k)
(PIR functions reference from:
Arduino with PIR Motion Sensor - Arduino Project Hub)
•	nokia5110.h: header file for Nokia LCD related codes. Code obtained from: LittleBuster/avr-nokia5110: Lightweight and easy to use library for PCD8544 lcd controller, tested on ATmega328P and Nokia 5110 LCD (github.com)
•	nokia5110.c: source code for Nokia LCD related codes. Code obtained from: LittleBuster/avr-nokia5110: Lightweight and easy to use library for PCD8544 lcd controller, tested on ATmega328P and Nokia 5110 LCD (github.com)
•	nokia5110_chars.h: necessary characters needed for the coding of Nokia LCD related codes. Code obtained from: LittleBuster/avr-nokia5110: Lightweight and easy to use library for PCD8544 lcd controller, tested on ATmega328P and Nokia 5110 LCD (github.com)
Component Visualization
 

Technologies Learned
•	setting up complexities (components) learning outside from the course:
o	How to implement Nokia LCD Screen:
	I learned about how to wire up the Nokia Screen onto my breadboard
	I learned how to implement the Nokia Screen source code, and build my project upon it
	I learned how different component has different voltage requirements, as for Nokia Screen it has to be using the 3.3 V instead of 5V.
o	How to implement Joystick:
	I learned how Joystick works as a component, it has a x axis and a y axis to indictor which direction the user pressed (up, down, left, right)
	I learned the two axises can be written into code using ADC
	I learned that there has a chip connect under the Joystick for easier wiring
	I learned how to draw the task diagram of the Joystick tasks
o	How to implement Motion Sensor:
	I learned how Motion Sensor works just like a light detector
	I learned how to connect my Motion Sensor to the breadboard
•	working large projects on VM:
o	I learned how to enlarge disk spaces
•	breadboard:
o	I learned how to accommodate different components that have different voltage requirements
o	Of course and space arrangements and component implementations
