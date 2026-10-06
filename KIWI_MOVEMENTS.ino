#include <Bluepad32.h>

// Motor pins definitions
#define MOTOR1_PWM_PIN 12
#define MOTOR2_PWM_PIN 27
#define MOTOR3_PWM_PIN 25

#define MOTOR1_DIR_PIN 13
#define MOTOR2_DIR_PIN 14
#define MOTOR3_DIR_PIN 26

// Speed constants
const int MOTOR_SPEED = 20; // Adjust PWM speed value to a safe range
const int NEUTRAL_THRESHOLD = 10; // Threshold for motor PWM deadzone

// Store connected controllers
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Callback - when a controller connects
void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.print("Controller connected at index: ");
            Serial.println(i);
            myControllers[i] = ctl;
            break;
        }
    }
}

// Callback - when a controller disconnects
void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.print("Controller disconnected at index: ");
            Serial.println(i);
            myControllers[i] = nullptr;
            break;
        }
    }
}

// Stop all motors
void stopMotors() {
    analogWrite(MOTOR1_PWM_PIN, 0);
    analogWrite(MOTOR2_PWM_PIN, 0);
    analogWrite(MOTOR3_PWM_PIN, 0);
}

// Handle motor neutral deadzone
int applyNeutralThreshold(int pwmValue) {
    if (abs(pwmValue) < NEUTRAL_THRESHOLD) {
        return 0; // Stop motor if PWM is very close to neutral
    }
    return pwmValue;
}

// Forward movement function
void moveForward() {
    analogWrite(MOTOR1_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
    analogWrite(MOTOR2_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
    analogWrite(MOTOR3_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
}

// Backward movement function
void moveBackward() {
    analogWrite(MOTOR1_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
    analogWrite(MOTOR2_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
    analogWrite(MOTOR3_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
}

// Clockwise rotation
void rotateClockwise() {
    analogWrite(MOTOR1_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
    analogWrite(MOTOR2_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
    analogWrite(MOTOR3_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
}

// Counterclockwise rotation
void rotateAntiClockwise() {
    analogWrite(MOTOR1_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
    analogWrite(MOTOR2_PWM_PIN, applyNeutralThreshold(MOTOR_SPEED));
    analogWrite(MOTOR3_PWM_PIN, applyNeutralThreshold(-MOTOR_SPEED));
}

// Process controller inputs
void processControllerInputs(ControllerPtr ctl) {
    if (ctl) {
        // Map joystick movement to motor control
        int forwardAxis = ctl->axisY(); // Forward/Backward
        int rotationAxis = ctl->axisX(); // Left/Right rotation

        // Apply forward/backward logic
        if (forwardAxis > 100) {
            moveForward();
        } else if (forwardAxis < -100) {
            moveBackward();
        } else {
            stopMotors();
        }

        // Apply rotation logic
        if (rotationAxis > 100) {
            rotateClockwise();
        } else if (rotationAxis < -100) {
            rotateAntiClockwise();
        }
    }
}

void handleControllerUpdates() {
    for (auto ctl : myControllers) {
        if (ctl && ctl->isConnected()) {
            processControllerInputs(ctl);
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Set up motor pins as OUTPUT
    pinMode(MOTOR1_PWM_PIN, OUTPUT);
    pinMode(MOTOR2_PWM_PIN, OUTPUT);
    pinMode(MOTOR3_PWM_PIN, OUTPUT);

    pinMode(MOTOR1_DIR_PIN, OUTPUT);
    pinMode(MOTOR2_DIR_PIN, OUTPUT);
    pinMode(MOTOR3_DIR_PIN, OUTPUT);

    // Set callbacks for controller connection events
    BP32.setup(&onConnectedController, &onDisconnectedController);
}

void loop() {
    BP32.update(); // Update Bluepad32 connection status
    handleControllerUpdates();
    delay(50); // Avoid busy loops
}
