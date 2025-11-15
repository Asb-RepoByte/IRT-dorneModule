Drone Telemetry & Control System

A modular platform for collecting drone data, transmitting it over LoRa, forwarding online, and visualizing it.

📌 Project Overview

This project aims to build a complete hardware & software system to:

Collect sensor data on a drone using an ESP32

Transmit data wirelessly via LoRa (nRF24L01) to a fixed ground module

Forward data from the fixed module to the internet via Wi-Fi

Send the data to a VPS-based backend server

Store and process data in a backend application (Spring Boot)

Visualize data in real time using a frontend (Angular)

(Future) Send commands back to the drone — potentially even predefined flight trajectory control.

The system combines embedded development, wireless communication, networking, and full-stack software.

🗂️ Architecture Overview
1. Drone Module (Hardware)

ESP32

Sensors / Captors (acceleration, altitude, telemetry, etc.)

nRF24L01 module for LoRa-like communication

Powered by the drone's supply

Sends sensor data → via SPI → nRF24L01 → Wireless → Fixed Module

2. Fixed Ground Module (Hardware)

ESP32

nRF24L01 wireless module

Wi-Fi internet connection (via router / provider)

Forwards raw telemetry to VPS via HTTP/MQTT/Websockets

3. VPS / Server (Networking)

Receives telemetry from the fixed module

Communicates with Spring Boot backend

4. Backend (Software – Spring Boot)

Exposes REST API

Stores data in database

Provides endpoints for:

Submitting telemetry

Querying drone history

Sending commands back to modules

5. Frontend (Software – Angular)

Dashboard UI

Displays live telemetry

Graphs & charts

Interface for sending commands (future control system)

✅ Task Breakdown

You can post these as GitHub issues — each team member picks a task.

🔧 HARDWARE TASKS
1. Drone Module (ESP32)
