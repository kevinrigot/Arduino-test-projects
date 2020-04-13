#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define APP_NAME  "Numpad Mega - " __FILE__
#define APP_VERSION  "v0.1-" __DATE__ " " __TIME__

const byte ROWS = 5; //four rows
const byte COLS = 5; //four columns
//define the cymbols on the buttons of the keypads
#define AC 'A'
#define TIME 'T'
#define ALARM 'B'
#define SET 'S'
#define SOUND 'M'
#define UP 'U'
#define DOWN 'D'
#define NOT_AVAILABLE '_'

char hexaKeys[ROWS][COLS] = {
  {'=',AC,'0','.','+'},
  {'3','_','1','2','-'},
  {'6',TIME,'4','5','*'},
  {'9',ALARM,'7','8','/'},
  {'%',SET,DOWN,UP,SOUND}
};
byte rowPins[ROWS] = {31, 32, 33, 34, 35}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {41, 42, 43, 44, 45}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup(){
  Serial.begin(9600);
  Serial.println(APP_NAME);
  Serial.println(APP_VERSION);

  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

}

String currentLine = "";
String currentOperand = "";
long operand1 = 0;
long operand2 = 0;
char op = ' ';
float result = 0;

boolean isOperator(char customKey){
    return customKey == '/' || customKey == '*' || customKey =='+' || customKey =='-';
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
    if(customKey == AC){
      currentLine = "";
      currentOperand = "";
      operand1 = 0;
      operand2 = 0;
      op = ' ';
      result = 0;
      lcd.clear();
    }else if(customKey == '='){
      operand2 = currentOperand.toInt();
      switch(op){
        case '+': result = operand1 + operand2;
          break;
        case '-': result = operand1 - operand2;
          break;
        case '*': result = operand1 * operand2;
          break;
        case '/': result = operand1 / operand2;
          break;
      }
      currentLine = currentLine + customKey;
      lcd.setCursor(0,0);
      lcd.print(currentLine);
      lcd.setCursor(0,1);
      lcd.print(String(result));
      
    }else if(isDigit(customKey)){
      currentOperand = currentOperand + customKey;
      currentLine = currentLine + customKey;
      lcd.setCursor(0,1);
      lcd.print(currentLine);
    }else if(isOperator(customKey)){
      operand1 = currentOperand.toInt();
      currentOperand = "";
      op = customKey;
      currentLine = currentLine + customKey;
      lcd.setCursor(0,1);
      lcd.print(currentLine);
    }
    
  }
}
