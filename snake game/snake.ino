#include &lt;LedControl.h&gt;

// Pin definitions for MAX7219 (Dot Matrix -1088BS)
const int DIN_PIN = 12;  // Data In
const int CLK_PIN = 11;  // Clock
const int CS_PIN = 10;   // Chip Select

// Pin definitions for Joystick
const int axis0 = A0; // Analog pin connected to VRY of the joystick (Y-axis)
const int axis1 = A1; // Analog pin connected to VRX of the joystick (X-axis)
const int JOY_SW_PIN = 2;  // SW (Button)

// LedControl object
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// Define thresholds for up, down, left, right, and center
const int centerMin = 450; // Minimum value for the joystick to be considered 'centered'
const int centerMax = 550; // Maximum value for the joystick to be considered 'centered'

// Game variables
const int MATRIX_SIZE = 8;
int snake[64][2];  // Snake body positions (x, y)
int snakeLength = 1;
int food[2] = {3, 3};  // Initial food position
int direction = 0;     // 0: right, 1: up, 2: left, 3: down
bool gameOver = false;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize MAX7219
  lc.shutdown(0, false);  // Wake up
  lc.setIntensity(0, 8);  // Set brightness (0-15)
  lc.clearDisplay(0);     // Clear display

  // Initialize joystick button
  pinMode(JOY_SW_PIN, INPUT_PULLUP);

  // Initialize snake
  snake[0][0] = 0;  // Head x
  snake[0][1] = 0;  // Head y

  // Generate initial food
  generateFood();

  // Display initial state
  updateDisplay();

  Serial.println("Snake Game Started!");
}

void loop() {
  if (!gameOver) {
    readJoystick();
    moveSnake();
    checkCollision();
    checkFood();
    updateDisplay();
    delay(300);  // Game speed
  } else {
    // Game over: light up the border
    displayBorder();
    Serial.println("Game Over!");
  }
}

void readJoystick() {
  // Read the analog values from the joystick
  int xValue = analogRead(axis1); // X-axis (left/right)
  int yValue = analogRead(axis0); // Y-axis (up/down)

  int newDirection = direction;

  // Determine direction based on joystick position
  if (xValue > centerMax) {
    newDirection = 0;  // Right
  } else if (xValue < centerMin) {
    newDirection = 2;  // Left
  } else if (yValue > centerMax) {
    newDirection = 1;  // Up
  } else if (yValue < centerMin) {
    newDirection = 3;  // Down
  }

  // Prevent reversing direction
  if ((newDirection + 2) % 4 != direction) {
    direction = newDirection;
  }
}

void moveSnake() {
  // Move body
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];
  }

  // Move head
  switch (direction) {
    case 0: snake[0][0] = (snake[0][0] + 1) % MATRIX_SIZE; break;  // Right
    case 1: snake[0][1] = (snake[0][1] - 1 + MATRIX_SIZE) % MATRIX_SIZE; break;  // Up
    case 2: snake[0][0] = (snake[0][0] - 1 + MATRIX_SIZE) % MATRIX_SIZE; break;  // Left
    case 3: snake[0][1] = (snake[0][1] + 1) % MATRIX_SIZE; break;  // Down
  }
}

void checkCollision() {
  // Check self collision
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1]) {
      gameOver = true;
      return;
    }
  }
}

void checkFood() {
  if (snake[0][0] == food[0] && snake[0][1] == food[1]) {
    snakeLength++;
    generateFood();
  }
}

void generateFood() {
  bool valid = false;
  while (!valid) {
    food[0] = random(MATRIX_SIZE);
    food[1] = random(MATRIX_SIZE);
    valid = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i][0] == food[0] && snake[i][1] == food[1]) {
        valid = false;
        break;
      }
    }
  }
}

void updateDisplay() {
  lc.clearDisplay(0);

  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snake[i][1], snake[i][0], true);
  }

  // Draw food
  lc.setLed(0, food[1], food[0], true);
}

void displayBorder() {
  lc.clearDisplay(0);

  // Light up the border
  for (int i = 0; i < MATRIX_SIZE; i++) {
    lc.setLed(0, 0, i, true);        // Top row
    lc.setLed(0, MATRIX_SIZE-1, i, true);  // Bottom row
    lc.setLed(0, i, 0, true);        // Left column
    lc.setLed(0, i, MATRIX_SIZE-1, true);  // Right column
  }
}