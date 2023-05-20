/*
Name: Sergio Satyabrata
Date: May 19, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

The Lumen class models a light-emitting object with adjustable brightness, size, and power. 
It provides various member functions to manipulate and query the state of the object, 
such as glowing, resetting, recharging, and checking whether the object is active, erratic, 
or requires a reset. The class also maintains internal states like brightness, size, power, 
glowRequest, and other related attributes to ensure the proper functioning of the Lumen object.

ASSUMPTIONS:
- Constructor is implemented correctly.
- Random function randomizes the values evenly.
- glow() and reset() functions work as they should
- the construction of array objects are created without failure
- Input Brightness, input power and input size are positive.
- Operators will be used in the correct format, an example would be:
When adding a value to a lumen, the lumen must be at the left hand side. The same thing goes with subtraction.
e.g. lumen1 + 1;
e.g. lumen1 - 1;

When adding a lumen to another lumen, specific variables will be added. The same thing will go with subtraction.
*/

// New Feature added:
// - a lumen may be reset at most some number of times (dependent on size)

#include "lumen.h"

// Preconditions: inputBrightness, inputSize, and inputPower are positive
// Postconditions: Lumen object is created with given input values
Lumen::Lumen(int inputBrightness, int inputSize, int inputPower) {
    if (inputBrightness <= 0 || inputSize <= 0 || inputPower <= 0) {
        throw std::out_of_range("All input values for Lumen must be positive.");
    }
    brightness = inputBrightness;
    size = inputSize;
    power = inputPower;
    brightnessCopy = inputBrightness;
    powerCopy = inputPower;
    glowRequest = 0;
    dimmingValue = static_cast<int>(brightnessCopy * (10.0 / 100));
    powerThreshold = static_cast<int>(powerCopy * (20.0 / 100));
    maxReset = size * 3;
    resetCount = 0;

    charged = false;
}

// Preconditions: None
// Postconditions: Returns the glow value based on the state of the Lumen object
int Lumen::glow() {
    glowRequest++;
    power--;
    if (!isActive()) {
        if (isErratic()) {
            return erraticValue();
        }
        return dimmingValue;
    }
    return brightness * size;
}

// Preconditions: None
// Postconditions: Resets the Lumen object if conditions are met, otherwise reduces brightness by 1
bool Lumen::reset() {
    if (resetCount >= maxReset){
        return false;
    }

    if (resetRequest()) {
        resetOriginal();
        return true;
    }

    brightness--;
    return false;
}

// Preconditions: None
// Postconditions: Helper function that helps the reset() function reset its values to the original input values
void Lumen::resetOriginal(){
    power = powerCopy;
    brightness = brightnessCopy;
    glowRequest = 0;
    resetCount++;
}

// Preconditions: None
// Postconditions: Returns true if the reset request is valid, otherwise false
bool Lumen::resetRequest() {
    return glowRequest >= RESETTHRESHOLD && power > INACTIVESTATE;
}

// Pre-condition: None
// Post-condition: power restored to its original value and charged set to true
void Lumen::recharge(){
    power = powerCopy;
    charged = true;
}

// Preconditions: None
// Postconditions: Returns true if the Lumen object is active, otherwise false
bool Lumen::isActive() {
    return power > powerThreshold;
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
    return power <= powerThreshold && power > INACTIVESTATE;
}

// Pre-condition: None
// Post-condition: return brightness * size if lumen is active, otherwise return dimmingValue or ErraticValue
int Lumen::currentGlowValue() {
    if (!isActive()) {
        if (isErratic()) {
            return erraticValue();
        }
        return dimmingValue;
    }
    return brightness * size;
}

/************************************************** Overloading Assignments********************************************************************/

// Copy assignment operator

// Precondition: none (it's safe to assign from any other Lumen instance)
// Postcondition: this instance is a copy of the other instance
Lumen& Lumen::operator=(const Lumen& other) {
    // Guard self assignment
    if (this == &other) {
        return *this;
    }

    // Copy each data member from the other object
    brightness = other.brightness;
    power = other.power;
    size = other.size;

    brightnessCopy = other.brightnessCopy;
    powerCopy = other.powerCopy;

    glowRequest = other.glowRequest;
    dimmingValue = other.dimmingValue;
    powerThreshold = other.powerThreshold;
    maxReset = other.maxReset;
    resetCount = other.resetCount;

    // Return a reference to this instance
    return *this;
}

// Comparison operators

// Precondition: none
// Postcondition: returns true if this and other represent the same Lumen (i.e., same brightness, power, and size)
bool Lumen::operator==(const Lumen& other) const {
    return brightness == other.brightness && power == other.power && size == other.size;
}

// Precondition: none
// Postcondition: returns true if this and other does not represent the same Lumen (i.e., same brightness, power, and size)
bool Lumen::operator!=(const Lumen& other) const {
    return !(*this == other);
}

// Precondition: none
// Postcondition: returns true if this is smaller than other (i.e., same brightness, power, and size)
bool Lumen::operator<(const Lumen& other) const {
    return brightness < other.brightness && power < other.power && size < other.size;
}
// Precondition: none
// Postcondition: returns true if this is greater than other (i.e., same brightness, power, and size)
bool Lumen::operator>(const Lumen& other) const {
    return brightness > other.brightness && power > other.power && size > other.size;
}
// Precondition: none
// Postcondition: returns true if this is smaller than or equal to other (i.e., same brightness, power, and size)
bool Lumen::operator<=(const Lumen& other) const {
    return !(*this > other);
}
// Precondition: none
// Postcondition: returns true if this is greater than or equal to other (i.e., same brightness, power, and size)
bool Lumen::operator>=(const Lumen& other) const {
    return !(*this < other);
}

// Arithmetic operators

// Precondition: none
// Postcondition: returns a new Lumen instance that is the sum of this and other
Lumen Lumen::operator+(const Lumen& other) const{
    Lumen result = *this;

    result.brightness = this->brightness + other.brightness;
    result.power = this->power + other.power;
    result.size = this->size + other.size;

    result.brightnessCopy = this->brightnessCopy + other.brightnessCopy;
    result.powerCopy = this->powerCopy + other.powerCopy;

    result.glowRequest = this->glowRequest + other.glowRequest;
    result.dimmingValue = this->dimmingValue + other.dimmingValue;
    result.powerThreshold = this->powerThreshold + other.powerThreshold;
    result.maxReset = this->maxReset + other.maxReset;
    result.resetCount = this->resetCount + other.resetCount;

    if (!this->charged || !other.charged){
        result.charged = false;
    } else{
        result.charged = true;
    }

    return result;
}

// Precondition: none
// Postcondition: returns a new Lumen instance where brightness, power, size, etc... are increased by value
Lumen Lumen::operator+(int value) const{ // mixed mode
    Lumen result = *this;
    result.brightness = this->brightness + value;
    result.power = this->power + value;
    result.size = this->size + value;

    result.brightnessCopy = this->brightnessCopy + value;
    result.powerCopy = this->powerCopy + value;

    result.glowRequest = this->glowRequest + value;
    result.dimmingValue = this->dimmingValue + value;
    result.powerThreshold = this->powerThreshold + value;
    result.maxReset = this->maxReset + value;
    result.resetCount = this->resetCount + value;

    return result;
}

// Precondition: none
// Postcondition: this instance has been modified by adding the brightness, power, size, etc... of other
Lumen& Lumen::operator+=(const Lumen& other) { // standard
    this->brightness += other.brightness;
    this->power += other.power;
    this->size += other.size;

    this->brightnessCopy += other.brightnessCopy;
    this->powerCopy += other.powerCopy;

    this->glowRequest += other.glowRequest;
    this->dimmingValue += other.dimmingValue;
    this->powerThreshold += other.powerThreshold;
    this->maxReset += other.maxReset;
    this->resetCount += other.resetCount;

    return *this;
}

// Precondition: none
// Postcondition: this instance has been modified by adding value to brightness, power, size, etc...
Lumen& Lumen::operator+=(int value){ // mixed mode
    brightness += value;
    power += value;
    size += value;

    brightnessCopy += value;
    powerCopy += value;

    glowRequest += value;
    dimmingValue += value;
    powerThreshold += value;
    maxReset += value;
    resetCount += value;

    return *this;
}

// Precondition: none
// Postcondition: the brightness, power, size, etc... of this instance have been incremented by 1
Lumen& Lumen::operator++() { // prefix increment
    ++brightness;
    ++power;
    ++size;

    ++brightnessCopy;
    ++powerCopy;

    ++glowRequest;
    ++dimmingValue;
    ++powerThreshold;
    ++maxReset;
    ++resetCount;

    return *this;
}


// Precondition: none
// Postcondition: returns a copy of this instance before incrementing brightness, power, size, etc... by 1
Lumen Lumen::operator++(int) { // postfix increment
    Lumen temp = *this;
    ++*this;
    return temp;
}

////////////////////////////////////////////////////////////////


// Precondition: none
// Postcondition: returns a new Lumen instance that is the difference of this and other
Lumen Lumen::operator-(const Lumen& other) const {
    Lumen result = *this;

    result.brightness = this->brightness - other.brightness;
    result.power = this->power - other.power;
    result.size = this->size - other.size;

    result.brightnessCopy = this->brightnessCopy - other.brightnessCopy;
    result.powerCopy = this->powerCopy - other.powerCopy;

    result.glowRequest = this->glowRequest - other.glowRequest;
    result.dimmingValue = this->dimmingValue - other.dimmingValue;
    result.powerThreshold = this->powerThreshold - other.powerThreshold;
    result.maxReset = this->maxReset - other.maxReset;
    result.resetCount = this->resetCount - other.resetCount;

    if (!this->charged || !other.charged){
        result.charged = false;
    } else{
        result.charged = true;
    }

    return result;
}

// Mixed-mode subtraction

// Precondition: none
// Postcondition: returns a new Lumen instance where brightness, power, size, etc... are decreased by value
Lumen Lumen::operator-(int value) const {
    Lumen result = *this;
    result.brightness = this->brightness - value;
    result.power = this->power - value;
    result.size = this->size - value;

    result.brightnessCopy = this->brightnessCopy - value;
    result.powerCopy = this->powerCopy - value;

    result.glowRequest = this->glowRequest - value;
    result.dimmingValue = this->dimmingValue - value;
    result.powerThreshold = this->powerThreshold - value;
    result.maxReset = this->maxReset - value;
    result.resetCount = this->resetCount - value;

    return result;
}

// Shortcut assignment
// Precondition: none
// Postcondition: this instance has been modified by subtracting the brightness, power, size, etc... of other
Lumen& Lumen::operator-=(const Lumen& other) { // standard
    this->brightness -= other.brightness;
    this->power -= other.power;
    this->size -= other.size;

    this->brightnessCopy -= other.brightnessCopy;
    this->powerCopy -= other.powerCopy;

    this->glowRequest -= other.glowRequest;
    this->dimmingValue -= other.dimmingValue;
    this->powerThreshold -= other.powerThreshold;
    this->maxReset -= other.maxReset;
    this->resetCount -= other.resetCount;

    return *this;
}


// Precondition: none
// Postcondition: this instance has been modified by subtracting value to brightness, power, size, etc...
Lumen& Lumen::operator-=(int value) { // mixed mode
    brightness -= value;
    power -= value;
    size -= value;

    brightnessCopy -= value;
    powerCopy -= value;

    glowRequest -= value;
    dimmingValue -= value;
    powerThreshold -= value;
    maxReset -= value;
    resetCount -= value;

    return *this;
}

// Decrement operators
// Precondition: none
// Postcondition: the brightness, power, size, etc... of this instance have been incremented by 1
Lumen& Lumen::operator--() { // prefix decrement
    --brightness;
    --power;
    --size;

    --brightnessCopy;
    --powerCopy;

    --glowRequest;
    --dimmingValue;
    --powerThreshold;
    --maxReset;
    --resetCount;

    return *this;
}

// Precondition: none
// Postcondition: returns a copy of this instance before incrementing brightness, power, size, etc... by 1
Lumen Lumen::operator--(int) { // postfix decrement
    Lumen temp = *this;
    --*this;
    return temp;
}

// Precondition: none
// Postcondition: returns the brightness of this Lumen instance
int Lumen::getBrightness(){
    return brightness;
} // For Nova to access these elements

// Precondition: none
// Postcondition: returns the power of this Lumen instance
int Lumen::getPower(){
    return power;
} // For Nova to access these elements

// Precondition: none
// Postcondition: returns the size of this Lumen instance
int Lumen::getSize(){
    return size;
} // For Nova to access these elements
    



/*
Implementation Invariant:
- isActive() checks if the Lumen object is active by comparing power to powerThreshold.
- isErratic() checks if the Lumen object is in an erratic state.
- ErraticValue() calculates the erratic value of the Lumen object.
- resetRequest() checks if a reset request is needed based on glowRequest and power.
- current_glow_value() returns the current glow value depending on the state of the Lumen object.
*/
