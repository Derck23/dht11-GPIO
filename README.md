| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | -------- | -------- | -------- |

# Example: DHT11 Temperature and Humidity Sensor

This example demonstrates how to read temperature and humidity data from a DHT11 sensor using ESP32.

## DHT11 Sensor Overview

The DHT11 is a basic, low-cost digital temperature and humidity sensor. It uses a single-wire communication protocol.

**Specifications:**
- Temperature: 0-50°C (±2°C accuracy)
- Humidity: 20-80% RH (±5% RH accuracy)
- Operating voltage: 3.3V-5V
- Operating current: 0.3mA (measuring) 60µA (standby)

## Hardware Connections

| DHT11 Pin | ESP32 Pin | Description |
| --------- | --------- | ----------- |
| VCC       | 3.3V      | Power supply |
| GND       | GND       | Ground |
| Data      | GPIO4     | Data signal (configurable in code) |

**Important:** Connect a 4.7kΩ-10kΩ pull-up resistor between the data pin and VCC.

## How it Works

1. **Start Signal**: MCU sends start signal by pulling data line low for 18ms, then high for 30µs
2. **Response Signal**: DHT11 responds by pulling data line low for 80µs, then high for 80µs
3. **Data Transmission**: DHT11 sends 40 bits of data (humidity + temperature + checksum)
4. **Data Format**: 
   - Byte 0: Humidity integer part
   - Byte 1: Humidity decimal part  
   - Byte 2: Temperature integer part
   - Byte 3: Temperature decimal part
   - Byte 4: Checksum (sum of bytes 0-3)

## Configuration

The DHT11 data pin is configured to use GPIO4 by default. You can change this by modifying the `DHT11_GPIO` define in the code:

```c
#define DHT11_GPIO    4  // Change this to your desired GPIO pin
```

## How to use example

Before project configuration and build, be sure to set the correct chip target using `idf.py set-target <chip_name>`.

### Hardware Required

* A development board with any Espressif SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* A USB cable for Power supply and programming
* DHT11 sensor module
* 4.7kΩ-10kΩ pull-up resistor
* Jumper wires for connections

### Wiring Diagram

```
ESP32          DHT11
-----          -----
3.3V    ---    VCC
GND     ---    GND
GPIO4   ---    Data (with 4.7kΩ pull-up to VCC)
```

### Build and Flash

Build the project and flash it to the board, then run the monitor tool to view the serial output:

Run `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

### Example Output

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

## Troubleshooting

1. **No response signal**: Check wiring and power supply
2. **Checksum error**: Interference or timing issues - try moving away from noise sources
3. **Timeout errors**: Pull-up resistor missing or wrong value
4. **Inconsistent readings**: Normal for DHT11 - implement averaging for better stability

## Notes

- DHT11 requires at least 1 second between readings
- This implementation uses busy-waiting for precise timing
- For production use, consider using a dedicated DHT library with better error handling
- The sensor may give occasional failed readings - this is normal for DHT11

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

As you run the example, you will see the following log:

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

## Troubleshooting

For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you soon.
