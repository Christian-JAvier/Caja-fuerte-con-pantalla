
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

//metodos//

void leerIngreso(int a){
ingreso = teclado.getKey();
  if (ingreso != NO_KEY)
    switch (ingreso) {
  	 case 'A': 
    	if (evaluar(1)==1)
          correcto();
    	else{
    
              msgError();
    	}
    	reset();
    	break;
    
    case 'B':
      informacion();
   	 reset();
   	 break;
    
    case 'C':
    	cambioPass();
    	reset();
    	break;
    
    case 'D':
    	cleanlcd();
  		seguro.write(CERRAR);
    	lcd.setCursor(0, 0);
    	lcd.print("Cerrado");
   		delay(2000);
    	tone(pinBuzzer, 110);
        delay(500);
  		noTone(pinBuzzer);
   		reset();
    	break;
    
    case '*':
        lcd.setCursor(0, 0);
        lcd.print("Presionaste *");
        delay(2000);
        reset();
        break;
    
      case '#':
        lcd.setCursor(0, 0);
        lcd.print("Presionaste #");
        delay(2000);
        reset();
        break;
    
    default:
    if(a==1){
    	passUser[i] = ingreso;
      	printPass(passUser[i], 5 + i, 1);
    }
    if(a==2){
        confirmPass[i] = ingreso;
        printPass(confirmPass[i], 5 + i, 1);
        }
        i++;
        if (i > 3)
          i = 0;
  }
}

void cleanlcd() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

void printPass(char a, int columna, int fila){
	lcd.setCursor(columna, fila);
  lcd.print(a);
  delay(100);
  lcd.setCursor(columna, fila);
  lcd.print("*");
}

int evaluar(int a) {
  int j = 0;
  if (a==1){
  for (int i = 0; i <= 3; i++) {
    if (password[i] == passUser[i]) {
        j++;
    }
  }
  }
  if(a==2){
  for (int i = 0; i <= 3; i++) {
    if (passUser[i] == confirmPass[i]) {
        j++;
    }
  }
  }
  if (j == 4) {
    return j=1;
  }
  else {
    return j=0;
  }
}

void reset() {
  lcd.setCursor(0, 0);
  lcd.print("  **PASSWORD** ");
  lcd.setCursor(5, 1);
  lcd.print("____");
  for(int i =0;i<=3;i++){
  passUser[i]=NO_KEY;
  confirmPass[i]=NO_KEY;
  }
  i=0;
}

void msgError(){
  		tone(pinBuzzer, 260);
        delay(1000);
  		noTone(pinBuzzer);
        lcd.setCursor(0, 0);
        lcd.print("      ERROR      ");
        delay(2000);
  		
  }

void correcto(){
  		tone(pinBuzzer, 550);
        delay(1000);
  		noTone(pinBuzzer);
        lcd.setCursor(0, 0);
        lcd.print("   BIENVENIDO      ");
        //Abrir servomotor
        seguro.write(ABRIR);
        delay(2000);
  		
}

  
void informacion() {
  lcd.setCursor(0, 0);
  lcd.print("'A' para introdu");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("cir la pass        ");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("'C' para cambiar");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print(" la pass             ");
  delay(1500);
}



void cambioPass() {
   lcd.setCursor(0, 0);
  lcd.print("Cambio de pass    ");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("Introduce pass     ");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("anterior        ");
  delay(1500);  
  reset();
   while(passUser[3]==NO_KEY){
    leerIngreso(1);}
  if (evaluar(1) == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Introduce           ");
    delay(1500);
    lcd.setCursor(0, 0);
    lcd.print("la pass nueva            ");
    delay(1500);
    reset();
    while(passUser[3]==NO_KEY){
      leerIngreso(1);
  }
    lcd.setCursor(0, 0);
    lcd.print("Vuelve a intro     ");
    delay(1500);
    lcd.setCursor(0, 0);
    lcd.print("cirla           ");
    delay(1500);    
    lcd.setCursor(0, 0);
    lcd.print("  **PASSWORD**      ");
    lcd.setCursor(5, 1);
    lcd.print("____");
    lcd.setCursor(0, 0);
    i=0;
    while(confirmPass[3]==NO_KEY){
      leerIngreso(2);
  }
    if(evaluar(2)==1){
      // funcion de EEPROM
      for (int i = 0; i <= 3; i++) { 
         EEPROM.write(i, passUser[i]);
    }
      for (int i = 0; i <= 3; i++) { 
         password[i] = EEPROM.read(i);
  }
      lcd.setCursor(0, 0);
      lcd.print("Contrasena cam  ");
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.print("biada           ");
      delay(2000);
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("Error las pass   ");
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.print("no coinciden        ");
      delay(2000);
    }
    
  }
  else {
    msgError();
  }
  reset();
}

void passChange(){
 for(int i=0;i<=3;i++){
   password[i]=passUser[i];
 }
  
}

