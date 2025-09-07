// Simon Says Game with ESP32 - Internal Pull-Up Version

#define NUM_LEDS 4
#define MAX_ROUNDS 20

int ledPins[NUM_LEDS]    = {14, 27, 26, 25};  // Red, Green, Blue, Yellow LEDs
int buttonPins[NUM_LEDS] = {32, 33, 18, 19};  // Buttons

int sequence[MAX_ROUNDS];   // stores LED sequence
int playerIndex = 0;        // player progress tracker
int roundLevel  = 1;        // start with 1 step
bool gameOver   = false;

void setup() {
  Serial.begin(115200);

  // Setup LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Setup Buttons with internal pull-ups
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(0)); // seed randomness
  generateSequence();
}

void loop() {
  if (gameOver) {
    resetGame();
  }

  // Show sequence for current round
  playSequence();

  // Wait for player input
  playerTurn();

  // Check if player completed sequence
  if (playerIndex == roundLevel) {
    roundLevel++;
    playerIndex = 0;
    delay(1000);
  }

  // Check win condition
  if (roundLevel > MAX_ROUNDS) {
    winGame();
  }
}

void generateSequence() {
  for (int i = 0; i < MAX_ROUNDS; i++) {
    sequence[i] = random(NUM_LEDS);
  }
}

void playSequence() {
  Serial.print("Round ");
  Serial.println(roundLevel);
  for (int i = 0; i < roundLevel; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);
    delay(500);
    digitalWrite(ledPins[led], LOW);
    delay(250);
  }
}

void playerTurn() {
  playerIndex = 0;
  while (playerIndex < roundLevel) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if (digitalRead(buttonPins[i]) == LOW) { // button pressed (LOW = pressed)
        // Light up LED for feedback
        digitalWrite(ledPins[i], HIGH);
        delay(300);
        digitalWrite(ledPins[i], LOW);

        if (i == sequence[playerIndex]) {
          playerIndex++;
        } else {
          gameOver = true;
          return;
        }
        delay(200); // debounce delay
      }
    }
  }
}

void resetGame() {
  Serial.println("Game Over! Restarting...");
  // Flash all LEDs as indication
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    delay(300);
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(300);
  }
  roundLevel = 1;
  playerIndex = 0;
  gameOver = false;
  generateSequence();
}

void winGame() {
  Serial.println("You Won the Game! 🎉");
  // Fancy LED pattern
  for (int k = 0; k < 10; k++) {
    int randLed = random(NUM_LEDS);
    digitalWrite(ledPins[randLed], HIGH);
    delay(200);
    for (int i = 0; i < NUM_LEDS; i++) {
      digitalWrite(ledPins[i], LOW);
    }
  }
  resetGame();
}
