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
    max_reset = size * 3;
    reset_count = 0;

    charged = false;
}

// Preconditions: None
// Postconditions: Returns the glow value based on the state of the Lumen object
int Lumen::glow() {
    glow_request++;
    power--;
    if (!isActive()) {
        if (isErratic()) {
            return erraticValue();
        }
        return dimming_value;
    }
    return brightness * size;
}

// Preconditions: None
// Postconditions: Resets the Lumen object if conditions are met, otherwise reduces brightness by 1
bool Lumen::reset() {
    if (reset_count >= max_reset){
        return false;
    }

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
    reset_count++;
}

// Preconditions: None
// Postconditions: Returns true if the reset request is valid, otherwise false
bool Lumen::resetRequest() {
    return glow_request >= reset_threshold && power > INACTIVE_STATE;
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
int Lumen::erraticValue() {
    int erraticFactor = 10;
    int erraticValue = brightness * size * (power + erraticFactor);
    return erraticValue;
}

// Preconditions: None
// Postconditions: Returns true if the Lumen object is erratic, otherwise false
bool Lumen::isErratic() {
    return power <= power_threshold && power > INACTIVE_STATE;
}

// Pre-condition: None
// Post-condition: return brightness * size if lumen is active, otherwise return dimming_value or ErraticValue
int Lumen::currentGlowValue() {
    if (!isActive()) {
        if (isErratic()) {
            return erraticValue();
        }
        return dimming_value;
    }
    return brightness * size;
}

/************************************************** Overloading Assignments********************************************************************/

// Copy assignment operator
Lumen& Lumen::operator=(const Lumen& other) {
    // Guard self assignment
    if (this == &other) {
        return *this;
    }

    // Copy each data member from the other object
    brightness = other.brightness;
    power = other.power;
    size = other.size;

    brightness_copy = other.brightness_copy;
    power_copy = other.power_copy;

    glow_request = other.glow_request;
    dimming_value = other.dimming_value;
    power_threshold = other.power_threshold;
    max_reset = other.max_reset;
    reset_count = other.reset_count;

    // Return a reference to this instance
    return *this;
}

// Comparison operators
bool Lumen::operator==(const Lumen& other) const {
    return brightness == other.brightness && power == other.power && size == other.size;
}

bool Lumen::operator!=(const Lumen& other) const {
    return !(*this == other);
}

bool Lumen::operator<(const Lumen& other) const {
    return brightness < other.brightness && power < other.power && size < other.size;
}

bool Lumen::operator>(const Lumen& other) const {
    return brightness > other.brightness && power > other.power && size > other.size;
}

bool Lumen::operator<=(const Lumen& other) const {
    return !(*this > other);
}

bool Lumen::operator>=(const Lumen& other) const {
    return !(*this < other);
}

// Arithmetic operators

Lumen Lumen::operator+(const Lumen& other) const{
    Lumen result = *this;

    result.brightness = this->brightness + other.brightness;
    result.power = this->power + other.power;
    result.size = this->size + other.size;

    result.brightness_copy = this->brightness_copy + other.brightness_copy;
    result.power_copy = this->power_copy + other.power_copy;

    result.glow_request = this->glow_request + other.glow_request;
    result.dimming_value = this->dimming_value + other.dimming_value;
    result.power_threshold = this->power_threshold + other.power_threshold;
    result.max_reset = this->max_reset + other.max_reset;
    result.reset_count = this->reset_count + other.reset_count;

    if (!this->charged || !other.charged){
        result.charged = false;
    } else{
        result.charged = true;
    }

    return result;
}

Lumen Lumen::operator+(int value) const{ // mixed mode
    Lumen result = *this;
    result.brightness = this->brightness + value;
    result.power = this->power + value;
    result.size = this->size + value;

    result.brightness_copy = this->brightness_copy + value;
    result.power_copy = this->power_copy + value;

    result.glow_request = this->glow_request + value;
    result.dimming_value = this->dimming_value + value;
    result.power_threshold = this->power_threshold + value;
    result.max_reset = this->max_reset + value;
    result.reset_count = this->reset_count + value;

    return result;
}

Lumen& Lumen::operator+=(const Lumen& other) { // standard
    this->brightness += other.brightness;
    this->power += other.power;
    this->size += other.size;

    this->brightness_copy += other.brightness_copy;
    this->power_copy += other.power_copy;

    this->glow_request += other.glow_request;
    this->dimming_value += other.dimming_value;
    this->power_threshold += other.power_threshold;
    this->max_reset += other.max_reset;
    this->reset_count += other.reset_count;

    return *this;
}

Lumen& Lumen::operator+=(int value){ // mixed mode
    brightness += value;
    power += value;
    size += value;

    brightness_copy += value;
    power_copy += value;

    glow_request += value;
    dimming_value += value;
    power_threshold += value;
    max_reset += value;
    reset_count += value;

    return *this;
}

Lumen& Lumen::operator++() { // prefix increment
    ++brightness;
    ++power;
    ++size;

    ++brightness_copy;
    ++power_copy;

    ++glow_request;
    ++dimming_value;
    ++power_threshold;
    ++max_reset;
    ++reset_count;

    return *this;
}

Lumen Lumen::operator++(int) { // postfix increment
    Lumen temp = *this;
    ++*this;
    return temp;
}



////////////////////////////////////////////////////////////////

Lumen Lumen::operator-(const Lumen& other) const {
    Lumen result = *this;

    result.brightness = this->brightness - other.brightness;
    result.power = this->power - other.power;
    result.size = this->size - other.size;

    result.brightness_copy = this->brightness_copy - other.brightness_copy;
    result.power_copy = this->power_copy - other.power_copy;

    result.glow_request = this->glow_request - other.glow_request;
    result.dimming_value = this->dimming_value - other.dimming_value;
    result.power_threshold = this->power_threshold - other.power_threshold;
    result.max_reset = this->max_reset - other.max_reset;
    result.reset_count = this->reset_count - other.reset_count;

    if (!this->charged || !other.charged){
        result.charged = false;
    } else{
        result.charged = true;
    }

    return result;
}

// Mixed-mode subtraction
Lumen Lumen::operator-(int value) const {
    Lumen result = *this;
    result.brightness = this->brightness - value;
    result.power = this->power - value;
    result.size = this->size - value;

    result.brightness_copy = this->brightness_copy - value;
    result.power_copy = this->power_copy - value;

    result.glow_request = this->glow_request - value;
    result.dimming_value = this->dimming_value - value;
    result.power_threshold = this->power_threshold - value;
    result.max_reset = this->max_reset - value;
    result.reset_count = this->reset_count - value;

    return result;
}

// Shortcut assignment
Lumen& Lumen::operator-=(const Lumen& other) { // standard
    this->brightness -= other.brightness;
    this->power -= other.power;
    this->size -= other.size;

    this->brightness_copy -= other.brightness_copy;
    this->power_copy -= other.power_copy;

    this->glow_request -= other.glow_request;
    this->dimming_value -= other.dimming_value;
    this->power_threshold -= other.power_threshold;
    this->max_reset -= other.max_reset;
    this->reset_count -= other.reset_count;

    return *this;
}

Lumen& Lumen::operator-=(int value) { // mixed mode
    brightness -= value;
    power -= value;
    size -= value;

    brightness_copy -= value;
    power_copy -= value;

    glow_request -= value;
    dimming_value -= value;
    power_threshold -= value;
    max_reset -= value;
    reset_count -= value;

    return *this;
}

// Decrement operators
Lumen& Lumen::operator--() { // prefix decrement
    --brightness;
    --power;
    --size;

    --brightness_copy;
    --power_copy;

    --glow_request;
    --dimming_value;
    --power_threshold;
    --max_reset;
    --reset_count;

    return *this;
}

Lumen Lumen::operator--(int) { // postfix decrement
    Lumen temp = *this;
    --*this;
    return temp;
}

int Lumen::getBrightness(){
    return brightness;
} // For Nova to access these elements
int Lumen::getPower(){
    return power;
} // For Nova to access these elements
int Lumen::getSize(){
    return size;
} // For Nova to access these elements
    



/*
Implementation Invariant:
- isActive() checks if the Lumen object is active by comparing power to power_threshold.
- isErratic() checks if the Lumen object is in an erratic state.
- ErraticValue() calculates the erratic value of the Lumen object.
- resetRequest() checks if a reset request is needed based on glow_request and power.
- current_glow_value() returns the current glow value depending on the state of the Lumen object.
*/
