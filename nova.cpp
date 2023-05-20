/*
Name: Sergio Satyabrata
Date: May 19, 2023 
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


NOTE: 
- When adding a Nova to another Nova with the + operator, we are adding the contents of the Nova, meaning the lumens will get added up.
- When subtracting a Nova from another Nova, we are subtracting the values within the Nova, meaning the Lumens' values. If it is negative or 0, they are set to 1.
- The -= and += subtracts/adds the Nova on the right side to the left side. It must be in this order.
- When using the -- or ++ it removes/adds 1 lumen from/to the Nova. 
- When adding or subtracting a value to or from the Nova, + (int value), it will add / remove the number of lumens within the Nova. The value must be at the right side of the Nova.
e.g novas + 5
*/

#include "nova.h"
#include <stdexcept>
#include <iostream>

/************************************ Nova Constructors *******************************************/

// Pre-Condition: initial brightness, size, and power should not be negative; should be positive
// Post-Condition: first lumen is initialized along with the rest of the lumens for the nova
Nova::Nova(ILuminosity* luminate, int initialBrightness, int initialSize, int initialPower, int numLumens) {
    if (initialBrightness <= 0 || initialSize <= 0 || initialPower <= 0 || numLumens <=0) {
        throw std::out_of_range("All input values for Nova must be positive.");
    }
    this->numLumens = numLumens;
    this->luminate = luminate;
    lumens = new Lumen*[numLumens];

    for (int i = 0; i < numLumens; i++) {
        int brightness = initialBrightness + i;
        int size = initialSize + i;
        int power = initialPower + i * 10;
        lumens[i] = luminate->illuminate(brightness, size, power);
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
: numLumens(other.numLumens), lumens(other.lumens) {
    other.numLumens = 0;
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
        numLumens = other.numLumens;
        lumens = other.lumens;

        // Reset the source object's resource pointers
        other.numLumens = 0;
        other.lumens = nullptr;
    }
    return *this;
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if both instances are the same, false otherwise
bool Nova::operator==(const Nova& other) const{
    if (this->numLumens != other.numLumens) {
        return false;
    }
    for (int i = 0; i < numLumens; i++) {
        if (!(*this->lumens[i] == *other.lumens[i])) {
            return false;
        }
    }
    return true;
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if both instances are not the same, false otherwise
bool Nova::operator!=(const Nova& other) const{
    return !(*this == other);
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if 'this' instance has fewer lumens than 'other', false otherwise
bool Nova::operator<(const Nova& other) const{
    return numLumens < other.numLumens;
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if 'this' instance has more lumens than 'other', false otherwise
bool Nova::operator>(const Nova& other) const{
    return numLumens > other.numLumens;
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if 'this' instance has fewer or equal lumens than 'other', false otherwise
bool Nova::operator<=(const Nova& other) const{
    return !(*this > other);
}

// Precondition: 'other' is an instance of Nova that can be compared with 'this' instance
// Postcondition: returns true if 'this' instance has more or equal lumens than 'other', false otherwise
bool Nova::operator>=(const Nova& other) const{
    return !(*this < other);
}

// Precondition: 'other' is an instance of Nova with the same number of lumens as 'this' instance
// Postcondition: returns a new Nova instance that is the sum of 'this' and 'other' instance
Nova Nova::operator+(const Nova& other) const{
    // Ensure that Nova1 and Nova2 have the same number of lumens
    if (this->numLumens != other.numLumens) {
        throw std::invalid_argument("Nova objects must have the same number of lumens to be added together.");
    }

    // New size is the size of either Nova (since they have the same size)
    int newSize = this->numLumens;

    // Create a new Nova with arbitrary initial brightness, size, and power.
    // The new Nova won't actually own any Lumen objects yet.
    Nova result(this->luminate, 1, newSize, 1, newSize);

    // Add the properties of corresponding Lumen objects in Nova1 and Nova2
    for (int i = 0; i < newSize; i++) {
        int newBrightness = this->lumens[i]->getBrightness() + other.lumens[i]->getBrightness();
        int newSize = this->lumens[i]->getSize() + other.lumens[i]->getSize();
        int newPower = this->lumens[i]->getPower() + other.lumens[i]->getPower();
        
        result.lumens[i] = this->luminate->illuminate(newBrightness, newSize, newPower);
    }

    return result;
} // standard

// Precondition: 'value' is an integer to be added to 'this' instance
// Postcondition: returns a new Nova instance that has 'value' number of new lumens added
Nova Nova::operator+(int value) const{
    // Create a copy of the current Nova.
    Nova result(*this);
    
    // Add the specified number of default Lumens.
    for (int i = 0; i < value; i++) {
        Lumen* newLumen = new Lumen(1, result.numLumens + i, 1);

        // Dynamically allocate a new array to hold the expanded Lumens.
        Lumen** newLumens = new Lumen*[result.numLumens + 1];
        
        // Copy the old Lumens over.
        for (int j = 0; j < result.numLumens; j++) {
            newLumens[j] = result.lumens[j];
        }

        // Add the new Lumen.
        newLumens[result.numLumens] = newLumen;
        
        // Clean up and update properties.
        delete[] result.lumens;
        result.lumens = newLumens;
        result.numLumens += 1;
    }

    return result;
} //mixed mode


// Precondition: 'other' is an instance of Nova that can be added to 'this' instance
// Postcondition: 'this' instance has been modified to include the lumens from 'other' instance
Nova& Nova::operator+=(const Nova& other){
    // This operation will simply extend the current Nova by adding the contents of the other Nova.
    // We'll need to dynamically allocate a new array, copy the contents over, and then clean up.
    int newSize = this->numLumens + other.numLumens;
    Lumen** newLumens = new Lumen*[newSize];
    
    // Copy contents of the current Nova.
    for (int i = 0; i < this->numLumens; i++) {
        newLumens[i] = this->lumens[i];
    }
    
    // Copy contents of the other Nova.
    for (int i = 0; i < other.numLumens; i++) {
        newLumens[i + this->numLumens] = other.lumens[i];
    }
    
    // Clean up and update properties.
    delete[] this->lumens;
    this->lumens = newLumens;
    this->numLumens = newSize;
    
    return *this;
} // shortcut standard

// Precondition: 'value' is an integer indicating the number of new lumens to be added to 'this' instance
// Postcondition: 'this' instance has been modified to include 'value' number of new lumens
Nova& Nova::operator+=(int value){
    int newSize = this->numLumens + value;
    Lumen** newLumens = new Lumen*[newSize];
    
    // Copy contents of the current Nova.
    for (int i = 0; i < this->numLumens; i++) {
        newLumens[i] = this->lumens[i];
    }
    
    // Add new Lumens.
    for (int i = this->numLumens; i < newSize; i++) {
        newLumens[i] = new Lumen(1, i, 1); // Assuming this creates a default Lumen.
    }
    
    // Clean up and update properties.
    delete[] this->lumens;
    this->lumens = newLumens;
    this->numLumens = newSize;
    
    return *this;
} // shortcut mixed mode


// Precondition: none
// Postcondition: 'this' instance has been modified to include one new default lumen
Nova& Nova::operator++(){
    // In this case, we will just add one default Lumen to the current Nova.
    int newSize = this->numLumens + 1;
    Lumen** newLumens = new Lumen*[newSize];
    
    // Copy contents of the current Nova.
    for (int i = 0; i < this->numLumens; i++) {
        newLumens[i] = this->lumens[i];
    }
    
    // Add new Lumen.
    newLumens[this->numLumens] = new Lumen(1, this->numLumens, 1); 
    
    // Clean up and update properties.
    delete[] this->lumens;
    this->lumens = newLumens;
    this->numLumens = newSize;
    
    return *this;
} // prefix increment

// Precondition: none
// Postcondition: returns a copy of the original 'this' instance and modifies 'this' instance to include one new default lumen
Nova Nova::operator++(int){
    // Create a copy of the current object.
    Nova copy = *this;
    
    // Use the prefix increment on the current object.
    ++(*this);
    
    // Return the original un-incremented copy.
    return copy;
} // postfix increment

// Precondition: 'other' is an instance of Nova with equal or fewer number of lumens as 'this' instance
// Postcondition: returns a new Nova instance that is the result of subtracting 'other' instance from 'this' instance
Nova Nova::operator-(const Nova& other) const{
    if (this->numLumens != other.numLumens) {
        throw std::out_of_range("Cannot subtract Novas with different numbers of Lumens.");
    }

    // Create a new Nova to hold the result.
    Nova result(*this);

    // Subtract each Lumen.
    for (int i = 0; i < numLumens; i++) {
        int brightness = this->lumens[i]->getBrightness() - other.lumens[i]->getBrightness();
        int size = this->lumens[i]->getSize() - other.lumens[i]->getSize();
        int power = this->lumens[i]->getPower() - other.lumens[i]->getPower();

        // If any properties are zero or negative, set them to 1.
        if (brightness <= 0) brightness = 1;
        if (size <= 0) size = 1;
        if (power <= 0) power = 1;

        // Set the properties of the Lumen in the result Nova.
        delete result.lumens[i];
        result.lumens[i] = new Lumen(brightness, size, power);
    }

    return result;
} // standard

// Precondition: 'value' is an integer less than or equal to the number of lumens in 'this' instance
// Postcondition: returns a new Nova instance that is the result of subtracting 'value' lumens from 'this' instance
Nova Nova::operator-(int value) const{
    if (value >= numLumens) {
        throw std::runtime_error("Cannot subtract more lumens than the Nova object contains.");
    }

    // Create a new Nova object that is a copy of the current one.
    Nova novaCopy(*this);

    // Remove lumens from the copy.
    for (int i = 0; i < value; i++) {
        --novaCopy;
    }

    return novaCopy;
} // mixed mode 

// Precondition: 'other' is an instance of Nova with equal or fewer number of lumens as 'this' instance
// Postcondition: 'this' instance has been modified by subtracting the lumens from 'other' instance
Nova& Nova::operator-=(const Nova& other){
    if (other.numLumens > numLumens) {
        throw std::runtime_error("Cannot subtract a larger Nova from a smaller one.");
    }

    // Subtract other's lumens from this Nova's lumens.
    for (int i = 0; i < other.numLumens; i++) {
        --(*this);
    }

    return *this;
} // shortcut standard

// Precondition: 'value' is an integer less than or equal to the number of lumens in 'this' instance
// Postcondition: 'this' instance has been modified by subtracting 'value' lumens
Nova& Nova::operator-=(int value){
    if (value > numLumens) {
        throw std::runtime_error("Cannot subtract more lumens than the Nova object contains.");
    }

    // Subtract value lumens from this Nova.
    for (int i = 0; i < value; i++) {
        --(*this);
    }

    return *this;
} // shortcut mixed mode

// Precondition: 'this' instance has at least one lumen
// Postcondition: 'this' instance has been modified by subtracting one lumen
Nova& Nova::operator--(){
    if (numLumens <= 0) {
        throw std::runtime_error("Cannot decrement: no lumens in the Nova object.");
    }
    
    Lumen** newLumens = new Lumen*[numLumens - 1];

    // Copy all but the last Lumen.
    for (int i = 0; i < numLumens - 1; i++) {
        newLumens[i] = lumens[i];
    }

    // Delete the last Lumen and free its memory.
    delete lumens[numLumens - 1];
    
    // Free the old array and update the lumens pointer.
    delete[] lumens;
    lumens = newLumens;

    // Decrement the lumen count.
    --numLumens;

    return *this;

} // prefix increment

// Precondition: 'this' instance has at least one lumen
// Postcondition: returns a copy of the original 'this' instance and modifies 'this' instance by subtracting one lumen
Nova Nova::operator--(int){
    // Create a copy of the current state.
    Nova temp(*this);
    
    // Use the prefix operator to decrement this instance.
    --(*this);

    return temp;
} // postfix increment

/********************************* Nova Core Functions **********************************************/

// Pre-Condition: x should be a non-negative integer and less than or equal to the number of lumens
// Post-Condition: The first x lumens are made to glow, and inactive lumens are recharged if necessary
void Nova::glow(int x) {
    if (x < 0 || x > numLumens) {
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
    numLumens = other.numLumens;
    lumens = new Lumen*[numLumens];
    for (int i = 0; i < numLumens; i++) {
        lumens[i] = new Lumen(*other.lumens[i]);
    }
}

// Pre-Condition: other must be a valid Nova object
// Post-Condition: Transfers ownership of lumens from the other Nova object to the current 
void Nova::moveLumens(Nova&& other) noexcept {
    numLumens = other.numLumens;
    lumens = other.lumens;
    other.numLumens = 0;
    other.lumens = nullptr;
}

// Pre-Condition: lumens must be a valid array of Lumen pointers
// Post-Condition: Frees the memory allocated for the lumens and their array
void Nova::freeMemory() {
    for (int i = 0; i < numLumens; i++) {
        delete lumens[i];
    }
    delete[] lumens;
}

// Pre-Condition: None
// Post-Condition: Returns the minimum glow value among all lumens in the Nova object
int Nova::minGlow() const {
    // If there are no lumens, return an appropriate value or error
    if(numLumens == 0) {
        throw std::runtime_error("No lumens in the Nova object.");
    }

    // Assume the first lumen has the minimum glow value
    int minGlowValue = lumens[0]->currentGlowValue();

    for (int i = 1; i < numLumens; i++) {
        int currentGlowValue = lumens[i]->currentGlowValue();
        if (currentGlowValue < minGlowValue) {
            minGlowValue = currentGlowValue;
        }
    }

    return minGlowValue;
}

// Pre-Condition: None
// Post-Condition: Returns the maximum glow value among all lumens in the Nova object
int Nova::maxGlow() const {
    // If there are no lumens, return an appropriate value or error
    if(numLumens == 0) {
        throw std::runtime_error("No lumens in the Nova object.");
    }

    // Assume the first lumen has the maximum glow value
    int maxGlowValue = lumens[0]->currentGlowValue();

    for (int i = 1; i < numLumens; i++) {
        int currentGlowValue = lumens[i]->currentGlowValue();
        if (currentGlowValue > maxGlowValue) {
            maxGlowValue = currentGlowValue;
        }
    }

    return maxGlowValue;
}

// Pre-Condition: None
// Post-Condition: Recharges inactive lumens if more than half of the lumens in the Nova object are inactive
void Nova::rechargeInactiveLumens() {
    int inactive_count = 0;
    // Count the number of inactive lumens
    for (int i = 0; i < numLumens; i++) {
        if (!lumens[i]->isActive()) {
            inactive_count++;
        }
    }
    // Recharge lumens if more than half are inactive
    if (inactive_count > numLumens / 2) {
        for (int i = 0; i < numLumens; i++) {
            if (!lumens[i]->isErratic() && lumens[i]->isActive()) {
                lumens[i]->recharge();
            }
        }
    }
}

// Precondition: none
// Postcondition: returns the number of lumens within a Nova
int Nova::getNumLumens(){
    return numLumens;
}

/*
Implementation invariants for the Nova
- A Nova object must always have a non-negative brightness, size, and power values.
- The derived properties, such as dimming_value and power_threshold, must always be calculated and updated based on the original properties.
- The glow() method must always return a non-negative value based on the current state of the Nova object.
- The reset() and recharge() methods must always restore the brightness and power of the Nova object to their original values.
- The isActive(), isErratic(), and resetRequest() methods must always return a boolean value based on the current state of the Nova object.
- The currentGlowValue() method must always return the current brightness value based on the current state of the Nova object.
*/