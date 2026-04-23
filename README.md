# Wireless-Controlled-Gas-Leakage-Detector
An IoT-based safety solution designed to monitor gas concentrations in real-time and provide wireless alerts. This system utilizes high-sensitivity sensors and a wireless communication module to detect leaks and trigger remote notifications, ensuring rapid response in industrial and domestic environments.
This project integrates embedded programming, sensor calibration, and wireless data transmission to create a robust, low-latency safety device.

🚀 Key Features
Real-time Monitoring: Continuous scanning of gas concentration levels.
Wireless Alerting: Instant notifications sent via Bluetooth when thresholds are exceeded.
Visual & Audio Alarms: On-site alerting through an integrated buzzer and LED indicators.
Automated Safety Response: (Optional) Can be configured to trigger exhaust fans or shut off solenoid valves.
Calibration Control: Software-based sensitivity adjustment to prevent false positives in different environments.

🛠 Tech Stack & Components
Hardware
Microcontroller: Arduino Uno
Sensor: MQ-2, MQ-3, MQ-4, MQ-5, DHT 111
Wireless Module: Bluetooth Module
Output Devices: 16x2 LCD Display, Active Buzzer, Servo Motor(sg90), Fan.

Software
Language: C++ (Arduino Framework)
Design Tools: Proteus (Simulation)
Development Environment: Arduino IDE

📊 System Architecture
The system follows a standard Sensing-Processing-Action loop:
Sensing: The MQ-series sensor detects gas molecules and converts them into an analog signal.
Processing: The microcontroller converts the analog signal to digital, comparing it against a pre-defined safety threshold.
Communication: If the threshold is breached, the wireless module sends a signal to the remote receiver/user.
Action: Local alarms are triggered, the servo motor opened the door, the fan blow out the gases, and status is updated on the LCD.

🔮 Future Enhancements
Edge AI Integration: Implementing predictive maintenance to detect sensor degradation and real time communication with the user using voice recognition.
Cloud Dashboard: Integration with ThingSpeak for historical data logging.
Mobile App: A dedicated React Native app for real-time monitoring and controls.
