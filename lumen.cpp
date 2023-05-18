/*
Name: Sergio Satyabrata
Date: April 14, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

The Lumen class models a light-emitting object with adjustable brightness, size, and power. 
It provides various member functions to manipulate and query the state of the object, 
such as glowing, resetting, recharging, and checking whether the object is active, erratic, 
or requires a reset. The class also maintains internal states like brightness, size, power, 
glow_request, and other related attributes to ensure the proper functioning of the Lumen object.

ASSUMPTIONS:
- Constructor is implemented correctly.
- Random function randomizes the values evenly.
- glow() and reset() functions work as they should
- the construction of array objects are created without failure
- Input Brightness, input power and input size are positive.
*/

#include "lumen.h"

// Preconditions: input_brightness, input_size, and input_power are positive
// Postconditions: Lumen object is created with given input values
Lumen::Lumen(int input_brightness, int input_size, int input_power) {
    if (input_brightness <= 0 || input_size <= 0 || input_power <= 0) {
        throw std::out_of_range("All input values for Lumen must be positive.");
    }
    brightness = input_brightness;
    size = input_size;
    power = input_power;
    brightness_copy = input_brightness;
    power_copy = input_power;
    glow_request = 0;
    dimming_value = static_cast<int>(brightness_copy * (10.0 / 100));
    power_threshold = static_cast<int>(power_copy * (20.0 / 100));

    charged = false;
}

// Preconditions: None
// Postconditions: Returns the glow value based on the state of the Lumen object
int Lumen::glow() {
    glow_request++;
    power--;
    if (!isActive()) {
        if (isErratic()) {
            return ErraticValue();
        }
        return dimming_value;
    }
    return brightness * size;
}

// Preconditions: None
// Postconditions: Resets the Lumen object if conditions are met, otherwise reduces brightness by 1
bool Lumen::reset() {
    if (resetRequest()) {
        resetOriginal();
        return true;
    }
    brightness--;
    return false;
}

void Lumen::resetOriginal(){
    power = power_copy;
    brightness = brightness_copy;
    glow_request = 0;
}



// Pre-condition: None
// Post-condition: power restored to its original value and charged set to true
void Lumen::recharge(){
    power = power_copy;
    charged = true;
}

// Preconditions: None
// Postconditions: Returns true if the Lumen object is active, otherwise false
bool Lumen::isActive() {
    return power > power_threshold;
}

// Preconditions: The Lumen object is in an erratic state
// Postconditions: Returns a non-negative value less than the current power
int Lumen::ErraticValue() {
    int erraticFactor = 10;
    int erraticValue = brightness * size * (power + erraticFactor);
    return erraticValue;
}

// Preconditions: None
// Postconditions: Returns true if the Lumen object is erratic, otherwise false
bool Lumen::isErratic() {
    return power <= power_threshold && power > INACTIVE_STATE;
}

// Preconditions: None
// Postconditions: Returns true if the reset request is valid, otherwise false
bool Lumen::resetRequest() {
    return glow_request >= reset_threshold && power > INACTIVE_STATE;
}

// Pre-condition: None
// Post-condition: return brightness * size if lumen is active, otherwise return dimming_value or ErraticValue
int Lumen::currentGlowValue() {
    if (!isActive()) {
        if (isErratic()) {
            return ErraticValue();
        }
        return dimming_value;
    }
    return brightness * size;
}

/************************************************** Overloading Assignments********************************************************************/

Lumen::Lumen(const Lumen& other) {
    // Copy each data member from the other object
    brightness = other.brightness;
    power = other.power;
}

// Copy assignment operator
Lumen& Lumen::operator=(const Lumen& other) {
    // Guard self assignment
    if (this == &other) {
        return *this;
    }
    // Free up any resources that *this may have (not needed in this simple class)

    // Copy each data member from the other object
    brightness = other.brightness;
    power = other.power;

    // Return a reference to this instance
    return *this;
}

// Comparison operators
bool Lumen::operator==(const Lumen& other) const {
    return brightness == other.brightness && power == other.power;
}

bool Lumen::operator!=(const Lumen& other) const {
    return !(*this == other);
}

bool Lumen::operator<(const Lumen& other) const {
    return brightness < other.brightness;
}

bool Lumen::operator>(const Lumen& other) const {
    return brightness > other.brightness;
}

bool Lumen::operator<=(const Lumen& other) const {
    return !(*this > other);
}

bool Lumen::operator>=(const Lumen& other) const {
    return !(*this < other);
}

// Arithmetic operators
Lumen& Lumen::operator+=(const Lumen& other) {
    brightness += other.brightness;
    power += other.power;
    return *this;
}

Lumen Lumen::operator+(const Lumen& other) const {
    Lumen result = *this;
    result += other;
    return result;
}

Lumen& Lumen::operator++() { // prefix increment
    ++brightness;
    ++power;
    return *this;
}

Lumen Lumen::operator++(int) { // postfix increment
    Lumen temp = *this;
    ++*this;
    return temp;
}


/*
Implementation Invariant:
- isActive() checks if the Lumen object is active by comparing power to power_threshold.
- isErratic() checks if the Lumen object is in an erratic state.
- ErraticValue() calculates the erratic value of the Lumen object.
- resetRequest() checks if a reset request is needed based on glow_request and power.
- current_glow_value() returns the current glow value depending on the state of the Lumen object.
*/
