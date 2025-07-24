| Objetivos Soportados | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| -------------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | -------- | -------- | -------- |

# Ejemplo: Sensor de Temperatura y Humedad DHT11

Este ejemplo demuestra cómo leer datos de temperatura y humedad de un sensor DHT11 usando ESP32.

## Descripción del Sensor DHT11

El DHT11 es un sensor digital básico y de bajo costo para temperatura y humedad. Utiliza un protocolo de comunicación de un solo cable.

**Especificaciones:**
- Temperatura: 0-50°C (precisión ±2°C)
- Humedad: 20-80% HR (precisión ±5% HR)
- Voltaje de operación: 3.3V-5V
- Corriente de operación: 0.3mA (midiendo) 60µA (en espera)

## Conexiones de Hardware

| Pin DHT11 | Pin ESP32 | Descripción |
| --------- | --------- | ----------- |
| VCC       | 3.3V      | Alimentación |
| GND       | GND       | Tierra |
| Data      | GPI0 15   | Señal de datos (configurable en el código) |

**Importante:** Conecta una resistencia pull-up de 4.7kΩ-10kΩ entre el pin de datos y VCC.

## Cómo Funciona

1. **Señal de Inicio**: El MCU envía una señal de inicio poniendo la línea de datos en bajo por 18ms, luego en alto por 30µs
2. **Señal de Respuesta**: El DHT11 responde poniendo la línea de datos en bajo por 80µs, luego en alto por 80µs
3. **Transmisión de Datos**: El DHT11 envía 40 bits de datos (humedad + temperatura + checksum)
4. **Formato de Datos**: 
   - Byte 0: Parte entera de la humedad
   - Byte 1: Parte decimal de la humedad
   - Byte 2: Parte entera de la temperatura
   - Byte 3: Parte decimal de la temperatura
   - Byte 4: Checksum (suma de los bytes 0-3)

## Configuración

El pin de datos del DHT11 está configurado para usar GPIO4 por defecto. Puedes cambiarlo modificando la definición `DHT11_GPIO` en el código:

```c
#define DHT11_GPIO    15 // Cambia esto al GPIO que desees
```

## Cómo usar el ejemplo

Antes de configurar y compilar el proyecto, asegúrate de establecer el chip objetivo correcto usando `idf.py set-target <nombre_del_chip>`.

### Hardware Requerido

* Una placa de desarrollo con cualquier SoC de Espressif (ej., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* Un cable USB para alimentación y programación
* Módulo sensor DHT11
* Resistencia pull-up de 4.7kΩ-10kΩ
* Cables jumper para las conexiones

### Diagrama de Conexión

```
ESP32          DHT11
-----          -----
3.3V    ---    VCC
GND     ---    GND
GPIO4   ---    Data (con resistencia pull-up de 4.7kΩ a VCC)
```

### Compilar y Flashear

Compila el proyecto y flashealo a la placa, luego ejecuta la herramienta de monitor para ver la salida serial:

Ejecuta `idf.py -p PUERTO flash monitor` para compilar, flashear y monitorear el proyecto.

(Para salir del monitor serial, presiona ``Ctrl-]``.)

### Salida de Ejemplo

```
DHT11 Sensor Example
=====================
Reading from GPIO4

Minimum free heap size: 290280 bytes

--- Reading 1 ---
✓ Temperature: 23.0°C
✓ Humidity: 65.0%

--- Reading 2 ---
✓ Temperature: 23.1°C
✓ Humidity: 64.0%

--- Reading 3 ---
✗ Failed to read DHT11 sensor
```

## Solución de Problemas

1. **Sin señal de respuesta**: Verificar el cableado y la fuente de alimentación
2. **Error de checksum**: Interferencia o problemas de temporización - intenta alejarte de fuentes de ruido
3. **Errores de timeout**: Resistencia pull-up faltante o valor incorrecto
4. **Lecturas inconsistentes**: Normal para el DHT11 - implementa promediado para mejor estabilidad

## Notas

- El DHT11 requiere al menos 1 segundo entre lecturas
- Esta implementación usa espera activa para temporización precisa
- Para uso en producción, considera usar una librería DHT dedicada con mejor manejo de errores
- El sensor puede dar lecturas fallidas ocasionales - esto es normal para el DHT11

Consulta la [Guía de Inicio](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) para pasos completos sobre cómo configurar y usar ESP-IDF para compilar proyectos.

## Salida de Ejemplo

Al ejecutar el ejemplo, verás el siguiente log:

```
I (317) gpio: GPIO[18]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (327) gpio: GPIO[19]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (337) gpio: GPIO[4]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:1
I (347) gpio: GPIO[5]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:1
Minimum free heap size: 289892 bytes
cnt: 0
cnt: 1
GPIO[4] intr, val: 1
GPIO[5] intr, val: 1
cnt: 2
GPIO[4] intr, val: 0
cnt: 3
GPIO[4] intr, val: 1
GPIO[5] intr, val: 1
cnt: 4
GPIO[4] intr, val: 0
cnt: 5
GPIO[4] intr, val: 1
GPIO[5] intr, val: 1
cnt: 6
GPIO[4] intr, val: 0
cnt: 7
GPIO[4] intr, val: 1
GPIO[5] intr, val: 1
cnt: 8
GPIO[4] intr, val: 0
cnt: 9
GPIO[4] intr, val: 1
GPIO[5] intr, val: 1
cnt: 10
...
```

## Solución de Problemas

Para cualquier consulta técnica, por favor abre un [issue](https://github.com/espressif/esp-idf/issues) en GitHub. Te responderemos pronto.
