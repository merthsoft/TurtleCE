# Turtle CE
This is a program made to generate art. It has a spirograph mode and a random walk mode, and various different color modes.

# Controls:
<pre>
VARS - see the current settings and palette.
DEL - exit.
CLEAR - clear the screen to black.

Modes:
Mode - Toggle between modes without changing and values
Y= - Spirograph mode, resets, 
            angle = 9;
            speed = 6;
Window - Random walk mode, resets,
            angle = 45;
            speed = 5;

Colors:
Multiply - Toggle change turtle colors
Divide - Toggle palette shift
Comma - Rainbow palette
Sin - Monochromatic color->black (current color value used as H in HSV, V is spectrum), switches palette direction if already selected
Cos - Monochromatic color->white (current color value used as H in HSV, S is spectrum), switches palette direction if already selected
Tan - Cycles through a spectrum of palettes
Apps - Monochromatic gray, switches palette direction if already selected
Left paren - Subtracts 16 from the main color the palette is built from
Left paren - Adds 16 from the main color the palette is built from

Turtles:
Up - Increase speed
Down - Decrease speed
Left - Decrease angle
Right - Increase angle
0 - Teleport turtles to center
Decimal - Move turtles to random location
1 - Set the turtles color to index 1 of the palette
2 - Give turtles a random color
3 - Spread the palette across the number of turtles
4 - angle = 9; speed = 6;
5 - angle = 60; speed = 20;
6 - angle = 90; speed = 20;
7 - angle = 45; speed = 5;
8 - angle = 180; speed = 10;
9 - angle = 1; speed = 1;
(-) Negative - Give turltles random speed and angle
Plus - Add a turtle
Minus - Remove a turtle
Enter - Pause or Run the turtles
</pre>

Video:
https://youtu.be/zjMOiD62u7w

![Turtles](https://merthsoft.com/spiro/turtle.png)
![Tendrils](https://merthsoft.com/spiro/tendrils.png)
![Shifting](https://merthsoft.com/spiroshifting.png)
![Squiggles](https://merthsoft.com/spirosquiggles.png)
