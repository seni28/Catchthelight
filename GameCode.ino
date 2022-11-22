/*
Adimaker project : Catch the light

Date : 20/11/2022

Written by : Ines CABRAL
Contact : ines.cabral--toucheboeuf@student.junia.com


*/


#define NUMBER_BUTTONS 8
#define NUMBER_LED 10

#include <PCF8574.h>

// SCREEN
//#include <TFT_eSPI.h>
#include <SPI.h>
//TFT_eSPI tft = TFT_eSPI();

// Multiplexer to add more pins ;)
PCF8574 buttonRed(0x20);
PCF8574 buttonBoth(0x21);
PCF8574 buttonBlue(0x22);

// Inialisation of all the pin used

int a = 0;
int ledstart = 4 ;
int ButtonPin[8] = {P0, P1, P2, P3, P4, P5, P6, P7};

//int buttonPinRed[10] = {4, 16, 22, 12, 19, 21, 17, 13 }; 
int ledPinRed[10] = {23, 25 , 27, 29, 31, 33, 35, 37, 39, 41};
int ledStateRed[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

int ledPinBlue[10] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40 };
int ledStateBlue[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

int buttonState = HIGH;
int buttonStartState = HIGH;

int buttonStateMultiplexerRed = HIGH;
int buttonStateMultiplexerBlue= HIGH;
int buttonStateMultiplexerBoth = HIGH;

// Scoring vairables
int scoreRed =0;
int scoreBlue =0;
long randomNumber;
int randomLedRed;
int randomLedBlue;

unsigned long lastActionTime = 0; 
long  startingGame=0;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 1000;    // the debounce time; increase if the output flickers
unsigned long gameTime = 40000; // Duration of one game



void setup() {

// screen setup

  for(int i=0; i<NUMBER_LED; i++) {
    pinMode(ledPinRed[i], OUTPUT);
    pinMode(ledPinBlue[i], OUTPUT);
  }
  for(int i=0; i<NUMBER_BUTTONS; i++) {
    buttonRed.pinMode(ButtonPin[i], INPUT_PULLUP);
    buttonBlue.pinMode(ButtonPin[i], INPUT_PULLUP);
  }

  // Red Buttons
  buttonBoth.pinMode(P0, INPUT_PULLUP);
  buttonBoth.pinMode(P1, INPUT_PULLUP);
  // Blue Buttons
  buttonBoth.pinMode(P2, INPUT_PULLUP);
  buttonBoth.pinMode(P3, INPUT_PULLUP);

  //Start Button
  buttonBoth.pinMode(P6, INPUT_PULLUP);
/*
for(int i=0; i<NUMBER_BUTTONS; i++) {
    changeLedToOnRed(i);
  }
*/
pinMode(ledstart, OUTPUT);
Serial.begin(9600);
Serial.println("Loading...");
digitalWrite(ledstart, HIGH);
 
}


/* ---- Here are a bunch of different functions called in the code ----- */


/* ---- LEDS ----- */

// Turning all the RED LED OFF
 void changeLedToOffRed(int pin) {
    digitalWrite(ledPinRed[pin], LOW);
    ledStateRed[pin] = 0;
  }

// Turning all the RED LED ON
  void changeLedToOnRed(int pin) {
    digitalWrite(ledPinRed[pin], HIGH);
    ledStateRed[pin] = 1;
  }

// Turning all the BLUE LED OFF
 void changeLedToOffBlue(int pin) {
    digitalWrite(ledPinBlue[pin], LOW);
    ledStateBlue[pin] = 0;
  }

// Turning all the LED ON
  void changeLedToOnBlue(int pin) {
    digitalWrite(ledPinBlue[pin], HIGH);
    ledStateBlue[pin] = 1;
  }


//  Selecting a random LED
void newRandom() {
  randomNumber = random(0,NUMBER_LED);
  randomLedRed = ledPinRed [randomNumber];
  randomLedBlue = ledPinBlue [randomNumber];
  Serial.print("New random : ");
  Serial.println(randomLedRed);
  Serial.println(randomLedBlue);
  
  
}

// Turning ON the LED selected earlier

void RandomLED() {
 if (millis() - lastActionTime > 5000 ) {
    lastActionTime = millis();
    // malus car le joueur a raté un appui ? => voir avec l'équipe ou le client
    RandomLEDOff();
    newRandom();  
    changeLedToOnRed(randomNumber);
    changeLedToOnBlue(randomNumber);
    //digitalWrite(ledPinRed[randomNumber], HIGH);
    //ledStateRed[randomNumber] = 1;
   
  }
}

void RandomLEDOff() {
  //randomLed = ledPinRed [randomNumber];
  changeLedToOffRed(randomNumber);
  changeLedToOffBlue(randomNumber);

  //digitalWrite(ledPinRed[randomNumber], LOW);
  //ledStateRed[randomNumber] = 0;
    //for (int i = 0; i < NUMBER_LED; i++){    }
}

void NewGame(){
  //buttonStartState = buttonBoth.digitalRead(P6);
  if (buttonStartState == LOW ){
    Serial.println("Here we Go !");
    a = 1;
  }
  else{
    Serial.println ("Still waiting for button start");
  }
}

void loop() {
// GAME TIME 

  buttonStartState = buttonBoth.digitalRead(P6);

  NewGame();

  if (a==1) {
  while (millis() - startingGame <= gameTime){

    // Call the function to turn on the seleted LED
    //RandomLED();


    for(int i=0; i<NUMBER_LED; i++) {
    //unsigned long currentTime = millis();
    changeLedToOnRed(i);
    changeLedToOnBlue(i);
    if ( (millis() - lastDebounceTime) > debounceDelay) {

      
      buttonState = digitalRead(ButtonPin[i]);
      buttonStateMultiplexerRed = buttonRed.digitalRead(ButtonPin[i]);
      buttonStateMultiplexerBlue = buttonBlue.digitalRead(ButtonPin[i]);
      buttonStateMultiplexerBoth = buttonBoth.digitalRead(ButtonPin[i]);

      // Player 1 : RED
      if ((buttonState == LOW) && (ledStateRed[i]== 1) )
      {    
        scoreRed = scoreRed + 1;
        changeLedToOffRed(i);
        Serial.print("Bouton rouge: ");
        Serial.print(i);
        Serial.print(" => Touché ! scoreRed=");
        Serial.println(scoreRed);
        
        

        lastDebounceTime = millis();
      }
    
      else if ( (buttonState == LOW) && (ledStateRed[i] == 0) )
      {
        RandomLEDOff();
        scoreRed = scoreRed - 1;
        //changeLedToOnRed(i);
        Serial.print("Bouton rouge: ");
        Serial.print(i);
        Serial.print(" => perdu : ");
        Serial.println (scoreRed);
        
    

        lastDebounceTime = millis();
      }

      if ((buttonState == LOW) && (ledStateRed[i]== 1) )
      {    
        scoreRed = scoreRed + 1;
        changeLedToOffRed(i);
        Serial.print("Bouton rouge: ");
        Serial.print(i);
        Serial.print(" => Touché ! scoreRed=");
        Serial.println(scoreRed);
        
        

        lastDebounceTime = millis();
      }
    
      else if ( (buttonState == LOW) && (ledStateRed[i] == 0) )
      {
        //RandomLEDOff();
        scoreRed = scoreRed - 1;
        //changeLedToOnRed(i);
        Serial.print("Bouton rouge: ");
        Serial.print(i);
        Serial.print(" => perdu : ");
        Serial.println (scoreRed);
        
    

        lastDebounceTime = millis();
      }



       
      // Player 2 : BLUE
      if ((buttonState == LOW) && (ledStateBlue[i]== 1) )
      {    
        scoreBlue = scoreBlue + 1;
        changeLedToOffBlue(i);
        Serial.print("Bouton bleu: ");
        Serial.print(i);
        Serial.print(" => Touché ! scoreRed=");
        Serial.println(scoreBlue);
    

        lastDebounceTime = millis();
      }
    
      else if ( (buttonState == LOW) && (ledStateRed[i] == 0) )
      {
        RandomLEDOff();
        scoreBlue = scoreBlue - 1;
        //changeLedToOnRed(i);
        Serial.print("Bouton bleu: ");
        Serial.print(i);
        Serial.print(" => perdu : ");
        Serial.println (scoreBlue);
        
      

        lastDebounceTime = millis();
      }
    }
  }
 }   

 if (millis() > gameTime){

  for(int i=0; i<NUMBER_LED; i++){
  changeLedToOnRed(i);
  changeLedToOnBlue(i);
 }
  Serial.print("Time's up!!");
  Serial.print("Score du joueur = ");
  Serial.println(scoreRed);
  
  }
  }
}

 
