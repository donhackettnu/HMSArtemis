
/* This is a work in progress.  The bulk of this functionality works.
 * But there are some pieces that have not been implimented.
 * Most notibly the station ID selector piece does not work.
 * You need to set it via a static variable via the main loop.
 */

// This is a standard library. 
#include <Keypad.h>



/*
Main button matrix for the console 
*/
const byte STATIONS = 5; // 5 different stations
const byte ROWS = 5; //four rows
const byte COLS = 5; //three columns

/* Station IDs 
    0 = test harness
    1 = helm
    2 = weapons 
    3 = engineering 
    4 = science 
    5 = comms 
*/
const int stationTestHarness = 0;
const int stationHelm        = 1;
const int stationWeapons     = 2;
const int stationEngineering = 3;
const int stationScience     = 4; 
const int stationComms       = 5; 

const int stationCommsB1     = 6; 
const int stationScienceB1   = 7; 


const int buttonShift = 3;
const int buttonTorpedo1 = 0;
const int buttonTorpedo2 = 1;
const int buttonTorpedo3 = 2;
const int buttonTorpedo4 = 3; 

// stationID = 7 , do nothing if the sation is not set
char keysBlank[ROWS][COLS] = {
    {'.','.','.','.','.'}, 
    {'.','.','.','.','.'}, 
    {'.','.','.','.','.'},
    {'.','.','.','.','.'},
    {'.','.','.','.','.'},
};

// stationID = 0  (stationPin0,stationPin1,stationPin2)
char keysTestHarness[ROWS][COLS] = {
    {'a','b','c','d','.'}, 
    {'e','f','g','h','.'}, 
    {'i','j','k','l','.'},
    {'m','n','o','p','.'},
    {'q','r','s','t','.'},
};

// stationID = 1 (stationPin0)
char keysHelm[ROWS][COLS] = {
    {'M','N','G','V','.'}, 
    {'k','l','H',' ','.'}, 
    {'C','D','I','.','.'},
    {'F','E','.','r','.'},
    {'.','.','.','.','.'},
};

//    {'M','k','c','F'}, 
//    {'N','l','D','E'}, 
//    {'G','H','I','y'},
//    {'V',' ','r','z'},
//    {'.','.','.','.'},

// stationID = 2  (stationPin1)
char keysWeapons[ROWS][COLS] = {
    {'E','C','F','D','R'}, 
    {'.','G','H','I','2'}, 
    {'1','.','4','3','Q'},
    {'K','M','k','O','T'},
    {'L','N','l','P','S'},
};

// stationID = 3 (stationPin0,stationPin1)
char keysEngineering[ROWS][COLS] = {
    {'1','3','5','2','.'}, 
    {'.','4','0','.','.'}, 
    {'6','8','9','7','.'},
    {'U','c','s','a','.'},
    {' ','e','w','d','.'},
};

//    {'w','e','d',' '}, 
//    {'s','c','a','U'}, 
//    {'5','3','2','1'},
//    {'9','8','7','6'},
//    {'0','4','.','.'},

// stationID = 4  (stationPin2)
char keysScience[ROWS][COLS] = {
    {'M','u','.','.','.'}, 
    {'U','y','.','.','.'}, 
    {'N','i','.','.','.'},
    {'.','.','.','.','.'},
    {'.','.','.','.','.'},
};

// stationID = 5 (stationPin0,stationPin2)
char keysComms[ROWS][COLS] = {
    {'.','r','0','.','.'}, 
    {'9','8','7','.','.'}, 
    {'3','2','1','.','.'},
    {'6','5','4','.','.'},
    {'.','.','.','.','.'},
}; 

// Comms on bridge one.  
// changed because the labelling is silly
char keysCommsB1[ROWS][COLS] = {
    {'0','r','.','.','.'}, 
    {'7','8','9','.','.'}, 
    {'4','5','6','.','.'},
    {'1','2','3','.','.'},
    {'.','.','.','.','.'},
}; 

// Science on bridge 1.  
char keysScienceB1[ROWS][COLS] = {
    {'p','o','.','.','.'}, 
    {'u','i','.','.','.'},     
    {'y','U','.','.','.'},
    {'.','.','.','.','.'},
    {'.','.','.','.','.'},
}; 

//N =  down arrow     zoom out
//M =  up arrow       zoom in
//U =  return key   = scan button 
//y =  y            = scan closest
//i =  i              scan previous
//u =  u              scan next

//    {'1','2','3','.'}, 
//    {'4','5','6','.'}, 
//    {'7','8','9','.'},
//    {'0','r','.','.'},
//    {'.','.','.','.'},

//byte colPins[COLS] = {10,16,14,15}; //connect to the column pinouts of the keypad
//byte rowPins[ROWS] = {5,6,7, 8, 9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10,16,14,15,18}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {9,8,7,6,5}; //connect to the row pinouts of the keypad


Keypad keypadTestHarness = Keypad( makeKeymap(keysTestHarness), rowPins, colPins, ROWS, COLS );
Keypad keypadHelm =        Keypad( makeKeymap(keysHelm), rowPins, colPins, ROWS, COLS );
Keypad keypadWeapons =     Keypad( makeKeymap(keysWeapons), rowPins, colPins, ROWS, COLS );
Keypad keypadEngineering = Keypad( makeKeymap(keysEngineering), rowPins, colPins, ROWS, COLS );
Keypad keypadScience =     Keypad( makeKeymap(keysScience), rowPins, colPins, ROWS, COLS );
Keypad keypadComms =       Keypad( makeKeymap(keysComms), rowPins, colPins, ROWS, COLS );
Keypad keypadCommsB1 =     Keypad( makeKeymap(keysCommsB1), rowPins, colPins, ROWS, COLS );
Keypad keypadScienceB1 =   Keypad( makeKeymap(keysScienceB1), rowPins, colPins, ROWS, COLS );

// dedicated buttons and thier states
int buttonPin[] = {1,0,2,3};
int buttonPinState[] = {LOW,LOW,LOW,LOW};
int buttonPinPreviousState[] = {LOW,LOW,LOW,LOW};


//  Station ID pins
int stationPin[] = {19,20,21};
int stationPinState[] = {LOW,LOW,LOW};
int stationID = 0;


void setup(){
  Serial.begin(9600);
  Keyboard.begin();
  //pinMode(shiftPin,INPUT);

  // read the station ID as part of setup.
  pinMode(stationPin[0],INPUT);
  pinMode(stationPin[1],INPUT);
  pinMode(stationPin[2],INPUT);

  pinMode(buttonPin[0],INPUT);
  pinMode(buttonPin[1],INPUT);
  pinMode(buttonPin[2],INPUT);
  pinMode(buttonPin[3],INPUT);

  readStationID();
}

void readStationID() {
  stationID = 0;
  stationPinState[0] = digitalRead(stationPin[0]);
  stationPinState[1] = digitalRead(stationPin[1]);
  stationPinState[2] = digitalRead(stationPin[2]);

  // yes, there is a better way of doing this through logical ORs...
  if (stationPinState[0] == HIGH) {
    stationID = stationID + 1;
  }
  if (stationPinState[1] == HIGH) {
    stationID = stationID + 2;
  }
  if (stationPinState[2] == HIGH) {
    stationID = stationID + 4;
  }  
  
  //Serial.print("---Station ID [");
  //Serial.print(stationID);
  //Serial.println("]");
  
  // set the station ID for now
  stationID=5;
}


// Some keystrokes need to be translated when the key is pressed
void actualKeyPress(char keyIn, int shiftKeyState){

   Serial.print("-- Key pressed [");
   Serial.print(keyIn);
   Serial.println("]");
   
   switch (keyIn) {
     case 'A': 
       // next client console
       Serial.print("F9");
       Keyboard.press(KEY_F9);
       break;
     case 'B':
       // previous client console
       Serial.print("F10");
       Keyboard.press(KEY_F10);
       break;
     case 'C':
       // screen front
       Serial.print("F2");
       Keyboard.press(KEY_F2);
       break;
     case 'D':
       // screen port 
       Serial.print("F3");
       Keyboard.press(KEY_F3);
       break;
     case 'E':
       // screen starboard
       Serial.print("F4");
       Keyboard.press(KEY_F4);
       break;
     case 'F':
       // screen aft
       Serial.print("F5");
       Keyboard.press(KEY_F5);
       break;
     case 'G': 
       // screen tactical
       Serial.print("F6");
       Keyboard.press(KEY_F6);
       break;
     case 'H':
       // screen sector 
       Serial.print("F7");
       Keyboard.press(KEY_F7);
       break;
     case 'I':
       // screen status 
       Serial.print("F8");
       Keyboard.press(KEY_F8);
       break;
     case 'K':
       // Torp -> Energy
       Serial.print("Shift U");
       Keyboard.press(KEY_LEFT_SHIFT);
       Keyboard.print("u");
       break;
     case 'L':
       // Energy -> Torp
       Serial.print("Shift I");
       Keyboard.press(KEY_LEFT_SHIFT);
       Keyboard.print("i");
       break;
     case 'M':
       // Up Arrow
       Serial.print("Up Arrow");
       Keyboard.press(KEY_UP_ARROW);
       break;
     case 'N':
       // Down Arrow 
       Serial.print("Down Arrow");
       Keyboard.press(KEY_DOWN_ARROW);
       break;
     case 'O':
       // Right Arrow 
       Serial.print("Right Arrow");
       Keyboard.press(KEY_RIGHT_ARROW);
       break;
     case 'P':
       // Left Arrow
       Serial.print("Left Arrow");
       Keyboard.press(KEY_LEFT_ARROW);
       break;
     case 'Q':
       // Shift 1
       Serial.print("Shift 1");
       //Keyboard.press(KEY_LEFT_SHIFT);
       //Keyboard.print("1");
       // weapons is broken, rebound as #5
       Keyboard.print("5");
       break;
     case 'R':
       // Shift 2
       Serial.print("Shift 2");
       //Keyboard.press(KEY_LEFT_SHIFT);
       //Keyboard.print("2");
       // game does not like this, rebound as #6
       Keyboard.print("6");
       break;
     case 'S':
       // Shift 3
       Serial.print("Shift 3");
       //Keyboard.press(KEY_LEFT_SHIFT);
       //Keyboard.print("3");
       Keyboard.print("7");
       break;
     case 'T':
       // Shift 4
       Serial.print("Shift 4");
       //Keyboard.press(KEY_LEFT_SHIFT);
       //Keyboard.print("4");
       Keyboard.print("8");
       break;
     case 'U':
       // Return Key
       Serial.print("Return Key");
       Keyboard.press(KEY_RETURN);
       break;
     case 'V':
       // Escape Key
       Serial.print("Escape Key");
       Keyboard.press(KEY_ESC);
       break;
 
     default:
       if (shiftKeyState == HIGH) {
         Serial.print("ShiftKey ");
         //Keyboard.set_modifier(MODIFIERKEY_SHIFT);
         Keyboard.press(KEY_LEFT_SHIFT);
       };
       Serial.print(keyIn);
       shiftKeyState=digitalRead(3);
       Serial.print(",");
       Serial.print(shiftKeyState);
       //don't forget to unpress the shift key

       Keyboard.print(keyIn);
       
   };
   //release all of the keys that were pressed.
   Keyboard.releaseAll();
};

// the torpedo can potentially be on for a long time
// So they need to be handled differenly
void torpedoButtonPresses() {
  // read the pushbutton:
  buttonPinState[0] = digitalRead(buttonPin[0]);
  buttonPinState[1] = digitalRead(buttonPin[1]);
  buttonPinState[2] = digitalRead(buttonPin[2]);
  buttonPinState[3] = digitalRead(buttonPin[3]);
  buttonPinState[4] = digitalRead(buttonPin[4]);

if (buttonPinState[0] == HIGH && buttonPinPreviousState[0] != HIGH) {
   Serial.println("You pressed button 0"); 
}
if (buttonPinState[1] == HIGH && buttonPinPreviousState[1] != HIGH) {
   Serial.println("You pressed button 1"); 
}
if (buttonPinState[2] == HIGH && buttonPinPreviousState[2] != HIGH) {
   Serial.println("You pressed button 2"); 
}
if (buttonPinState[3] == HIGH && buttonPinPreviousState[3] != HIGH) {
   Serial.println("You pressed button 3"); 
}

 
  buttonPinPreviousState[0] = buttonPinState[0]; 
  buttonPinPreviousState[1] = buttonPinState[1]; 
  buttonPinPreviousState[2] = buttonPinState[2]; 
  buttonPinPreviousState[3] = buttonPinState[3]; 
}  
   
void loop(){
  //char key = keypad.getKey();
  char key; 

   // Make the test harness dynamic
   // comment out this line for 'production' use
   //readStationID();
   //stationID=stationEngineering;
   //stationID=stationComms;

   // Bridge1 items
   //stationID=stationCommsB1;
   stationID=stationScienceB1; 

   //DEBUG Serial.println("-MainLoop");

   switch (stationID) {
     case stationTestHarness: 
       key = keypadTestHarness.getKey();
       // also read the other buttons state
       //torpedoButtonPresses();
       break;
     case stationHelm: 
       key = keypadHelm.getKey();
       break;
     case stationWeapons: 
       key = keypadWeapons.getKey();
       // also read the other buttons state
       //torpedoButtonPresses();
       break;
     case stationEngineering: 
       key = keypadEngineering.getKey();
       // also read the other buttons state
       buttonPinState[buttonShift] = digitalRead(buttonPin[buttonShift]);
       break;
     case stationScience: 
       key = keypadScience.getKey();
       break;
     case stationComms: 
       key = keypadComms.getKey();
       break;
     case stationCommsB1: 
       key = keypadCommsB1.getKey();
       break;
     case stationScienceB1: 
       key = keypadScienceB1.getKey();
       break;

     default:
       Serial.print("--- default stationID =");
       Serial.println(stationID);
       break;
   };
  

  if (key != NO_KEY) {
      Serial.print("StationID [");
      Serial.print(stationID);
      Serial.print("] ");
      // translate the keypress to what  is intended
      // but do not accidently press the shift key on torpedo on button[3].
      if (stationID == stationEngineering) {
        actualKeyPress(key,buttonPinState[buttonShift]);
      } else {
        actualKeyPress(key,LOW);
      };
      Serial.println("");
  }

  //delay(250);
  
}
