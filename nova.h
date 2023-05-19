/*
Name: Sergio Satyabrata
Date: April 14, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

Nova.h is the header file for the Nova class, which represents a collection of Lumen objects 
that together form a more complex light source. The class provides methods for managing and 
manipulating the individual Lumen objects and simulating the overall behavior of the Nova light 
source in different conditions. The Nova class is responsible for maintaining the state of the 
Lumen objects, ensuring proper resource management, and providing methods to interact with and 
modify the collection of Lumens. This header file contains the declarations of the Nova class's 
member functions and variables, ensuring proper encapsulation and separation of concerns in the program.
*/


#ifndef NOVA_H
#define NOVA_H

#include "lumen.h"

class Nova {
public:
    Nova(int initial_brightness, int initial_size, int initial_power, int num_lumens);

    ~Nova();
    Nova(const Nova& other);
    Nova(Nova&& other) noexcept;
    Nova& operator=(const Nova& other);
    Nova& operator=(Nova&& other) noexcept;

    //Overloading Operators

    // comparison
    bool operator==(const Nova& other) const;
    bool operator!=(const Nova& other) const;
    bool operator<(const Nova& other) const;
    bool operator<=(const Nova& other) const;
    bool operator>(const Nova& other) const;
    bool operator>=(const Nova& other) const;

    // Arithmetic operators

    Nova operator+(const Nova& other) const; // standard
    Nova operator+(int value) const; // mixed mode 

    Nova& operator+=(const Nova& other); // shortcut standard
    Nova& operator+=(int value); // shortcut mixed mode

    Nova& operator++(); // prefix increment
    Nova operator++(int); // postfix increment


    Nova operator-(const Nova& other) const; // standard
    Nova operator-(int value) const; // mixed mode 

    Nova& operator-=(const Nova& other); // shortcut standard
    Nova& operator-=(int value); // shortcut mixed mode

    Nova& operator--(); // prefix increment
    Nova operator--(int); // postfix increment


    void glow(int x);
    int minGlow() const;
    int maxGlow() const;


private:
    Lumen** lumens;
    int num_lumens;

    int max_glow_value;
    int min_glow_value;

    void copyLumens(const Nova& other);
    void moveLumens(Nova&& other) noexcept;

    void freeMemory();

    void rechargeInactiveLumens();
};

#endif


/*
Class Invariants for the Nova class:

- The number of lumens (num_lumens) must always be non-negative and should be positive.
- The lumens pointer must always point to an array of Lumen pointers, or be nullptr if there are no lumens.
- The individual Lumen objects within the lumens array must be properly initialized and have valid state, according to the Lumen class invariants.
- The methods provided for managing and manipulating the Lumen objects (e.g., glow, reset_lumen) should maintain the invariants of the Lumen class and not introduce any inconsistencies in the state of the Lumen objects.
- The Nova class must ensure proper memory management for the dynamically allocated Lumen objects, including correct use of copy/move constructors, assignment operators, and the destructor.
*/