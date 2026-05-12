# Snake Game for Arduino

This is a Snake game implemented for Arduino Uno using an 8x8 dot matrix display (-1088BS) and a joystick.

## Components Required
- Arduino Uno
- 8x8 LED Dot Matrix Display (-1088BS, with MAX7219 driver)
- Joystick Module (with URX, URY, SW pins)

## Hardware Connections

### Dot Matrix (-1088BS)
- DIN (Data In) → Arduino Pin 12
- CLK (Clock) → Arduino Pin 11
- CS (Chip Select) → Arduino Pin 10
- VCC → Arduino 5V
- GND → Arduino GND

### Joystick
- URX → Arduino Analog Pin A1
- URY → Arduino Analog Pin A0
- SW (Switch/Button) → Arduino Digital Pin 2
- VCC → Arduino 5V
- GND → Arduino GND

### Detailed Wiring Steps
1. Connect the dot matrix module:
   - Locate the pins on the -1088BS module (usually labeled).
   - Connect DIN to Arduino pin 12.
   - Connect CLK to Arduino pin 11.
   - Connect CS to Arduino pin 10.
   - Connect VCC to Arduino 5V pin.
   - Connect GND to Arduino GND pin.

2. Connect the joystick module:
   - Connect URX (usually the X-axis output) to Arduino A0.
   - Connect URY (Y-axis output) to Arduino A1.
   - Connect SW (button) to Arduino pin 2.
   - Connect VCC to Arduino 5V.
   - Connect GND to Arduino GND.

Ensure all connections are secure. Use jumper wires for breadboard setup.

## Software Requirements
- Arduino IDE (download from https://www.arduino.cc/en/software)
- LedControl library (install via Arduino IDE Library Manager)

## Installation
1. Install the LedControl library in Arduino IDE:
   - Go to Sketch > Include Library > Manage Libraries
   - Search for "LedControl" and install it.

2. Open the `snake.ino` file in Arduino IDE.

3. Select the board: Tools > Board > Arduino AVR Boards > Arduino Uno

4. Select the port: Tools > Port > (your Arduino port)

5. Upload the sketch: Sketch > Upload (or Ctrl+U)

## How to Play
- Use the joystick to control the snake:
  - Tilt right: Move snake right
  - Tilt left: Move snake left
  - Tilt up: Move snake up
  - Tilt down: Move snake down
- Avoid going backwards (opposite direction) - this will end the game.
- Eat the food (blinking dot) to grow the snake.
- The snake wraps around the edges of the matrix (toroidal movement).
- Game over conditions:
  - Going backwards
  - Snake collides with itself
- On game over, the border of the matrix lights up.

## Troubleshooting
- If the display doesn't work, check the connections to the dot matrix.
- Adjust the `JOY_THRESHOLD` constant if the joystick sensitivity is too high/low.
- If the game runs too fast/slow, adjust the `delay(300)` in the loop.

## Code Overview
- The snake is represented as an array of positions.
- Food is randomly placed on empty spots.
- Joystick input is read and mapped to directions.
- Collision detection checks for self-collision.
- On game over, the border LEDs are lit.