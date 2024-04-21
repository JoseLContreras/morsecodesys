
//PROGRAMA PARA DESCIFRAR CÓDIGO MORSE USANDO UN BOTÓN Y PANTALLA EN "MONITOR SERIE"
//FECHA: 27 JUNIO 2018
//AUTOR: JOSÉ LUIS CONTRERAS CORTÉS

unsigned long signal_len,t1,t2;   //tiempo por el cual se presiona el botón
int inputPin = 2;                 //pin de entrada para el botón
int ledPin = 4;                   //salida pin para LED
String code = "";                 //cadena donde se almacena alfabeto

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); //resistencia interna, conectadas directo a la alimentación, para simplificar circuito
  pinMode(ledPin,OUTPUT);
}

void loop()
{
NextDotDash:
  while (digitalRead(inputPin) == HIGH) {}
  t1 = millis();                           //tiempo al pulsar botón 
  digitalWrite(ledPin, HIGH);              //LED encendido con botón presionado 
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();                            //tiempo de lanzamiento del botón
  digitalWrite(ledPin, LOW);                //LED apagado al soltar botón
  signal_len = t2 - t1;                     //tiempo durante el cual se presiona el botón
  if (signal_len > 50)                      //se toma en cuenta rebote del pulsador
  {
    code += readio();                      //leer punto o guión
  }
  while ((millis() - t2) < 500)            //si el tiempo entre los botones presiona más de 0.5seg, omita el ciclo y vaya a la siguiente letra 
  {     
    if (digitalRead(inputPin) == LOW)
    {
      goto NextDotDash;
    }
  }
  convertor();                            //función para descifrar el código en el alfabeto
}

char readio()
{
  if (signal_len < 600 && signal_len > 50)
  {
    return '.';                          //si el botón presiona menos de 0.6seg, es un punto
  }
  else if (signal_len > 600)
  {
    return '-';                          //si el botón presiona más de 0.6seg, es un guión
  }
}

void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
                            };
  int i = 0;
  if (code == ".-.-.-")
  {
    Serial.print(".");         //para interrumpir          
  }
  else
  {
    while (letters[i] != "E")  
    {
      if (letters[i] == code)
      {
        Serial.print(char('A' + i));
        break;
      }
      i++;
    }
    if (letters[i] == "E")
    {
      Serial.println("<Señal NO coincide con letra>");  //si el código de entrada no coincide con ninguna letra, valida error
    }
  }
  code = "";                   //restablecer código a la cadena en blanco         
}
