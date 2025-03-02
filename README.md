# ECE-230-Final-Project
# Advanced Steel Security System  

## Overview  
The **Advanced Steel Security System** is an embedded security project utilizing **TI MSP432P4111 microcontrollers** to create two independent alarm systems. These systems incorporate ultrasonic sensors, servo motors, speakers, LCD screens, and keypads to detect intrusions and provide visual and audible alerts.  

## Features  
- **Dual Alarm Systems**: One with a **keypad**, the other with an **LCD screen**.  
- **Ultrasonic Sensor Trigger**: Detects movement and activates the alarm.  
- **Servo Motor Control**: Automatically locks/unlocks based on the system state.  
- **Keypad Input for Disarm**: Enter the correct passcode (‘1’) to disable the alarm.  
- **LCD Alerts**: Displays warnings like *“INTRUDER ALERT!! / FEAR FOR YO LIFE”*.  
- **Audio Alerts**: Speaker plays distinct alarm sounds for armed/disarmed states.  

## Hardware Components  
- **Microcontroller**: TI MSP432P4111  
- **Sensors**: Ultrasonic sensor for motion detection  
- **Actuators**: Servo motor for door control  
- **User Interface**: Keypad & LCD display  
- **Audio**: Speaker for alarm sounds  
- **Miscellaneous**: Breadboard, wiring, enclosure (cardboard & 3D-printed parts)  

## System Operation  
### **Keypad Version**  
1. **Trigger**: Motion detected → Alarm activates, and the servo locks the door.  
2. **Disarm**: Enter passcode (‘1’) → Alarm stops, door unlocks.  

### **LCD Version**  
1. **Trigger**: Motion detected → Alarm activates, door locks, LCD displays warning.  
2. **Disarm**: Press the disable button → Alarm stops, door unlocks, LCD updates message.  

## Software Implementation  
- **Timers & Interrupts**: Ultrasonic sensor operates on **Timer A0**, outputs (LCD, speaker, servo) on **Timer A2**.  
- **Switch Case Logic**: Keypad input handled via an interrupt-driven switch case.  
- **State Management**: Button drivers & keypad dependencies manage **armed/disarmed** states.  
- **Speaker Control**: Different audio signals for alarm activation and deactivation.  

## Testing & Validation  
- **Servo Response**: Transitions between **-90° (0.75ms pulse width)** and **0° (1.6ms pulse width)** within a **25ms period**.  
- **Speaker Activation**: Alarm sounds within **330ms** of motion detection.  
- **Speaker Deactivation**: Stops within **500ms** after correct keypad input.  

## Challenges & Future Work  
- **UART Communication Issues**: Initially planned for communication between systems but faced software challenges.  
- **Enclosure Constraints**: Wiring took up more space than anticipated, leading to a messy presentation.  
- **Hardware Reliability**: Loose wiring and noisy servo motors could be improved with soldering and higher-quality components.  
- **Future Enhancements**: Implement **UART** or **Bluetooth** communication for a seamless, wireless security network.  

## Budget  
- **Elegoo Most Complete Starter Kit**: Used for components  
- **Cardboard Enclosure**: $6  
- **3D Printing Costs**: $7  
- **Total Cost**: $13 (assuming access to an Elegoo Starter Kit)  

## Acknowledgments  
Special thanks to **Dr. Song** for providing project templates and guidance.  
