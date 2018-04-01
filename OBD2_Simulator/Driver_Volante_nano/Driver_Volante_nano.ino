//------------------------------------//
//      Javier Mainar Val             //
//      alu.21406@usj.es              //
//------------------------------------//
//      MCU: Arduino Nano         //
//------------------------------------//

#define LEFT 2 
#define RIGTH 3
#define RELE 4
#define MOTOR_LEFT  5
#define MOTOR_RIGTH 6
 

int top_RIGTH = 0;
int top_LEFT  = 0;
int cicle_RIGTH = 0;
int cicle_LEFT  = 0;
int status_RELE = 0;
int status_RFU  = 0;
char command = '@';
String datos = ""; // $C105050#  $:inicio de cadena C:comando (@=Reset, L=Turn_LEFT, R=Turn_RIGTH, S=Stop) 1:Rele 1:RFU 50:Speed_LEFT (only Cycle) 50:Speed_RIGTH (only Cycle) #:fin de cadena //

void setup() {
  pinMode(LEFT, INPUT);
  pinMode(RIGTH, INPUT);
  pinMode(RELE, OUTPUT);
  //MOTOR_RIGTH no hace falta definir modo de pin al ser analogico
  //MOTOR_LEFT no hace falta definir modo de pin al ser analogico
  
  Serial.begin(9600);

  analogWrite(MOTOR_RIGTH,0);
  analogWrite(MOTOR_LEFT,0);
  digitalWrite(RELE, LOW);

  attachInterrupt(digitalPinToInterrupt(LEFT), stop_Motor, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGTH), stop_Motor, CHANGE);
  
  reset();
 
  
}

void loop() {

sendStatus(); // $C10505010#  $:inicio de cadena C:comando (@=Reset, L=Turn_LEFT, R=Turn_RIGTH, S=Stop) 1:Rele 1:RFU 50:Speed_LEFT (only Cycle) 50:Speed_RIGTH (only Cycle) 1:top_LEFT 0:top_RIGTH  #:fin de cadena //

while (Serial.available() > 0) 
{
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

            if(datos.length()>6)
            {                             
                  if (datos[0] == '@') reset();
                  else if (datos[0] == 'L') command='L';
                  else if (datos[0] == 'R') command='R';
                  else if (datos[0] == 'S') command='S';
          
                  status_RELE=(int)datos[1]-48;
                  digitalWrite(RELE, status_RELE); //RELE
                  status_RFU=(int)datos[2]-48;
                  //digitalWrite(RFU, (int)datos[2]-48); //RFU
                 
          
                  if(command=='L')
                  {
                     cicle_LEFT=arrayTOint(datos[3], datos[4]);
                     if(digitalRead(LEFT)==0)
                          analogWrite(MOTOR_LEFT, cycle2pwm(cicle_LEFT));
                     else
                          analogWrite(MOTOR_LEFT, 0);
                     analogWrite(MOTOR_RIGTH,0);
                  }else if(command=='R')
                  {
                     cicle_RIGTH=arrayTOint(datos[5], datos[6]);
                     if(digitalRead(RIGTH)==0)
                          analogWrite(MOTOR_RIGTH, cycle2pwm(cicle_RIGTH));
                     else 
                          analogWrite(MOTOR_RIGTH,0);
                     analogWrite(MOTOR_LEFT,0);
                  }else if(command=='S')
                     stop_Motor();
                  else
                     asm("nop");//reset();
            }
                        
      }
  }
}
  
}//LOPP

void stop_Motor()
{
  if(digitalRead(RIGTH))
     analogWrite(MOTOR_RIGTH,0);
  if(digitalRead(LEFT))
     analogWrite(MOTOR_LEFT,0);
     
  top_RIGTH = digitalRead(RIGTH);
  top_LEFT  = digitalRead(LEFT);
    
}

void sendStatus()
{
  Serial.print("$");
  Serial.print(command);
  Serial.print(status_RELE,DEC);
  Serial.print(status_RFU,DEC);
  Serial.print(cicle_LEFT,DEC);
  Serial.print(cicle_RIGTH,DEC);
  Serial.print(top_LEFT,DEC);
  Serial.print(top_RIGTH,DEC); 
  Serial.print("#");
  Serial.println();     
}

int arrayTOint(char var1, char var2){  
   return  (((int)var1-48)*10) + (int)var2-48;
}

int cycle2pwm(int cycle)
{
   /*
    value  |   Cycle
      0         0%
      63        25
      127       50%
      190       75%
      255       99%
   */

  if(cycle < 25)
      return 0;
  else if(cycle >= 25 && cycle < 50)
      return 63; 
  else if(cycle >= 50 && cycle < 75)
      return 127; 
  else if(cycle >= 75 && cycle < 99)
      return 190;
  else if(cycle >= 99 )
      return 255; 

}

void reset()
{
  int top_RIGTH = 0;
  int top_LEFT  = 0;
  int cicle_RIGTH = 0;
  int cicle_LEFT  = 0;
  int status_RELE = 0;
  int status_RFU  = 0;
  char command = '@';
  String datos = "";
  analogWrite(MOTOR_RIGTH,0);
  analogWrite(MOTOR_LEFT,0);
  digitalWrite(RELE, LOW);
  delay(1000);
}

