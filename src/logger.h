#ifndef LOGGER_H
#define LOGGER_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// Shared handles
extern SemaphoreHandle_t uartMutex;
extern QueueHandle_t rxQueue;

// Function prototypes
void logger_init_primitives(void);
void safe_uart_print(const char *message, TickType_t timeout);
void vSensorLoggerTask(void *pvParameters);
void vCliTask(void *pvParameters);

#endif /* LOGGER_H */