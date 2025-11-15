IRT Drone Telemetry & Control System (IRT-DTC)

![IRT(IoT) Drone Module](docs/images/IRT(IoT)Drone_Module.png)

A modular platform for collecting drone data, transmitting it over LoRa, forwarding online, and visualizing it.
This system combines embedded development, wireless communication, networking, and full-stack software.

## Project Overview

This project aims to build a complete hardware & software system to:

- Collect sensor data on a drone using an ESP32 and Sensors

- Transmit data wirelessly via LoRa to a fixed ground module

- Forward data from the fixed module to the internet via Wi-Fi

- Send the data to a VPS-based backend server

- process data in a backend application (Spring Boot)

- Store data in a DataBase PostgreSQL/MySQL

- Visualize data in real time using a frontend (Angular)

- Send commands back to the drone — potentially even predefined flight trajectory control (Future plan).

---

## Architecture Overview

![Architecture Diagram](docs/images/drone-mvp-schema02.png)

1. Drone Module (Hardware)

    - ESP32

    - Sensors / Captors (acceleration, altitude, telemetry, etc.)

    - nRF24L01 module for LoRa-like communication

    - Powered by the drone's supply

    - Sends sensor data → via SPI → nRF24L01 → Wireless → Fixed Module

2. Fixed Ground Module (Hardware)

    - ESP32

    - nRF24L01 wireless module (for now, waiting for Lora module to arrive)

    - Wi-Fi internet connection (via router / provider)

    - Forwards raw telemetry to VPS via HTTP/MQTT/Websockets

3. VPS / Server (Networking)

    - Receives telemetry from the fixed module

    - Communicates with Spring Boot backend

4. Backend (Software – Spring Boot)

    - Exposes REST API

    - Stores data in database

    - Provides endpoints for:

        - Submitting telemetry

        - Querying drone history

        - Sending commands back to modules

5. Frontend (Software – Angular)

    - Dashboard UI

    - Displays live telemetry

    - Graphs & charts

    - Interface for sending commands (future control system)

---

## Task Breakdown

> HARDWARE TASKS

1. Drone Module (ESP32)

    ESP32 firmware base setup

    - SPI communication with nRF24L01

    - SPI communication with LoRA

    - Implement LoRa packet structure

    - Creating an API to enable communication with LoRA/nRF24L01 interchangibaly

    - Integrate sensor drivers (IMU, GPS future, etc.)

    - Power optimization & sleep modes

    - Error handling, reconnection logic

2. Fixed Module (ESP32 + WiFi)

    - ESP32 firmware base setup

    - Wireless LoRa receiver logic

    - WiFi connection manager

    - Data forwarder: send packets to VPS

    - Local buffering when no internet

    - Future: Receive commands from backend → LoRa → drone

---

> NETWORKING TASKS

1. VPS

    - Create VPS environment

    - Reverse proxy (Nginx / Traefik)

    - HTTPS setup (Let's Encrypt)

    - Expose endpoint to receive module data

    - Security rules & firewall

---

> BACKEND TASKS (SPRING BOOT)

1. Spring boot

    - Initial project scaffold

    - Telemetry ingestion API (POST /telemetry)

    - Database design (PostgreSQL)

    - Drone history queries

    - Authentication (API keys or JWT)

    - Websocket for real-time updates

    - Command endpoint (future control system)

---

> FRONTEND TASKS (ANGULAR)

1. Angular

    - Angular project setup

    - Real-time telemetry dashboard

    - Graphs (charts)

    - Map view (future GPS)

    - Device management UI

    - Commands panel (future control)

---

> TESTING TASKS

1. Test

    - Unit tests for ESP32 modules

    - Backend unit tests

    - Integration tests (backend + frontend)

    - End-to-end test environment

    - Field tests with real hardware

---
## 📁 Proposed Repository Structure

```
/project-root
├── firmware/
│   ├── drone-module/
│   ├── fixed-module/
│   └── shared/
│
├── backend/
│   ├── src/
│   └── README.md
│
├── frontend/
│   └── src/
│
├── docs/
│   ├── architecture/
│   └── communication-protocol.md

└── README.md

```

---
## How to Contribute

Every team member must follow:

1. Fork the repository

    Each developer works on their own copy.

2. Create a new branch for the task

    `git checkout -b feature/task-name`

3. Implement the task

    Follow coding guidelines in the /docs folder.

4. Push and create a Pull Request

    PR must include:

    Description of what was done
    Screenshots/logs when applicable

5. Code review

    At least one approval before merging.

---

## Coding Standards

- Firmware (ESP32)

    - Use PlatformIO or ESP-IDF

    - No blocking waits where possible

    - Modular components for each sensor

    - LoRa packet structure documented in /docs/protocol.md

- Backend (Spring Boot)

    - REST naming conventions

    - DTOs & Services separated cleanly

    - JPA for data models

- Frontend (Angular)

    - Use services for API calls

    - Use RxJS for streams

