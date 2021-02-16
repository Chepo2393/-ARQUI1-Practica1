#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <RBD_Timer.h>
RBD::Timer timer;

int pinCS = 10; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numeroMatricesHorizontales = 2;
int numeroMatricesVerticales = 1;

Max72xxPanel matriz = Max72xxPanel(pinCS, numeroMatricesHorizontales, numeroMatricesVerticales);

//Pines de los Botones
const int tipo_mensaje = 13;
const int dire_mensaje = 12;
const int vel_mensaje = 11;
const int btn_modo = 9;
const int arriba = 7;
const int izq = 6;
const int abajo = 5;
const int der = 4;
const int start = 3;

//TEXTO GAME OVER
long inicio, fin;
long inicio_m, fin_m;
int presiono = 1;
int presiono_mensaje = 1;
int presiono_juego = 1;
String mensaje = " Game Over ";
String mensaje_mensaje = " *TP1 - GRUPO 2 - SECCION B* ";
String mensaje_pausa = "PAUSA";
int tiempo2 = 15; //milisegundos
const int espacio = 1; //1 columna de espacio entre cada led
const int espacio_LxL = 6;
const int ancho_LxL =  5 + espacio_LxL;
const int ancho = 5 + espacio;
int Ultimo_Modo;


//Variables del Juego
int MODO = 1;
bool mode = false;
int velocidad = 49;
int puntos = 0;
int tiempo = 350;
int longitud = 2;

//Variables del snake
int x[15], y[15];
int coordenadas [15][7];
int LED_Actual, LED_Siguiente, mov;
int count = 1;


//Variables de la comida
int flag_comida = 1;
int x_comida, y_comida;

void setup() {
  //Declaracion de los Pines para los Botones
  Serial.begin(9600); //
  pinMode(arriba, INPUT);
  pinMode(abajo, INPUT);
  pinMode(izq, INPUT);
  pinMode(der, INPUT);
  pinMode(start, INPUT);
  pinMode(btn_modo, INPUT);
  pinMode(vel_mensaje, INPUT);
  pinMode(tipo_mensaje, INPUT);
  pinMode(dire_mensaje, INPUT);

  //Configuracion de los paneles
  matriz.setIntensity(7);
  matriz.setPosition(1, 0, 0);
  matriz.setPosition(0, 1, 0);
  matriz.setRotation(0, 1);
  matriz.setRotation(1, 1);

  //Punto de origen del snake
  for ( LED_Actual = 0; LED_Actual < longitud; LED_Actual++ ) {
    x[LED_Actual] = 8;
    y[LED_Actual] = 1;
  }
  LED_Siguiente = 0;
}

void loop() {
  switch (MODO) {
    case 0: JUEGO_SNAKE(); break; //juego snake
    case 1: SELECCION_NIVEL(); break; //seleccion de nivel
    case 2: PAUSA(); break; //Pausa
    case 3: MENSAJE(); break; //Mensaje del grupo
  }
}

void MODO_MENSAJE() {
  if (digitalRead(btn_modo) == 1 && presiono_mensaje == 1) {
    inicio_m = millis();
    presiono_mensaje = 0;
  } else if (presiono_mensaje == 0) {
    fin_m = millis();
    if ((fin_m - inicio_m) > 1500) {
      MODO = 3;
      fin_m = 0; inicio_m = 0;
      presiono_mensaje = 1;
      matriz.fillScreen(LOW);
      delay(100);
    }
  }
}

void MODO_JUEGO() {
  if (digitalRead(btn_modo) == 1 && presiono_juego == 1) {
    inicio_m = millis();
    presiono_juego = 0;
  } else if (presiono_juego == 0) {
    fin_m = millis();
    if ((fin_m - inicio_m) > 1500) {
      if (Ultimo_Modo == 0) {
        MODO = 0;
      } else if (Ultimo_Modo == 1) {
        MODO = 1;
      } else if (Ultimo_Modo == 2) {
        MODO = 2;
      }
      fin_m = 0; inicio_m = 0;
      presiono_juego = 1;
      matriz.fillScreen(LOW);
      delay(100);
    }
  }
}

void MENSAJE() {
  if (digitalRead(dire_mensaje) == 0) {
    if (digitalRead(tipo_mensaje) == 0) {
      matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setRotation(0, 3);
      matriz.setRotation(1, 3);
      for (int i = 0; i < ancho * mensaje_mensaje.length() + matriz.width() - 1 - espacio ; i++) {
        matriz.fillScreen(LOW); //Apagamos todos los LED. función dedicada para limpiar la pantalla.
        //LETRA POR LETRA
        int letra = i / ancho;
        int x = (matriz.width() - 1) - i % ancho;
        int y = (matriz.height() - 8) / 2; //sirve para centrar el texto verticalmente.
        while (x + ancho - espacio >= 0 && letra >= 0) {
          if (letra <  mensaje_mensaje.length()) {
            matriz.drawChar(x, y, mensaje_mensaje[letra], HIGH, LOW, 1); //llenamos el búfer de mapa de bits con la imagen, de la letra
          }
          letra--;
          x -= ancho;
        }
        if (digitalRead(tipo_mensaje) == 1) {
          break;
        }
        if ( digitalRead(dire_mensaje == 1)) {
          break;
        }
        MODO_JUEGO();
        matriz.write();
        if (digitalRead(vel_mensaje) == 0) {
          delay(15);
        } else {
          delay(90);
        }
      }
    } else  {
      matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setRotation(0, 3);
      matriz.setRotation(1, 3);
      for (int i = 0; i < mensaje_mensaje.length() - 1; i++) {
        matriz.fillScreen(LOW);
        matriz.drawChar(2, 0, mensaje_mensaje[i - 1], HIGH, LOW, 1); //display 2 izquierda
        matriz.drawChar(9, 0, mensaje_mensaje[i], HIGH, LOW, 1); //display 1 derecha
        matriz.write();
        if (digitalRead(tipo_mensaje) == 0) {
          break;
        }
        if (digitalRead(vel_mensaje) == 0) {
          delay(100);
        } else {
          delay(300);
        }
        MODO_JUEGO();
      }
    }
  } else {
    if (digitalRead(tipo_mensaje) == 0) {
      matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setRotation(0, 3);
      matriz.setRotation(1, 3);
      for (int i = ancho * mensaje_mensaje.length() + matriz.width() - 1 - espacio; i > 0 ; i--) {
        matriz.fillScreen(LOW); //Apagamos todos los LED. función dedicada para limpiar la pantalla.
        //LETRA POR LETRA
        int letra = i / ancho;
        int x = (matriz.width() - 1) - i % ancho;
        int y = (matriz.height() - 8) / 2; //sirve para centrar el texto verticalmente.
        while (x + ancho - espacio >= 0 && letra >= 0) {
          if (letra <  mensaje_mensaje.length()) {
            matriz.drawChar(x, y, mensaje_mensaje[letra], HIGH, LOW, 1); //llenamos el búfer de mapa de bits con la imagen, de la letra
          }
          letra--;
          x -= ancho;
        }
        if (digitalRead(tipo_mensaje) == 1) {
          break;
        }
        if ( digitalRead(dire_mensaje == 0)) {
          break;
        }
        MODO_JUEGO();
        matriz.write();
        if (digitalRead(vel_mensaje) == 0) {
          delay(15);
        } else {
          delay(90);
        }
      }
    } else  {
      matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
      matriz.setRotation(0, 3);
      matriz.setRotation(1, 3);
      for (int i = mensaje_mensaje.length() - 1; i > 0 ; i--) {
        matriz.fillScreen(LOW);
        matriz.drawChar(2, 0, mensaje_mensaje[i - 1], HIGH, LOW, 1); //display 2 izquierdo
        matriz.drawChar(9, 0, mensaje_mensaje[i], HIGH, LOW, 1); //display 1 derecha
        matriz.write();
        if (digitalRead(tipo_mensaje) == 0) {
          break;
        }
        if (digitalRead(vel_mensaje) == 0) {
          delay(100);
        } else {
          delay(300);
        }
        MODO_JUEGO();
      }
    }
  }
}

void PAUSA() {
  matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setRotation(0, 3);
  matriz.setRotation(1, 3);
  mensaje_pausa = "PAUSA / PUNTUACION: " + String(puntos);
  for (int i = 0; i < ancho * mensaje_pausa.length() + matriz.width() - 1 - espacio ; i++) {
    if (digitalRead(start) == 1) {
      MODO = 0;
      matriz.fillScreen(LOW);
      break;
    }
    matriz.fillScreen(LOW); //Apagamos todos los LED. función dedicada para limpiar la pantalla.
    //LETRA POR LETRA
    int letra = i / ancho;
    int x = (matriz.width() - 1) - i % ancho;
    int y = (matriz.height() - 8) / 2; //sirve para centrar el texto verticalmente.
    while (x + ancho - espacio >= 0 && letra >= 0) {
      if (letra <  mensaje_pausa.length()) {
        matriz.drawChar(x, y, mensaje_pausa[letra], HIGH, LOW, 1); //llenamos el búfer de mapa de bits con la imagen, de la letra
      }
      letra--;
      x -= ancho;
    }
    matriz.write();
    delay(tiempo2);
  }
  delay(250);
  MODO_MENSAJE();
  Ultimo_Modo = 2;
}

void SELECCION_NIVEL() {
  if (digitalRead(izq) == 1 && velocidad < 52) {
    velocidad++;
    tiempo = tiempo -50;
  } else if (digitalRead(der) == 1 && velocidad > 49) {
    velocidad--;
    tiempo = tiempo +50;
  }
  matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setRotation(0, 3);
  matriz.setRotation(1, 3);
  matriz.fillScreen(LOW); //Apagamos todos los LED. función dedicada para limpiar la pantalla.
  matriz.drawChar(2, 0, '0', HIGH, LOW, 1); //llenamos el búfer de mapa de bits con la imagen, de la letra
  matriz.drawChar(9, 0, int(velocidad), HIGH, LOW, 1);
  matriz.write();
  if (digitalRead(start) == 1 && presiono == 1) {
    inicio = millis();
    presiono = 0;
  } else if (presiono == 0) {
    fin = millis();
    if ((fin - inicio) > 1500) {
      MODO = 0;
      fin = 0; inicio = 0;
      presiono = 1;
    }
  }
  delay(150);
  matriz.fillScreen(LOW);
  MODO_MENSAJE();
  Ultimo_Modo = 1;
}

void JUEGO_SNAKE() {
  if (digitalRead(start) == 1) {
    MODO = 2;
  }

  matriz.setPosition(1, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setPosition(0, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setRotation(0, 1);
  matriz.setRotation(1, 1);

  LED_Actual = LED_Siguiente;
  LED_Siguiente = Proximo(LED_Actual);

  Coordenadas_rdm();//Generacion de cooordenadas random
  matriz. drawPixel(x_comida, y_comida, HIGH);//Enciende la comida



  Choque(x[LED_Actual], y[LED_Actual]);//Virficacion choque contra cuerpo.
  matriz.drawPixel(x[LED_Actual], y[LED_Actual], HIGH); //Enciende el primer led, cabeza del snake


  matriz.write(); // Dibuja en los paneles

  delay(tiempo);//Velocidad del snake

  if ( ! LED_ocupado(LED_Siguiente) ) {
    matriz.drawPixel(x[LED_Siguiente], y[LED_Siguiente], LOW); // Apaga el ultimo led de la cola del snake
    Vacio(x[LED_Siguiente], y[LED_Siguiente]);//almacena la posicion donde se encuentra el snake respecto la matriz 8*16
    Comio(x[LED_Actual], y[LED_Actual]);
  }
  if (digitalRead(arriba) == 1) {//Lectura de los botones
    mov = 0;
  } else if (digitalRead(abajo) == 1) {
    mov = 1;
  } else if (digitalRead(izq) == 1) {
    mov = 2;
  } else if (digitalRead(der) == 1) {
    mov = 3;
  }
  switch ( mov ) {//Movimiento dependendiendo de los botones
    case 0: x[LED_Siguiente] = constrain(x[LED_Actual] + 1, 0, matriz.width() - 1); y[LED_Siguiente] = y[LED_Actual]; break;
    case 1: x[LED_Siguiente] = constrain(x[LED_Actual] - 1, 0, matriz.width() - 1); y[LED_Siguiente] = y[LED_Actual]; break;
    case 2: y[LED_Siguiente] = constrain(y[LED_Actual] + 1, 0, matriz.height() - 1); x[LED_Siguiente] = x[LED_Actual]; break;
    case 3: y[LED_Siguiente] = constrain(y[LED_Actual] - 1, 0, matriz.height() - 1); x[LED_Siguiente] = x[LED_Actual]; break;
  }
  //Verificacion si choca contra las paredes
  if (x[LED_Siguiente] == 0 || x[LED_Siguiente] == (matriz.width() - 1) || y[LED_Actual] == 0 || y[LED_Actual] == (matriz.height() - 1)) {
    mensajes();
  }
  MODO_MENSAJE();
  Ultimo_Modo = 0;
}

//Verifica si el led esta ocupado para la cabeza del snake
boolean LED_ocupado(int LED_ActualA) {
  for ( int LED_ActualB = 0 ; LED_ActualB < longitud; LED_ActualB++ ) {
    if ( LED_ActualA != LED_ActualB ) {
      if ( equal(LED_ActualA, LED_ActualB) ) {
        return true;
      }
    }
  }

  return false;
}

//Generacion de coordinadas random para la comida
void Coordenadas_rdm() {
  if (flag_comida == 1) {
    x_comida = random(1, 14);
    y_comida = random(1, 6);
    flag_comida = 0;
    //Serial.println(x);
    //Serial.println(y);
  }
}

//Verificacion si la cabeza de la snake llego a la comida. +longitud, +velocidad
void Comio(int x, int y) {
  if (x == x_comida && y == y_comida) {
    //Amatriz.drawPixel(x, y, LOW);
    longitud++;
    tiempo = tiempo - 50;
    puntos++;
    flag_comida = 1;
  }
}

//Validacion si choca contra el cuerpo del snake
int Choque(int x, int y) {
  if (coordenadas[x][y] == 1) {
    mensajes();
  } else {
    coordenadas[x][y] = 1;
  }
}

int mensajes() {
  matriz.setPosition(1, 1, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setPosition(0, 0, 0); //Para empezar a escribir posicionamos la matriz en la 0,0 * Define cómo se ordenan las pantallas. La primera pantalla (0)
  matriz.setRotation(0, 3);
  matriz.setRotation(1, 3);
  mensaje += " | PUNTUACION: " + String(puntos);
  for (int i = 0; i < ancho * mensaje.length() + matriz.width() - 1 - espacio ; i++) {
    matriz.fillScreen(LOW); //Apagamos todos los LED. función dedicada para limpiar la pantalla.
    //LETRA POR LETRA
    int letra = i / ancho;
    int x = (matriz.width() - 1) - i % ancho;
    int y = (matriz.height() - 8) / 2; //sirve para centrar el texto verticalmente.
    while (x + ancho - espacio >= 0 && letra >= 0) {
      if (letra <  mensaje.length()) {
        matriz.drawChar(x, y, mensaje[letra], HIGH, LOW, 1); //llenamos el búfer de mapa de bits con la imagen, de la letra
      }
      letra--;
      x -= ancho;
    }
    matriz.write();
    delay(tiempo2);
  }
}

//Libera el espacio de la matriz de led
int Vacio(int x, int y) {
  coordenadas[x][y] = 0;
}

//Determina cual es el proxima posicion para mover la snake
int Proximo(int LED_Actual) {
  return (LED_Actual + 1) % longitud;
}

//Verificacion si dos led's tienen la misma posicion dentro de la matriz
boolean equal(int LED_ActualA, int LED_ActualB) {
  return x[LED_ActualA] == x[LED_ActualB] && y[LED_ActualA] == y[LED_ActualB];
}
