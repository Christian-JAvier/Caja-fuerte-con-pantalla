
#include <Wire.h>
#include <Keypad.h> // Controla el teclado
#include <LiquidCrystal.h>  //controla el LCD
#include <Servo.h>  //Control del servomotor
#include <EEPROM.h>  //COntrola E/S EEPROM
#define CERRAR 90 //Calibrar a gusto la pos del servomotor cerrado
#define ABRIR 180 //Calibrar a gusto la pos del servomotor abierto

//declaramos variables a utilizar

Servo seguro; //servomotor
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //pantalla LCD
const int pinBuzzer = 13;
const byte filas = 4;
const byte columnas = 4;
byte rowPins[filas] = {9, 10, 11, 12};
byte colPins[columnas] = {A1, A2, A3, A4};
char teclas[filas][columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
}; // declaracion de teclado
Keypad teclado = Keypad(makeKeymap(teclas), rowPins, colPins, filas, columnas);
char password[5]; //almacena la contraseña en eeprom
char ingreso; //ingreso de usuario
char passUser[4];
char confirmPass[4];

int i = 0;
int a; //aux

void setup(){
seguro.attach(A5); //pin del servomotor
seguro.write(CERRAR);
  lcd.begin(16, 2); //configuracion lcd 16x2 (col,row)
  seguro.write(90); //cerrar puerta
  lcd.setCursor(0, 0);
  lcd.print("  **PASSWORD**  ");
  lcd.setCursor(5, 1);
  lcd.print("____");
  
  if(EEPROM.read(4)!= 'Z'){
    EEPROM.write(0, '1');
    EEPROM.write(1, '2');
    EEPROM.write(2, '3');
    EEPROM.write(3, '4');
    EEPROM.write(4, 'Z');
  }
  for (int i = 0; i <= 4; i++){
  password[i] = EEPROM.read(i);
  }
}

void loop(){
leerIngreso(1);
  
}
