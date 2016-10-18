#include <FlexCAN.h> // import teensy library

//Description of Throttle Control Unit
//Senses the angle of each pedal to determine safely how much torque the motor controller should produce with the motor
//Reads a signal from the Brake System Plausibility Device located on the same board to sense if the BSPD has detected a fault.

FlexCAN CAN(500000);
static CAN_message_t msg;
int voltageBrakePedal = 0;//voltage of brakepedal
int voltageThrottlePedal1 = 0;//voltage of 1st throttle
int voltageThrottlePedal2 = 0;//voltage of 2nd throttle
unsigned long timer; // use timer = millis() to get time, and compare in ms

//FSAE requires that torque be shut off if an implausibility persists for over 100 msec (EV2.3.5).
//A deviation of more than 10% pedal travel between the two throttle sensors
//A failure of position sensor wiring which can cause an open circuit, short to ground, or short to sensor power.
bool torqueShutdown = false; //

//To detect an open circuit
//enable the pullup resistor on the Teensy input pin >>>
//open circuit will show a high signal outside of the working range of the sensor. 


//To detect a position sensor wiring failure
//find the ranges of values coming from each sensor during normal operation of the foot pedals
//Any values outside of these ranges could be caused by an open circuit, short to ground, or short to sensor power.

//Error Message Instructions
//an error message should be sent out on CAN Bus detailing which implausibility has been detected. 
//periodically sent until the implausibility ceases to exist. 
//If the implausibility ceases, a corresponding message should be sent on CAN Bus. 
//If an implausibility ceases to be detected, normal throttle controls should be reinstated
//i.e. the vehicle does not need to be restarted to reset an implausibility fault.



// timer


// setup code

void setup() {
    Serial.begin(115200); // init serial for PC communication

    CAN.begin(); // init CAN system
    Serial.println("CAN system and serial communication initialized");
}

// loop code
void loop() {
}

}

