# 3 Omni (kiwi Drive) Wireless Remote Variant-

During our college robotics club build, we adapted the platform to control *Kiwi Drive system* (3-wheel Omnidirectional drive system spaced at 120°) wirelessly via a PS4 controller. 

**Implementation Highlight:** /n
**Bluepad32 Library Integration:** Utilized the open source **Bluepad** library stack on the ESP32 for reliable, low latency Bluetooth pairing. 
**Gamepad Mapping:** Captured analog stick axes (`ctl->axisX()` and `ctl->axisY()`) to continuously map joystick movement to motor control. 
