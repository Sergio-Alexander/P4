/*
Name: Sergio Satyabrata
Date: May 19, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

Lumen.h is the header file for the Lumen class, which represents a light source 
with customizable properties such as brightness, size, and power. 
The class provides methods to manipulate these properties and simulate the behavior of 
the light source in different conditions. The Lumen class is responsible for maintaining 
the state of the light source and providing methods to query its current state and modify it 
accordingly. This header file contains the declarations of the Lumen class's member 
functions and variables, ensuring proper encapsulation and separation of concerns in the program.
*/

#ifndef LUMEN_H
#define LUMEN_H

#include <stdexcept>

class Lumen {
public:
    Lumen(int inputBrightness, int inputSize, int inputPower);

    int glow(); 
    bool reset();
    void recharge();

    bool isActive(); 
    bool isErratic();

    int currentGlowValue(); // must be public because Nova needs to access it
    
    // Copy constructor
    // Lumen(const Lumen& other);

    // Copy assignment operator
    Lumen& operator=(const Lumen& other);

    // Comparison operators
    bool operator==(const Lumen& other) const;
    bool operator!=(const Lumen& other) const;
    bool operator<(const Lumen& other) const;
    bool operator>(const Lumen& other) const;
    bool operator<=(const Lumen& other) const;
    bool operator>=(const Lumen& other) const;

    // Arithmetic operators

    Lumen operator+(const Lumen& other) const; // standard
    Lumen operator+(int value) const; // mixed mode 

    Lumen& operator+=(const Lumen& other); // shortcut standard
    Lumen& operator+=(int value); // shortcut mixed mode

    Lumen& operator++(); // prefix increment
    Lumen operator++(int); // postfix increment


    Lumen operator-(const Lumen& other) const; // standard
    Lumen operator-(int value) const; // mixed mode 

    Lumen& operator-=(const Lumen& other); // shortcut standard
    Lumen& operator-=(int value); // shortcut mixed mode

    Lumen& operator--(); // prefix increment
    Lumen operator--(int); // postfix increment

    int getBrightness(); // For Nova to access these elements
    int getPower(); // For Nova to access these elements
    int getSize(); // For Nova to access these elements
    
private:
    int brightness;
    int size;
    int power;
    int brightnessCopy;
    int powerCopy;

    int dimmingValue;
    int powerThreshold;

    int glowRequest;
    int maxReset;
    int resetCount;
    bool charged;
    static constexpr int INACTIVESTATE = 0;
    static constexpr int RESETTHRESHOLD = 5;


    void resetOriginal();
    int erraticValue();
    bool resetRequest();
};

#endif // LUMEN_H


/*
Class invariants for Lumen:

- brightness, size, and power must always be non-negative integer values.
- brightnessCopy and powerCopy must hold the original brightness and power values, respectively.
- glowRequest must be a non-negative integer value that counts the number of glow() calls.
- charged is a boolean that represents whether the Lumen has been recharged or not.
- INACTIVESTATE and RESETTHRESHOLD are constants with fixed values.
- dimmingValue must always be calculated as 10% of the original brightness.
- powerThreshold must always be calculated as 20% of the original power.
*/