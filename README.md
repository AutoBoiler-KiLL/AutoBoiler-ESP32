# SmartBoiler ESP32

## Features

- Precise temperature control with PID
- Local network control interface
- Global network connectivity (not implemented)
- Physical display interface
- Physical control buttons
- Factory reset capability
- Persistent memory storage

## Project Structure

```
SmartBoiler-ESP32/
├── lib/                    # Library modules
│   ├── Boiler/             # Boiler control logic
│   ├── Display/            # Display interface
│   ├── GlobalNetwork/      # WiFi and remote connectivity
│   ├── KiLL/               # Main control system
│   ├── LocalNetwork/       # Local network interface
│   ├── Memory/             # Persistent storage
│   ├── PID/                # Temperature control
│   ├── TemperatureSensor/  # Temperature sensing
│   └── Utils/              # Utility functions
├── src/                    # Source files (Main)
└── platformio.ini         # PlatformIO configuration
```

## Setup and Installation

1. Install PlatformIO IDE
2. Clone this repository
3. Open the project in PlatformIO
4. Configure your network settings
5. Upload to your ESP32 device

## Usage

### Physical Controls

- **Temperature Control**:
  - Use increase/decrease buttons to adjust target temperature
  - Temperature range: System minimum to maximum temperature

- **Boiler Control**:
  - Toggle button to turn the boiler on/off

- **Factory Reset**:
  - Hold the factory reset button for the configured duration to reset to default settings

## Development

This project is built using PlatformIO and Arduino framework for ESP32. The main control system (KiLL) orchestrates all components including:

- Temperature sensing and control
- Display updates
- Network connectivity
- User input handling
- System state management
