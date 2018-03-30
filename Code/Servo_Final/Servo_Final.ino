#include <Servo.h>//Librería para control de servomotores
#include <DHT.h>//Librería para control de módulo DHT

Servo srvH;//Servomotores ubicados en el hombro
Servo srvC;//Servomotor ubicado en el codo
Servo srvM1;//Servomotor para el movimiento vertical de la muñeca
Servo srvM2;//Servomotor para el movimiento horizontal de la muñeca
Servo srvP;//Servomotor para la apertura de la pinza
DHT dht(2, DHT22);//Definimos el pin de conexión y tipo de módulo DHT que se este utilizando

int mtrd=13;//Movimiento del motor hacia la derecha
int mtri=12;//Movimiento del motor hacia la izquierda

int ledrgb_R=8;//Led RGB Rojo
int ledrgb_V=7;//Led RGB Verde
int ledrgb_A=4;//Led RGB Azul

int input;//Valor leído desde la aplicación de AppInventor
int valor;//Valor angular para escritura en los servos
int tiempo;//Variable para control de tiempo en DHT
//Ángulos iniciales de posición de los servos
int posH=10;
int posC=150;
int posM1=0;
int posM2=90;
int posP=90;

//Inicialización de variables para módulo DHT
float t;//Temperatura
float h;//Humedad

int grados=2;//Incremento angular que se da en cada repetición del bucle
String dato;//Dato leído por la consola serial
char caso;//Indica el servomotor que ejecutará el movimiento

boolean demo=false;
boolean recolectado=false;

void setup()
{
  Serial.begin(9600);// Inicio de la comunicación Serial
  //Declaración de las salidas PWM usadas por los servos  
  srvH.attach(11);
  srvC.attach(10);
  srvM1.attach(9);
  srvM2.attach(6);
  srvP.attach(5);
  
  dht.begin();//Inicializamos el sensor DHT
  //Declaración de los pines de salida y entrada
  pinMode(mtrd,OUTPUT);
  pinMode(mtri,OUTPUT);
  
  Serial.setTimeout(5);//Determina la cantidad máxima de tiempo para esperar recibir un dato serial
  
  //Posicionar a los servomotores en la posición inicial
  srvH.write(posH);
  srvC.write(posC);
  srvM1.write(posM1);
  srvM2.write(posM2);
  srvP.write(posP);  
}

void loop()
{
  while(Serial.available()>0)//Lectura de la cadena de datos por puerto Serial
  {
    delay(10);
    char c=Serial.read();
    dato+=c;
  }
  
  if(dato.length()>0)//Se ejecuta cuando se haya recibido un dato
  {
    input=dato.substring(1,dato.length()).toInt();//Toma el valor numérico incluido en la cadena
    //Conversión del valor a ángulos 
    if(input > -90 && input < 90)
    {
      valor=map(input, -90, 90, 0, 180);    
    }
    
    caso=dato.charAt(0);//Determina el servomotor a realizar el movimiento
    //Apaga el motor en caso que se encuentre funcionando
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
    
    switch (caso)//Indica a que servomotor enviar el dato de control
    {
      case 'A'://Servomotor del Hombro
        /* Este Servomotor se controla mediante la aplicación
        de un acelerómetro, por lo cual varía su programación
        con los demás servomotores*/  
        if(valor>122)//Delimita el alcance máximo del servomotor
        {
          valor=122;
        }
        
        if(valor>110)//El acelerómetro se encuentra inclinado hacia arriba
        {
          posH+=1;//Aumentar la posición del hombro
        } 
        if(valor<70)//El acelerómetro se encuentra inclinado hacia arriba
        {
          posH-=1;//Disminuir la posición del hombro
        } 
        srvH.write(posH);//Mover al servomotor
        break;
        
      case 'B'://Motor de la base
        if(valor<60)//Acelerómetro inclinado hacia la izquierda
        {
          //Mover el motor a la izquierda
          digitalWrite(mtri,HIGH);
          digitalWrite(mtrd,LOW);
        }
        if(valor>120)//Acelerómetro inclinado hacia la derecha
        {
          //Mover el motor a la derecha
          digitalWrite(mtrd,HIGH);
          digitalWrite(mtri,LOW);
        }
        break;  
        
      case 'C'://Servomotor Muñeca 2
        if(valor>posM2)//El valor recibido es mayor a la posición
        {
          posM2+=grados;//Aumentar el valor de la posición
        }
 
        if(valor<posM2)//El valor recibido es menor a la posición
        {
          posM2-=grados;//Disminuir el valor de la posición
        }
        srvM2.write(posM2);//Mover al servomotor
        break;
        
      case 'D'://Servomotor Codo
        if(valor>posC)//El valor recibido es mayor a la posición
        {
          posC+=grados;//Aumentar el valor de la posición     
        } 
        if(valor<posC)//El valor recibido es menor a la posición
        {
          posC-=grados;//Disminuir el valor de la posición
        }
        srvC.write(posC);//Mover al servomotor
        break;
        
      case 'E'://Servomotor Muñeca 1
        if(valor>posM1)//El valor recibido es mayor a la posición
        {
          posM1+=grados;//Aumentar el valor de la posición
        } 
        if(valor<posM1)//El valor recibido es menor a la posición
        {
          posM1-=grados;//Disminuir el valor de la posición
        } 
        srvM1.write(posM1);//Mover al servomotor
        break;  
        
      case 'F'://Servomotor pinza
        srvP.write(valor);//Mover al servomotor
        break;
        
      case 'M': 
        if(!recolectado)
        {
          srvH.write(40);
          srvC.write(150);
          srvM1.write(0);
          srvM2.write(90);
          srvP.write(90);
          delay(1000);
          
          for(int i=40; i>10; i--)
          {
            srvH.write(i);
            delay(50);
          }
          
          delay(1000);
          
          srvP.write(150); 
          
          delay(1000);
          
          srvH.write(40);
          
          posH=40;
          posC=150;
          posM1=0;
          posM2=90;
          posP=150;
          
          recolectado=true;
        }
        
        break;
        
      case 'N':
        if(recolectado)
        {
          srvH.write(40);
          srvC.write(150);
          srvM1.write(0);
          srvM2.write(90);
          srvP.write(150);
          delay(1000);
          
          for(int i=40; i>10; i--)
          {
            srvH.write(i);
            delay(50);
          }
          
          srvP.write(90);      
          
          delay(1000);
          
          srvH.write(40);
          
          delay(1000);
          posH=40;
          posC=150;
          posM1=0;
          posM2=90;
          posP=90;
          
          recolectado=false;
        }        
        break;
        
      case 'O':        
      
        srvH.write(40);
        srvC.write(150);
        srvM1.write(0);
        srvM2.write(90);
        srvP.write(90);  
        
        delay(1000);
        
        if(demo)
        {
          digitalWrite(mtri,LOW);
          digitalWrite(mtrd,HIGH);
          delay(1000);
          digitalWrite(mtri,LOW);
          digitalWrite(mtrd,LOW);
          demo=false;
        }
        else
        {
          digitalWrite(mtrd,LOW);
          digitalWrite(mtri,HIGH);
          delay(1000);
          digitalWrite(mtri,LOW);
          digitalWrite(mtrd,LOW);
          demo=true;
        }
        
        delay(500);
        
        srvM2.write(0);
        delay(500);
        srvM2.write(180);
        delay(1000);
        srvM2.write(90);
        
        delay(500);
        
        srvM1.write(
        
        delay(500);
        
        for(int i=40; i>10; i--)
        {
          srvH.write(i);
          delay(50);
        }
        
        delay(500);
        
        for(int i=0; i<4; i++)
        {
          if(i%2==0)
          {
            srvP.write(150);            
          }
          else
          {
            srvP.write(90);
          }
          delay(500);
        }
        delay(1000);
        
        posH=10;
        posC=150;
        posM1=0;
        posM2=90;
        posP=90;
        break;
    }
    dato="";//Borrar el valor del dato para recibir uno nuevo
    //Condicional para detectar errores de lectura en el módulo DHT   
  }  
  if(tiempo==167)//Luego de 2 segs
  {
    h=dht.readHumidity();//Leer humedad del DHT
    t=dht.readTemperature();//Leer temperatura del DHT
    if (isnan(h) || isnan(t))//Algún valor recibido del DHT no es numérico 
    {
      Serial.println("Failed to read from DHT sensor!");//Mostrar mensaje de error
    }
    else
    {
      //Mostrar los datos recibidos
      Serial.print("Temperatura: "); 
      Serial.print(t);
      Serial.println(" °C ");
      Serial.print("Humedad: "); 
      Serial.print(h);
      Serial.print(" %\t");
      
      if(t>=27)//Temperatura es caliente
      {
        digitalWrite(ledrgb_R,HIGH);//Encender led caliente
        digitalWrite(ledrgb_V,LOW);
        digitalWrite(ledrgb_A,LOW);
      }
      
      if(t<=10)//Temperatura es fría
      {
        digitalWrite(ledrgb_R,LOW);
        digitalWrite(ledrgb_V,LOW);
        digitalWrite(ledrgb_A,HIGH);//Encender led frío
      }
      
      if(t>10&&t<27)//Temperatura es templada
      {
        digitalWrite(ledrgb_R,LOW);
        digitalWrite(ledrgb_V,HIGH);//Encender led templado
        digitalWrite(ledrgb_A,LOW);
      }
    }
    tiempo=0;//Reiniciar el tiempo de espera
  }
  
  tiempo++;//Aumenta en uno cada 12 ms
  delay(12);//Retardo de 12 ms
}
