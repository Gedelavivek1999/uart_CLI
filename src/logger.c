#include "logger.h"
#include <stdio.h>
#include <string.h>

// Define handles
SemaphoreHandle_t uartMutex = NULL;
QueueHandle_t rxQueue = NULL;

// Initialize FreeRTOS primitives
void logger_init_primitives(void) {
    uartMutex = xSemaphoreCreateMutex();
    rxQueue = xQueueCreate(64, sizeof(char));
}

// Thread-safe wrapper for UART printing
void safe_uart_print(const char *message, TickType_t timeout) {
    if (xSemaphoreTake(uartMutex, timeout) == pdTRUE) {
        // Replace with your target's actual block write function if needed
        // e.g., UART_WriteBlocking(DEMO_UART, (uint8_t *)message, strlen(message));
        
        xSemaphoreGive(uartMutex);
    }
}

// Background sensor logger task
void vSensorLoggerTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000); // 1 second interval
    char logBuffer[64];
    uint32_t counter = 0;

    for (;;) {
        snprintf(logBuffer, sizeof(logBuffer), "[LOG] Uptime Counter: %lu\r\n", counter++);
        safe_uart_print(logBuffer, pdMS_TO_TICKS(100));

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

// Command-line interface task
void vCliTask(void *pvParameters) {
    char rxChar;
    char cmdBuffer[32];
    uint8_t index = 0;

    safe_uart_print("\r\n--- FreeRTOS CLI Initialized ---\r\n> ", portMAX_DELAY);

    for (;;) {
        if (xQueueReceive(rxQueue, &rxChar, portMAX_DELAY) == pdTRUE) {
            // Echo character back
            // UART_WriteBlocking(DEMO_UART, (uint8_t *)&rxChar, 1);

            if (rxChar == '\r' || rxChar == '\n') {
                cmdBuffer[index] = '\0'; // Null-terminate

                if (strcmp(cmdBuffer, "status") == 0) {
                    safe_uart_print("\r\nSystem Status: HEALTHY\r\n> ", pdMS_TO_TICKS(100));
                } else if (strcmp(cmdBuffer, "help") == 0) {
                    safe_uart_print("\r\nAvailable Commands: status, help\r\n> ", pdMS_TO_TICKS(100));
                } else if (index > 0) {
                    safe_uart_print("\r\nUnknown command. Type 'help'.\r\n> ", pdMS_TO_TICKS(100));
                } else {
                    safe_uart_print("\r\n> ", pdMS_TO_TICKS(100));
                }
                index = 0;
            } else if (index < sizeof(cmdBuffer) - 1) {
                cmdBuffer[index++] = rxChar;
            }
        }
    }
}