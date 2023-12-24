/*
There are 2 games in this sketch. There is Freddie Says and Simon Says. If you hold down lights 1 & 2 and then switch the game on it will startup Simon Says
otherwise it will play Freddie Says which is a simpler game made to teach cognitive amd fine motor skills to little ones.

The game will cycle through the lights and the flash once for Freddie Says or twice for Simon Says.
*/

const int buttonPins[] = { 3, 4, 5, 6, 7 };  // Pins for the 5 buttons
const int ledPins[] = { 8, 9, 10, 11, 12 };        // Pins for the corresponding LEDs
const int buzzer = 2;                        // Pin for the buzzer
int gameMode = 0;

void setup() {
  for (int i = 0; i < 5; i++) {     // set the pins up as either buttons of lights (INPUT_PULLUP or OUTPUT) INPUT_PULLUP
    pinMode(buttonPins[i], INPUT_PULLUP); //  INPUT_PULLUP activates the internal pullup resistors to get an accurate read
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  randomSeed(analogRead(0)); // Make the random numbers more random by using a read off an unconnected pin
  if (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == LOW) { // Check if button 1 & 2 are pressed at startup. If they are set the game mode to 1 (currently simon says)
    gameMode = 1;
  }
}

void loop() {

  switch (gameMode) { // Check game mode. If it's 1 then play Simon Says otherwise start Freddie Says
    case 1:
      simonSays();
      break;
    default:
      freddieSays();
      break;
  }
}

void playBuzzer(int button) { // Play a different pitch tone depending on which button it is. Really the argument variable should be called light but it's all the same thing.
  if (button == 3) {
    tone(buzzer, 950);
    delay(300);
    noTone(buzzer);
  } else if (button == 4) {
    tone(buzzer, 850);
    delay(300);
    noTone(buzzer);
  } else if (button == 5) {
    tone(buzzer, 750);
    delay(300);
    noTone(buzzer);
  } else if (button == 6) {
    tone(buzzer, 650);
    delay(300);
    noTone(buzzer);
  } else if (button == 7) {
    tone(buzzer, 550);
    delay(300);
    noTone(buzzer);
  }
}

void animateLights(int flashes) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPins[i], HIGH);
    playBuzzer(ledPins[i]);
    delay(50);
    digitalWrite(ledPins[i], LOW);
  }
  for (int i = 0; i < flashes; i++) {
    for (int i = 0; i < flashes; i++) {
      digitalWrite(ledPins[i], HIGH);
    }
    playBuzzer(ledPins[1]);
    for (int i = 0; i < flashes; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(300);
  }
}




void freddieSays() { // A simplified game where a light turns on and when you press it another turns on somewhere else.
  int currentButtonState;
  int lastButtonState = HIGH;
  int randomButton = random(5);
  animateLights(1);
  delay(500);
  digitalWrite(ledPins[randomButton], HIGH);  // Turn on the initial light


  for (;;) {
    currentButtonState = digitalRead(buttonPins[randomButton]);

    if (currentButtonState == LOW && lastButtonState == HIGH) {
      digitalWrite(ledPins[randomButton], LOW);  // Turn off the light for the pressed button
      playBuzzer(ledPins[randomButton]);         // Play the buzzer sound

      // Generate a new random button
      randomButton = random(5);
      delay(500);
      digitalWrite(ledPins[randomButton], HIGH);  // Turn on the light for the new button
    }

    lastButtonState = currentButtonState;
  }
}

void simonSays() { // Classic Simon Says but with 5 buttons just because they were sent in the pack
  Serial.begin(9600);
  Serial.println("simon says started");
  animateLights(2);
  delay(1000);
  const int levelsInGame = 50;
  int simonSaid[levelsInGame];  //initialize the levels array
  bool gameOver = false;

  // populate the array with random 'colours'
  for (int i = 0; i < levelsInGame; i++) {
    simonSaid[i] = random(5);
  }

  // the loop function runs over and over again forever
  for (;;) {
    gameOver = false;

    for (int i = 1; i <= levelsInGame && !gameOver; i++) {  // for each level
      Serial.println();
      Serial.print("Level ");
      Serial.println(i);
      if (i > 1) { delay(1000); }

      for (int g = 0; g < i; g++) {  //play the colors through to that level
        Serial.print("Color number ");
        Serial.print(g + 1);
        Serial.print(" in this sequence is the button on D");
        Serial.println(simonSaid[g]);
        digitalWrite(ledPins[simonSaid[g]], HIGH);
        playBuzzer(ledPins[simonSaid[g]]);
        delay(500);
        digitalWrite(ledPins[simonSaid[g]], LOW);
        delay(100);
      }

      for (int g = 0; g < i && !gameOver; g++) {  //check the colors you enter for that level
        Serial.print("Press the correct button for number ");
        Serial.print(g + 1);
        Serial.print(" in Simon's sequence which should be the one connected to pin D");
        Serial.println(simonSaid[g]);
        int sensorButton3 = digitalRead(buttonPins[0]);
        int sensorButton4 = digitalRead(buttonPins[1]);
        int sensorButton5 = digitalRead(buttonPins[2]);
        int sensorButton6 = digitalRead(buttonPins[3]);
        int sensorButton7 = digitalRead(buttonPins[4]);

        int buttonPressed = 0;  //a button that does not exist

        while (sensorButton3 == HIGH && sensorButton4 == HIGH && sensorButton5 == HIGH && sensorButton6 == HIGH && sensorButton7 == HIGH) {  //until a button is pressed keep checking all of them
          sensorButton3 = digitalRead(buttonPins[0]);
          sensorButton4 = digitalRead(buttonPins[1]);
          sensorButton5 = digitalRead(buttonPins[2]);
          sensorButton6 = digitalRead(buttonPins[3]);
          sensorButton7 = digitalRead(buttonPins[4]);
        }

        delay(200);  // a kind of debounce  - maybe later swap it for a check that all buttons have been depressed for 100ms?

        //record which one was pressed
        if (sensorButton3 == LOW) {
          buttonPressed = 0;
        } else if (sensorButton4 == LOW) {
          buttonPressed = 1;
        } else if (sensorButton5 == LOW) {
          buttonPressed = 2;
        } else if (sensorButton6 == LOW) {
          buttonPressed = 3;
        } else {
          buttonPressed = 4;
        }

        Serial.print("Button that was pressed is = ");  //print out the pressed button
        Serial.println(buttonPressed);
        digitalWrite(ledPins[simonSaid[g]], HIGH);  //turn on the button light
        playBuzzer(ledPins[simonSaid[g]]);          //make the sound of the button pressed - right or wrong
        digitalWrite(ledPins[simonSaid[g]], LOW);   //turn off button light

        //check to see if it is what should be pressed
        if (buttonPressed == simonSaid[g]) {
          Serial.println("Correct button - well done");
        } else {
          Serial.println("Wrong button was pressed.");
          Serial.println("Game finished - sorry. Better luck next time.");
          tone(buzzer, 350);  //play game over low tone on buzzer
          for (int i = 0; i < 5; i++) {
            digitalWrite(ledPins[i], HIGH);
          }
          delay(2000);
          noTone(buzzer);
          for (int i = 0; i < 5; i++) {
            digitalWrite(ledPins[i], LOW);
          }
          delay(2000);
          gameOver = true;  // exit the loop when the game is over
        }
      }
    }
  }
}
