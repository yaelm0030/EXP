// הגדרת פינים לג'ויסטיק
const int joyX = A0;
const int joyY = A1;

// הגדרת פינים למטריצה (8 שורות ו-8 עמודות)
const int rowPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const int colPins[8] = {10, 11, 12, 13, A2, A3, A4, A5};

// גודל מקסימלי של הנחש
const int MAX_SNAKE_LENGTH = 64;

// מבנה נתונים לנקודה על המסך
struct Point {
  int x;
  int y;
};

Point snake[MAX_SNAKE_LENGTH];
int snakeLength = 1;

// כיוון תנועה נוכחי: 0=למעלה, 1=ימינה, 2=למטה, 3=שמאלה
int dirX = 1;
int dirY = 0;

Point food;
unsigned long lastMoveTime = 0;
const int moveInterval = 300; // מהירות המשחק (במילישניות)

// מטריצת התצוגה (0 = כבוי, 1 = דלוק)
byte screen[8][8] = {0};

void setup() {
  // הגדרת פיני המטריצה כפלט
for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);   // הפוך ממה שהיה
    digitalWrite(colPins[i], HIGH);  // הפוך ממה שהיה
  }
  
  // מיקום ראשוני של הנחש במרכז
  snake[0] = {3, 3};
  
  // יצירת אוכל ראשון
  generateFood();
  
  randomSeed(analogRead(A6)); // איתחול מספרים אקראיים מפין פנוי
}

void loop() {
  // 1. קריאת כיוון מהג'ויסטיק באופן רציף
  readJoystick();

  // 2. עדכון מיקום הנחש לפי הזמן שהוגדר
  if (millis() - lastMoveTime >= moveInterval) {
    moveSnake();
    lastMoveTime = millis();
  }

  // 3. ריענון התצוגה (מולטיפלקסינג) - חייב לרוץ כל הזמן בלולאה
  updateScreenBuffer();
  refreshDisplay();
}

void readJoystick() {
  int xVal = analogRead(joyX);
  int yVal = analogRead(joyY);

  // בדיקת כיוון ג'ויסטיק ומניעת חזרה לאחור ישירה
  if (xVal < 300 && dirX == 0) {       // ימינה או שמאלה (תלוי בכיון הפיזי של הרכיב)
    dirX = -1; dirY = 0;
  } else if (xVal > 700 && dirX == 0) {
    dirX = 1; dirY = 0;
  }
  
  if (yVal < 300 && dirY == 0) {
    dirX = 0; dirY = -1;
  } else if (yVal > 700 && dirY == 0) {
    dirX = 0; dirY = 1;
  }
}

void moveSnake() {
  // חישוב הראש החדש של הנחש
  int newX = snake[0].x + dirX;
  int newY = snake[0].y + dirY;

  // מעגליות המסך (אם יצא מהגבולות - יחזור מהצד השני)
  if (newX < 0) newX = 7;
  if (newX > 7) newX = 0;
  if (newY < 0) newY = 7;
  if (newY > 7) newY = 0;

  // בדיקה אם הנחש אכל את האוכל
  if (newX == food.x && newY == food.y) {
    if (snakeLength < MAX_SNAKE_LENGTH) {
      snakeLength++;
    }
    generateFood();
  }

  // הזזת חלקי הגוף של הנחש קדימה
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  // עדכון הראש
  snake[0].x = newX;
  snake[0].y = newY;
  
  // בדיקה עצמית (אם הנחש נשך את עצמו - המשחק מתאפס)
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      // איפוס המשחק
      snakeLength = 1;
      snake[0] = {3, 3};
      generateFood();
      break;
    }
  }
}

void generateFood() {
  bool validLocation = false;
  while (!validLocation) {
    food.x = random(0, 8);
    food.y = random(0, 8);
    
    // בדיקה שהאוכל לא מוגר על גוף הנחש
    validLocation = true;
    for (int i = 0; i < snakeLength; i++) {
      if (snake[i].x == food.x && snake[i].y == food.y) {
        validLocation = false;
        break;
      }
    }
  }
}

void updateScreenBuffer() {
  // ניקוי המסך בזיכרון
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      screen[r][c] = 0;
    }
  }

  // הזנת האוכל למסך
  screen[food.y][food.x] = 1;

  // הזנת הנחש למסך
  for (int i = 0; i < snakeLength; i++) {
    screen[snake[i].y][snake[i].x] = 1;
  }
}

// פונקציית המולטיפלקסינג שמדליקה את הלדים בפועל
void refreshDisplay() {
  for (int r = 0; r < 8; r++) {
    // שינוי 1: הפכנו מ-LOW ל-HIGH
    digitalWrite(rowPins[r], HIGH); 

    for (int c = 0; c < 8; c++) {
      if (screen[r][c] == 1) {
        // שינוי 2: הפכנו מ-HIGH ל-LOW
        digitalWrite(colPins[c], LOW); 
      } else {
        // שינוי 3: הפכנו מ-LOW ל-HIGH
        digitalWrite(colPins[c], HIGH);  
      }
    }

    delayMicroseconds(500); 

    // שינוי 4: הפכנו את הניקוי (HIGH ל-LOW, ו-LOW ל-HIGH)
    digitalWrite(rowPins[r], LOW);
    for (int c = 0; c < 8; c++) {
      digitalWrite(colPins[c], HIGH);
    }
  }
}