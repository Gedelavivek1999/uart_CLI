
**FRDM-MCXN23 FreeRTOS Deterministic Telemetry & UART CLI**

A modular architecture which implements a deterministic **50Hz flight controller telemetry telemetry loop** alongside an asynchronous **UART Command Line Interface (CLI)** using FreeRTOS synchronization primitives.

Key Features & Architecture

Deterministic 50Hz Control Loop: Precise execution timing using FreeRTOS periodic task delay (vTaskDelayUntil).
Non-Blocking Telemetry Engine: Thread-safe telemetry handling isolated from real-time task loops to prevent priority inversion.
Safe Concurrent Logging: Mutex-protected (uartmutex) and queue-backed (rxQueue) UART communication pipeline for concurrent multi-task safe output.
Hardware Watchdog Protection: Integrated 10-second structural watchdog timeout for fault recovery.
