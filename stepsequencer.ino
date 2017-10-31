#include <EEPROM.h>

//this is a comment to test gitHub
int buttonPin[6] = { 7, 8, 9, 10, 14, 15 };
int ledPin[4] = { 2, 3, 4, 5 };
boolean lastButtonState [4] = { LOW, LOW, LOW, LOW };
boolean buttonState [6] = { LOW, LOW, LOW, LOW, LOW, LOW };
boolean lastButton5State = LOW;
boolean lastButton4State = LOW;

boolean stepState [3][4] = {
  {true, true, true, true},
  {false, true, false, false},
  {false, true, true, false}
};

int tempo = 0;
int currentStep = 0;
int currentSequence = 0;
unsigned long lastStepTime = 0;

void setup() {
  loadFromEEPROM();
  pinMode(buttonPin[0], INPUT);
  pinMode(ledPin[0], OUTPUT);
  pinMode(buttonPin[1], INPUT);
  pinMode(ledPin[1], OUTPUT);
  pinMode(buttonPin[2], INPUT);
  pinMode(ledPin[2], OUTPUT);
  pinMode(buttonPin[3], INPUT);
  pinMode(ledPin[3], OUTPUT);

  pinMode(buttonPin[4], INPUT);
  pinMode(buttonPin[5], INPUT);

}

void loop() {
  checkForward();
  checkBackwards();
  sequence();
  checkButtons();
  setLeds();
  saveToEEPROM();
}

void checkForward()  {
  lastButton4State = buttonState[4];
  buttonState[4] = digitalRead(buttonPin[4]);
  if (buttonState[4] == HIGH && lastButton4State == LOW) {
    currentSequence = currentSequence + 1;
    if (currentSequence > 2)  {
      currentSequence = 0;
    }
    Serial.println(currentSequence);
  }
}

void checkBackwards() {
  lastButton5State = buttonState[5];
  buttonState[5] = digitalRead(buttonPin[5]);
  if (buttonState[5] == HIGH && lastButton5State == LOW) {
    currentSequence = currentSequence - 1;
    if (currentSequence < 0)  {
      currentSequence = 2;
    }
    Serial.println(currentSequence);
  }
}

void sequence() {

  tempo = 700;

  if (millis() > lastStepTime + tempo) {
    //digitalWrite(ledPin[currentStep], LOW);

    currentStep = currentStep + 1;
    if (currentStep > 3) {
      currentStep = 0;
    }
    if (stepState[0][currentStep] == true)  {
      usbMIDI.sendNoteOn(60, 90, 1);
    } else {
      usbMIDI.sendNoteOff(60, 0, 1);
    }
    if (stepState[1][currentStep] == true)  {
      usbMIDI.sendNoteOn(62, 90, 1);
    } else {
      usbMIDI.sendNoteOff(62, 0, 1);
    }
    if (stepState[2][currentStep] == true)  {
      usbMIDI.sendNoteOn(64, 90, 1);
    } else {
      usbMIDI.sendNoteOff(64, 0, 1);
    }

    lastStepTime = millis();

  }
}


void checkButtons() {
  for (int i = 0; i < 4 ; i++)  {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentSequence][i] == false) {
        stepState[currentSequence][i] = true;

      } else if (stepState[currentSequence][i] == true) {
        stepState[currentSequence][i] = false;


      }
    }
  }
}


void setLeds() {
  for (int i = 0; i < 4 ; i++)  {
    if (i == currentStep) {
      analogWrite(ledPin[i], 255);
    } else if (stepState[currentSequence][i] == true) {
      analogWrite(ledPin[i], 10);
    } else  {
      analogWrite(ledPin[i], 0);
    }
  }
}

void saveToEEPROM() {
/*  EEPROM.write(0, true);
  EEPROM.write(1, true);
  EEPROM.write(2, true);
  EEPROM.write(3, true);
  EEPROM.write(4, false);
  EEPROM.write(5, true);
  EEPROM.write(6, false);
  EEPROM.write(7, false);
  EEPROM.write(8, false);
  EEPROM.write(9, true);
  EEPROM.write(10, true);
  EEPROM.write(11, false);
*/
  int number = 0;
  for (int x = 0; x < 3 ; x++)  {
    for (int y = 0; y < 4 ; y++)  {
      EEPROM.write(number,stepState[x][y] );
      number++;
    }
  }

}
void loadFromEEPROM() {
  int number = 0;
  for (int x = 0; x < 3 ; x++)  {
    for (int y = 0; y < 4 ; y++)  {
      stepState[x][y] = EEPROM.read(number);
      number++;
    }
  }
}

