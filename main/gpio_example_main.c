#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

/**
 * Brief:
 * This code shows how to use DHT11 sensor to read temperature and humidity.
 *
 * DHT11 Sensor conexiones en ESP32:
 * - Data pin connected to GPIO15 este pin puede cambiarse en el código
 * - VCC en salida de 3.3V
 * - GND a GND que es el pin de tierra
 *
 * valores que debe imprimir el DHT11:
 * - Humedad: 20-80% 
 * - Temperatura: 0-50°C 
 */

#define DHT11_GPIO    15  //  pin del DHT11 sensor en el esp32
#define DHT11_TIMEOUT 85 // tiempo de espera en microsegundos

// DHT11 sensor estructura de datos
// Estructura para almacenar los datos del sensor DHT11
typedef struct {
    float humedad;
    float temperatura;
    bool valido;
} dht11_data_t;

// Delay function for microseconds
static void delay_us(uint32_t us) {
    esp_timer_get_time(); // Initialize timer if not already done
    int64_t start = esp_timer_get_time();
    while ((esp_timer_get_time() - start) < us) {
    }
}

// lectura del sensor DHT11
static dht11_data_t read_dht11(void) {
    dht11_data_t result = {0};
    uint8_t data[5] = {0};
    uint8_t bit_count = 0;
    uint8_t byte_count = 0;

    portDISABLE_INTERRUPTS();
    
   // paso 1: Enviar señal de inicio
    gpio_set_direction(DHT11_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT11_GPIO, 0);  // apagar el pin
    delay_us(18000);  
    gpio_set_level(DHT11_GPIO, 1);  // enciende el pin
    delay_us(30);

    // paso 2: Cambiar a modo de entrada
    gpio_set_direction(DHT11_GPIO, GPIO_MODE_INPUT);

    // paso 3: Esperar la respuesta del DHT11
    // Esperar a que el DHT11 baje el nivel (señal de respuesta)
    uint32_t timeout = 0;
    while (gpio_get_level(DHT11_GPIO) == 1) {
        if (++timeout > DHT11_TIMEOUT) {
            portENABLE_INTERRUPTS();
            printf("DHT11: No response signal\n");
            return result;
        }
        delay_us(1);
    }
    
    // Esperar a que el DHT11 suba el nivel (inicio de datos)
    timeout = 0;
    while (gpio_get_level(DHT11_GPIO) == 0) {
        if (++timeout > DHT11_TIMEOUT) {
            portENABLE_INTERRUPTS();
            printf("DHT11: Response timeout\n");
            return result;
        }
        delay_us(1);
    }
    
    // Esperar a que el DHT11 baje el nivel (inicio de datos)
    timeout = 0;
    while (gpio_get_level(DHT11_GPIO) == 1) {
        if (++timeout > DHT11_TIMEOUT) {
            portENABLE_INTERRUPTS();
            printf("DHT11: inicio de datos timeout\n");
            return result;
        }
        delay_us(1);
    }
    
    // Paso 4: Leer los 40 bits de datos
    for (int i = 0; i < 40; i++) {
        // Esperar el inicio del bit (bajo a alto)
        timeout = 0;
        while (gpio_get_level(DHT11_GPIO) == 0) {
            if (++timeout > DHT11_TIMEOUT) {
                portENABLE_INTERRUPTS();
                printf("DHT11: Bit start timeout\n");
                return result;
            }
            delay_us(1);
        }
        
        // Medir la duración del pulso alto
        delay_us(30);  // Esperar 30us
        
        if (gpio_get_level(DHT11_GPIO) == 1) {
            // Pulso alto > 30us significa que el bit es 1
            data[byte_count] |= (1 << (7 - bit_count));
        }
        // else bit is 0 (already initialized to 0)

        // Esperar el final del bit
        timeout = 0;
        while (gpio_get_level(DHT11_GPIO) == 1) {
            if (++timeout > DHT11_TIMEOUT) {
                portENABLE_INTERRUPTS();
                printf("DHT11: final del bit timeout\n");
                return result;
            }
            delay_us(1);
        }
        
        bit_count++;
        if (bit_count == 8) {
            bit_count = 0;
            byte_count++;
        }
    }
    
    portENABLE_INTERRUPTS();

    // Paso 5: Verificar checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) {
        printf("DHT11: checksum error (calculado: %d, recibido: %d)\n", checksum, data[4]);
        return result;
    }

    // Paso 6: Analizar datos
    result.humedad = (float)data[0] + (float)data[1] * 0.1f;
    result.temperatura = (float)data[2] + (float)data[3] * 0.1f;
    result.valido = true;
    
    return result;
}

void app_main(void)
{
    printf("Uso del sensor DHT11\n");
    printf("=====================\n");
    printf("Leyendo desde el puerto GPIO%d\n\n", DHT11_GPIO);
    
    // configurar el pin del DHT11
    gpio_reset_pin(DHT11_GPIO);
    gpio_set_direction(DHT11_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DHT11_GPIO, GPIO_PULLUP_ONLY);
    
    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());
    
    int cnt = 0;
    while (1) {
        printf("\n--- Leyendo %d ---\n", ++cnt);
        
        dht11_data_t sensor_data = read_dht11();
        
        if (sensor_data.valido) {
            printf("✓ Temperatura: %.1f°C\n", sensor_data.temperatura);
            printf("✓ Humedad: %.1f%%\n", sensor_data.humedad);
        } else {
            printf("✗ Error al leer el sensor DHT11\n");
        }
        
        // Wait 2 seconds between readings (DHT11 needs at least 1 second)
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
