# Lemurian Clean

### Descripción del Proyecto

Lemurian Clean es una solución autónoma y eficiente para la recogida de desechos en cuerpos de agua. Equipado con una cámara ESP-CAM y hélices para su movilidad, el robot navega por el agua recogiendo desechos con una cinta transportadora.

[![modelo3d](https://cdn.discordapp.com/attachments/764428029907697674/1312109408221921310/image.png?ex=674b4c9c&is=6749fb1c&hm=afc8e5852c0a428d3898343387bf875bf2933bb03627be2158ba6f2bbb948278& "modelo3d")](https://cdn.discordapp.com/attachments/764428029907697674/1312109408221921310/image.png?ex=674b4c9c&is=6749fb1c&hm=afc8e5852c0a428d3898343387bf875bf2933bb03627be2158ba6f2bbb948278& "modelo3d")

### Características Principales

**Cámara ESP-CAM:** Proporciona capacidad de visión para la navegación y detección de desechos.

**Propulsión con Hélices:** Permite un movimiento eficiente y controlado en el agua.

**Cinta Transportadora:** Recolecta desechos desde la superficie del agua y los almacena en un contenedor interno.

### Hardware del Proyecto

-  **ESP32-CAM:** Proporciona capacidad de visión para la navegación y detección de desechos.

-  **ESP32:** Utiliza un ESP32 para controlar los drivers y motores del robot, asegurando una operación eficiente y precisa.

-  **Motores y Hélices:** Para la propulsión del robot.

- **Cinta Transportadora:** Para la recogida de basura.

- **Chasis y Carcasa:** Estructura resistente al agua y flotante.

### Código del Robot
Podes encontrar el código completo del robot en la carpeta `Codigo LC` del repositorio.

#### Función de Callback para Bluetooth

```c
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
```
> Explicacion:
Esta función maneja diferentes eventos del Bluetooth, como la inicialización, conexión y recepción de datos. Es crucial para interactuar con dispositivos Bluetooth.

#### Función setup()
```c
void setup() {
  Serial.begin(115200);     // Inicializando la conexión serial para debug
  BT.begin("ESP32_HOA");  // Nombre de tu Dispositivo Bluetooth y en modo esclavo
  Serial.println("El dispositivo Bluetooth está listo para emparejar");
  BT.register_callback(callback_function);  // Registramos la función "callback_function" como función callback.

  pinMode(Motor1a, OUTPUT);
  pinMode(Motor1b, OUTPUT);
  pinMode(Motor2a, OUTPUT);
  pinMode(Motor2b, OUTPUT);
	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); 
	scale.set_scale(-471.497);
	scale.tare();
```
> Explicación:
> Inicialización Serial y Bluetooth: Se inicializa la comunicación serial para debug y la comunicación Bluetooth con el nombre "ESP32_HOA".
> Configuración de Pines: Se configuran los pines de los motores como salidas.
> Inicialización del Sensor: Se inicializa el sensor de celda de carga y se calibra la escala.

#### Función loop()
```c
void loop() {
  if (BT.available())  // Compruebe si recibimos algo de Bluetooth
  {
    int incoming = BT.read();  // Lee lo que recibimos
    Serial.print("Recibido: ");
    Serial.println(incoming);
    switch (incoming) {
      case 70:  // F en ASCII
        Adelante();
        break;
      case 66:  // B en ASCII
        Retroceder();
        break;
      case 76:     // L en ASCII
        Izquierda();
        break;
      case 82:  // R en ASCII
        Derecha();
        break;
      case 40:
        Serial.print("Primer Lectura:\t");
        Serial.print(scale.get_units(), 1);
        scale.power_down();
        scale.power_up();
        break;
      case 48:  // Velocidad 0
        veloc = 0;
        break;
      case 49:  // Velocidad 1
        veloc = 25;
        break;
      // Otros casos para velocidades
      default:
        Parar();
        break;
    }
  }
}

```
> Explicación:
Recepción de Comandos: La función `loop()` verifica si se han recibido datos por Bluetooth y actúa según el comando recibido. Dependiendo del comando, se ejecutan funciones para mover el robot hacia adelante, atrás, izquierda, derecha, o detenerse.

#### Funciones de Control de Motores
```c
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

```

> Explicación:
Control de Motores: Estas funciones controlan la dirección de los motores para mover el robot hacia adelante, atrás, izquierda, derecha, o detenerlo. Usan `digitalWrite()`para cambiar el estado de los pines.

### Diseño del PCB
Los archivos del diseño del PCB están ubicados en la carpeta `PCB`. Puedes descargar los archivos y el diagrama esquemático para fabricar y ensamblar tu propia placa.

![vdd](https://cdn.discordapp.com/attachments/764428029907697674/1313203542303051866/image.png?ex=674f479a&is=674df61a&hm=b0168fd87d1c546b8bf0e078cbec903b45f046837bd8b8e10d6ea27db589d3e3& "vdd")
> PCBv4 Capa Superior

![](https://cdn.discordapp.com/attachments/764428029907697674/1313203198168793128/image.png?ex=674f4748&is=674df5c8&hm=820f8bad63979d715dffeeb4abd32cc8a6a5bcb0a996e14bb644c7c0f2f4adc4&)
> PCBv4 Capa Inferior

### Uso
1. Encender el robot y asegurarse de que todos los sistemas están operativos.

3. Colocar el robot en el agua y activarlo.

5. Supervisar su operación a través de la cámara ESP-CAM y ajustar si es necesario.

#### Contacto
Para más información o preguntas sobre Lemurian Clean, puedes contactarnos en [ aruzamencristianmarcelo@gmail.com ], [damian34casas@gmail.com].
