#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth no esta disponible!
#endif

#define Motor1a 32
#define Motor1b 33
#define Motor2a 25
#define Motor2b 26
const int LOADCELL_DOUT_PIN = 19;
const int LOADCELL_SCK_PIN = 18; 
int veloc;
HX711 scale;
BluetoothSerial BT;  // Objeto Bluetooth

void callback_function(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Inicializado SPP");
  } else if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Cliente conectado");
  } else if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Cliente desconectado");
  } else if (event == ESP_SPP_DATA_IND_EVT) {
    Serial.println("Datos recibidos"); 
    while (BT.available()) {     // Mientras haya datos por recibir
      int incoming = BT.read();  // Lee un byte de los datos recibidos
    }
  }
}
void setup() {
  Serial.begin(115200);     // Inicializando la conexión serial para debug
  BT.begin("ESP32_HOA");  // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function);  // Registramos la función "callback_function" como función callback.

  pinMode(Motor1a, OUTPUT);
  pinMode(Motor1b, OUTPUT);
  pinMode(Motor2a, OUTPUT);
  pinMode(Motor2b, OUTPUT);
  
  Serial.println("HX711 Demo");

  Serial.println("Inicializando lectura");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Lectura de escala:");
  Serial.print("Lectura Escala: \t\t");
  Serial.println(scale.read());

  Serial.print("Lectura Promedio: \t\t");
  Serial.println(scale.read_average(20));

  Serial.print("Valores obtenidos: \t\t");
  Serial.println(scale.get_value(5));

  Serial.print("Unidades obtenidas: \t\t");
  Serial.println(scale.get_units(5), 1);
            
  scale.set_scale(-471.497);
  scale.tare();

  Serial.println("Despues de configurada la escala:");

  Serial.print("Lectura base: \t\t");
  Serial.println(scale.read());

  Serial.print("Lectura Promedio: \t\t");
  Serial.println(scale.read_average(20));

  Serial.print("Valor obtenido: \t\t");
  Serial.println(scale.get_value(5));

  Serial.print("Unidades obtenidas: \t\t");
  Serial.println(scale.get_units(5), 1);
  Serial.println("Lecturas:");
}
void loop() {
  if (BT.available())  // Compruebe si recibimos algo de Bluetooth
  {
    int incoming = BT.read();  // Lee lo que recibimos
    Serial.print("Recibido: ");
    Serial.println(incoming);
    Serial.print("\n");
    switch (incoming) {

      case 70:  // F en ASCII
        Adelante();
        Serial.print("Adelante \n");
        break;

      case 66:  // B en ASCII
        Retroceder();
        Serial.print("Retroceder \n");
        break;

      case 76:     // L en ASCII
        Izquierda();
        Serial.print("Izquierda \n");
        break;

      case 82:  // R en ASCII
        Derecha();
        Serial.print("Derecha \n");
        break;

      case 40:
        Serial.print("Primer Lectura:\t");
        Serial.print(scale.get_units(), 1);
        Serial.print("\t| promedio:\t");
        Serial.println(scale.get_units(10), 5);
        scale.power_down();
        scale.power_up();
        break;

      case 48:  // Velocidad 0
        veloc = 0;
        break;
      case 49:  // Velocidad 1
        veloc = 25;
        break;
      case 50:  // Velocidad 2
        veloc = 51;
        break;
      case 51:  // Velocidad 3
        veloc = 76;
        break;
      case 52:  // Velocidad 4
        veloc = 102;
        break;
      case 53:  // Velocidad 5
        veloc = 127;
        break;
      case 54:  // Velocidad 6
        veloc = 153;
        break;
      case 55:  // Velocidad 7
        veloc = 179;
        break;
      case 56:  // Velocidad 8
        veloc = 200;
        break;
      case 57:  // Velocidad 9
        veloc = 225;
        break;
      case 113:  // q en ASCII
        veloc = 255;
        break;
      default:
        Parar();
        Serial.print("Parado \n");
        break;
    }
  }
}
void Adelante() {

  digitalWrite(Motor1a, HIGH);
  digitalWrite(Motor1b, LOW);
  digitalWrite(Motor2a, HIGH);
  digitalWrite(Motor2b, LOW);
}
void Retroceder() {

  digitalWrite(Motor1a, LOW);
  digitalWrite(Motor1b, HIGH);
  digitalWrite(Motor2a, LOW);
  digitalWrite(Motor2b, HIGH);
}
void Izquierda() {

  digitalWrite(Motor1a, LOW);
  digitalWrite(Motor1b, HIGH);
  digitalWrite(Motor2a, HIGH);
  digitalWrite(Motor2b, LOW);
}
void Derecha() {

  digitalWrite(Motor1a, HIGH);
  digitalWrite(Motor1b, LOW);
  digitalWrite(Motor2a, LOW);
  digitalWrite(Motor2b, HIGH);
}
void Parar() {
  digitalWrite(Motor1a, LOW);
  digitalWrite(Motor1b, LOW);
  digitalWrite(Motor2a, LOW);
  digitalWrite(Motor2b, LOW);
}