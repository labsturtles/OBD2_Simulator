//------------------------------------//
//      Javier Mainar Val             //
//      alu.21406@usj.es              //
//------------------------------------//
//      MCU: Arduino Leonardo         //
//------------------------------------//

#include <DigiPotX9Cxxx.h>

#define RESET asm("jmp 0x0000")

#define INC 3 
#define UD  2

DigiPot AXIS_A(INC,UD,4); //EJE X = IZQUIERDO DERECHA
DigiPot AXIS_B(INC,UD,5); //EJE ROTACION Z = DERECHO ARRIBA
DigiPot AXIS_C(INC,UD,6);
DigiPot AXIS_D(INC,UD,7);

#define RELE_A 9 //AXIS_A/B
#define RELE_B 15 //AXIS_C/D

#define SALIDA_1 8 //NO CONECTADO
#define SALIDA_2 14 //BOTON 11 
#define SALIDA_3 16 //BOTON 12
#define ENTRADA_1 10 //NO CONECTADO

#define LED_ENGINE 18 // Activa el LED a nivel bajo "LOW"
#define BOTON_ENGINE  19


String datos=""; // $@101050505050#  $:inicio de cadena @:comando reset 1:BOTON_11 1:BOTON_12 1:RELE_A 0:RELE_B 50:AXIS_A 50:AXIS_B 50:AXIS_C 50:AXIS_D #:fin de cadena //
char command;
int axis_a=50;
int axis_b=50;
int axis_c=50;
int axis_d=50;
int blink=0;
 
void setup() {
  
  Serial.begin(115200);

  pinMode(INC, OUTPUT);
  pinMode(UD, OUTPUT);
  pinMode(RELE_A, OUTPUT);
  pinMode(RELE_B, OUTPUT);
  pinMode(SALIDA_1, OUTPUT);
  pinMode(SALIDA_2, OUTPUT);
  pinMode(SALIDA_3, OUTPUT);
  pinMode(ENTRADA_1, INPUT);
  pinMode(LED_ENGINE, OUTPUT);
  pinMode(BOTON_ENGINE, INPUT);

  digitalWrite(RELE_A, LOW);
  digitalWrite(RELE_B, LOW);
  digitalWrite(SALIDA_1, LOW);
  digitalWrite(SALIDA_2, LOW);
  digitalWrite(SALIDA_3, LOW);
  digitalWrite(LED_ENGINE, LOW);

  AXIS_A.increase(100);
  AXIS_B.increase(100);
  AXIS_C.increase(100);
  AXIS_D.increase(100);
  delay(20);
  AXIS_A.decrease(axis_a);
  AXIS_B.decrease(axis_b);
  AXIS_C.decrease(axis_c);
  AXIS_D.decrease(axis_d);
  delay(20);
  
  delay(3000);
  
  reset(); //Reinicioamos todo
  
}//SETUP


void loop() {

//while(1)Serial.println( digitalRead(ENTRADA_1));

if(digitalRead(BOTON_ENGINE)) 
       Serial.println("$P111111111111#"); //Boton=1;
else
       Serial.println("$p000000000000#"); //Boton=0;

blinkLED();

while (Serial.available() > 0) 
{
  
  if(digitalRead(BOTON_ENGINE)) 
       Serial.println("$P111111111111#"); //Boton=1;
  else
       Serial.println("$p000000000000#"); //Boton=0;


  if (Serial.read() == '$')
  {
      while(Serial.available() <1);
      command = Serial.read();
      if (command == '@') reset();
      else
      {
        
        datos="";
        do
        {
           datos+=command;
           while(Serial.available() <1);
           command=Serial.read();
         }while(command!='#');

        //Serial.println(datos);
        if (datos[0] == '@') reset();
        else if (datos[0] == 'l') blink=0;
        else if (datos[0] == 'L') blink=1;
        else if (datos[0] == 'B') blink=2;
      
        digitalWrite(SALIDA_2, (int)datos[1]-48); //BOTON 11
        digitalWrite(SALIDA_3, (int)datos[2]-48); //BOTON 12
        digitalWrite(RELE_A, (int)datos[3]-48);
        digitalWrite(RELE_B, (int)datos[4]-48);

        moveAxis(AXIS_A, &axis_a, arrayTOint(datos[5], datos[6]));
        moveAxis(AXIS_B, &axis_b, arrayTOint(datos[7], datos[8]));
        moveAxis(AXIS_C, &axis_c, arrayTOint(datos[9], datos[10]));
        moveAxis(AXIS_D, &axis_d, arrayTOint(datos[11], datos[12]));

      }
  }
}
 
         
}//LOOP

void moveAxis(DigiPot AXIS, int *var_axis, int valor) {  

  if(*var_axis<valor)  
  {    
    AXIS.increase(valor-*var_axis);
  }else if(*var_axis>valor)
  {  
    AXIS.decrease(*var_axis-valor);
  }
  
   *var_axis=valor;
}

int arrayTOint(char var1, char var2){  
   return  (((int)var1-48)*10) + (int)var2-48;
}

void blinkLED()
{
  if(blink==0)
      digitalWrite(LED_ENGINE, HIGH);
  if(blink==1)    
      digitalWrite(LED_ENGINE, LOW);
  else if(blink==2)
  {
    digitalWrite(LED_ENGINE, LOW);
    delay(500);
    digitalWrite(LED_ENGINE, HIGH);
    delay(500);
  }
}

void reset()
{
  blink=0;
  blinkLED();
  digitalWrite(RELE_A, LOW);
  digitalWrite(RELE_B, LOW);
  digitalWrite(SALIDA_1, LOW);
  digitalWrite(SALIDA_2, LOW);
  digitalWrite(SALIDA_3, LOW);
  moveAxis(AXIS_A, &axis_a, 50);
  moveAxis(AXIS_B, &axis_b, 50);
  moveAxis(AXIS_C, &axis_c, 50);
  moveAxis(AXIS_D, &axis_d, 50);

}

