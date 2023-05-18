/*
Name: Sergio Satyabrata
Date: April 14, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

The Nova class represents a light source that derives its properties from the Lumen class. 
The Nova class provides methods for changing the brightness, size, and power of the light source
while taking into account the Lumen class properties. 
The Nova class is used to simulate the behavior of a light source and its interactions in 
different conditions.

ASSUMPTIONS:
Assumption: The user will provide valid input values for the Nova class.
i.e., positive integers for brightness, size, and power, and a positive integer for the number of lumens.


NOTE: recharge() function was implemented as it will internally recharge if more than half of the lumens in
the Nova are inactive and it will recharge lumens that are stable within the Nova.
*/


#include "nova.h"
#include <stdexcept>
#include <iostream>


/************************************ Nova Constructors *******************************************/

// Pre-Condition: initial brightness, size, and power should not be negative; should be positive
// Post-Condition: first lumen is initialized along with the rest of the lumens for the nova
Nova::Nova(int initial_brightness, int initial_size, int initial_power, int num_lumens) {
    if (initial_brightness <= 0 || initial_size <= 0 || initial_power <= 0 || num_lumens <=0) {
        throw std::out_of_range("All input values for Nova must be positive.");
    }
    this->num_lumens = num_lumens;
    lumens = new Lumen*[num_lumens];

    for (int i = 0; i < num_lumens; i++) {
        int brightness = initial_brightness + i;
        int size = initial_size + i;
        int power = initial_power + i * 10;
        lumens[i] = new Lumen(brightness, size, power);
    }
}

// Pre-Condition: None
// Post-Condition: Frees Memory
Nova::~Nova() {
    freeMemory();
}

// Copy Nova
// Pre-Condition: other must be a valid Nova object
// Post-Condition: Creates a copy of the other Nova object
Nova::Nova(const Nova& other) {
    copyLumens(other);
}


// Copy assignment operator
// Pre-Condition: other must be a valid Nova object
// Post-Condition: The current Nova object is replaced by a copy of the other Nova object
Nova& Nova::operator=(const Nova& other) {
    if (this != &other) {
        freeMemory();
        copyLumens(other);
    }
    return *this;
}

// Move constructor
// Pre-Condition: other must be a valid Nova object
// Post-Condition: The current Nova object takes ownership of the other Nova object's resources, and other's resources are reset
Nova::Nova(Nova&& other) noexcept
: num_lumens(other.num_lumens), lumens(other.lumens) {
    other.num_lumens = 0;
    other.lumens = nullptr;
}

// Move assignment operator
// Pre-Condition: other must be a valid Nova object
// Post-Condition: The current Nova object takes ownership of the other Nova object's 
Nova& Nova::operator=(Nova&& other) noexcept {
    if (this != &other) {
        // Release the resources of the current object
        delete[] lumens;

        // Transfer ownership of resources from the source object
        num_lumens = other.num_lumens;
        lumens = other.lumens;

        // Reset the source object's resource pointers
        other.num_lumens = 0;
        other.lumens = nullptr;
    }

    return *this;
}





/********************************* Nova Core Functions **********************************************/


// Pre-Condition: x should be a non-negative integer and less than or equal to the number of lumens
// Post-Condition: The first x lumens are made to glow, and inactive lumens are recharged if necessary
void Nova::glow(int x) {
    if (x < 0 || x > num_lumens) {
        throw std::out_of_range("Invalid number of lumens to glow.");
    }
    for (int i = 0; i < x; i++) {
        if (lumens[i])
        lumens[i]->glow();
    }
    rechargeInactiveLumens();
}


// Pre-Condition: other must be a valid Nova object
// Post-Condition: Creates a deep copy of the lumens from the other Nova object into the 
void Nova::copyLumens(const Nova& other) {
    num_lumens = other.num_lumens;
    lumens = new Lumen*[num_lumens];
    for (int i = 0; i < num_lumens; i++) {
        lumens[i] = new Lumen(*other.lumens[i]);
    }
}

// Pre-Condition: other must be a valid Nova object
// Post-Condition: Transfers ownership of lumens from the other Nova object to the current 
void Nova::moveLumens(Nova&& other) noexcept {
    num_lumens = other.num_lumens;
    lumens = other.lumens;
    other.num_lumens = 0;
    other.lumens = nullptr;
}

// Pre-Condition: lumens must be a valid array of Lumen pointers
// Post-Condition: Frees the memory allocated for the lumens and their array
void Nova::freeMemory() {
    for (int i = 0; i < num_lumens; i++) {
        delete lumens[i];
    }
    delete[] lumens;
}

// Pre-Condition: None
// Post-Condition: Returns the minimum glow value among all lumens in the Nova object
int Nova::minGlow() const {
    // If there are no lumens, return an appropriate value or error
    if(num_lumens == 0) {
        throw std::runtime_error("No lumens in the Nova object.");
    }

    // Assume the first lumen has the minimum glow value
    int min_glow_value = lumens[0]->currentGlowValue();

    for (int i = 1; i < num_lumens; i++) {
        int current_glow_value = lumens[i]->currentGlowValue();
        if (current_glow_value < min_glow_value) {
            min_glow_value = current_glow_value;
        }
    }

    return min_glow_value;
}

// Pre-Condition: None
// Post-Condition: Returns the maximum glow value among all lumens in the Nova object
int Nova::maxGlow() const {
    // If there are no lumens, return an appropriate value or error
    if(num_lumens == 0) {
        throw std::runtime_error("No lumens in the Nova object.");
    }

    // Assume the first lumen has the maximum glow value
    int max_glow_value = lumens[0]->currentGlowValue();

    for (int i = 1; i < num_lumens; i++) {
        int current_glow_value = lumens[i]->currentGlowValue();
        if (current_glow_value > max_glow_value) {
            max_glow_value = current_glow_value;
        }
    }

    return max_glow_value;
}


// Pre-Condition: None
// Post-Condition: Recharges inactive lumens if more than half of the lumens in the Nova object are inactive
void Nova::rechargeInactiveLumens() {
    int inactive_count = 0;
    // Count the number of inactive lumens
    for (int i = 0; i < num_lumens; i++) {
        if (!lumens[i]->isActive()) {
            inactive_count++;
        }
    }
    // Recharge lumens if more than half are inactive
    if (inactive_count > num_lumens / 2) {
        for (int i = 0; i < num_lumens; i++) {
            if (!lumens[i]->isErratic() && lumens[i]->isActive()) {
                lumens[i]->recharge();
            }
        }
    }
}


// /************************************** Getters **************************************/

// // Pre-Condition: index must be a valid integer within the range of the number of lumens
// // Post-Condition: Returns a pointer to the lumen at the given index
// Lumen* Nova::get_lumen(int index) const{
//     if (index < 0 || index >= num_lumens) {
//         throw std::out_of_range("Index is out of range.");
//     }
//     return lumens[index];
// }

// // Pre-Condition: None
// // Post-Condition: Returns the number of lumens in the Nova object
// int Nova::get_num_lumens() const {
//     return num_lumens;
// }


/*
Implementation invariants for the Nova
- A Nova object must always have a non-negative brightness, size, and power values.
- The derived properties, such as dimming_value and power_threshold, must always be calculated and updated based on the original properties.
- The glow() method must always return a non-negative value based on the current state of the Nova object.
- The reset() and recharge() methods must always restore the brightness and power of the Nova object to their original values.
- The isActive(), isErratic(), and resetRequest() methods must always return a boolean value based on the current state of the Nova object.
- The current_glow_value() method must always return the current brightness value based on the current state of the Nova object.
*/