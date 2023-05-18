/*
Name: Sergio Satyabrata
Date: April 14, 2023 
Class: CPSC-3200
Revision History: Revised
Platform: MacBook Pro (OSX)

The driver program for the Lumen and Nova classes is designed to demonstrate and test the
functionality of these classes. It focuses on creating multiple Nova objects, 
manipulating their internal Lumen objects, and displaying the results of these operations. 


ASSUMPTIONS:

1) The number of Nova objects and the number of Lumens per Nova object are fixed and 
known at compile-time (as defined by the constants num_novas and num_lumens_per_nova). 
This allows for the use of arrays for storing Nova objects and simplifies memory management.

2) The client assumes that the Nova and Lumen classes are implemented correctly and provide 
the necessary member functions to manipulate and query the state of the objects. 
This allows the driver to focus on testing and demonstrating the functionality provided by the classes.

3) The client assumes that the user has sufficient knowledge about the Lumen and Nova 
classes and how they interact. This allows for the use of comments and display messages 
in the driver that explain the purpose and outcome of each operation.

4) The client assumes that the memory allocated for the Nova objects is sufficient and 
that the new operator does not fail. This simplifies memory management and error handling in the driver.

5) The client assumes that the functions called in the main() function are implemented 
correctly and do not throw any exceptions. This allows for a more straightforward flow of
execution and easier debugging.
*/

#include <iostream>
#include "nova.h"

const int num_novas = 3;
const int num_lumens_per_nova = 5;
Nova* novas[num_novas];

void display_nova_glows(const Nova* const [], int );
void display_glow_values(const Nova* );

void initialize_novas(Nova**, int, int);
void glow_novas(Nova**, int, int);
void glow_novas_50_times(Nova**, int, int);
void reset_all_lumens(Nova**, int, int);


void modify_nova(Nova);
void test_nova_copying_call_by_value_and_assignment();

void test_nova_move_assignment();
void test_nova_move_value();


int main() {
    initialize_novas(novas, num_novas, num_lumens_per_nova);

    std::cout << "********* Initializing all Novas ********* " << std::endl;
    glow_novas(novas, num_novas, 5);
    std::cout << std::endl;
    display_nova_glows(novas, num_novas);

    std::cout << "******** Glowing Lumens in Nova 50 times *******" << std::endl;
    glow_novas_50_times(novas, num_novas, 5);
    std::cout << std::endl;
    display_nova_glows(novas, num_novas);

    std::cout << "********* Copy *********" << std::endl;
    test_nova_copying_call_by_value_and_assignment();
    std::cout << std::endl;

    std::cout << "********* Move *********" << std::endl;
    test_nova_move_value();
    std::cout << std::endl;
    test_nova_move_assignment();

    // Free the memory when done
    for (int i = 0; i < num_novas; ++i) {
        delete novas[i];
    }

    return 0;
}

void display_nova_glows(const Nova* const novas[], int num_novas) {
    // Display the glow values for each Nova object
    for (int i = 0; i < num_novas; ++i) {
        std::cout << "Glow values for Nova " << i + 1 << ":" << std::endl;
        
        std::cout << "Min glow value for Nova " << i + 1 << ": " << novas[i]->minGlow() << std::endl;
        std::cout << "Max glow value for Nova " << i + 1 << ": " << novas[i]->maxGlow() << std::endl;
        std::cout << std::endl;
    }
}

void initialize_novas(Nova** novas, int num_novas, int num_lumens_per_nova){
    for (int i = 0; i < num_novas; ++i) {
        int initial_brightness = 10 + 5 * i;
        int initial_size = 2 + i;
        int initial_power = 50 + 10 * i;
        novas[i] = new Nova(initial_brightness, initial_size, initial_power, num_lumens_per_nova);
    }
}

void glow_novas(Nova** novas, int num_novas, int lumens_to_glow) {
    for (int i = 0; i < num_novas; ++i) {
        novas[i]->glow(lumens_to_glow);
    }
}

void glow_novas_50_times(Nova** novas, int num_novas, int lumens_to_glow) {
    for (int i = 0; i < num_novas; i++) {
        for (int j = 0; j < 50; j++) {
            novas[i]->glow(lumens_to_glow);
        }
    }
}

// Function to demonstrate call by value for Nova objects
void modify_nova(Nova nova_object) {
    nova_object.glow(2);
}

void test_nova_copying_call_by_value_and_assignment() {
    // Create a Nova object
    Nova original_nova(20, 10, 200, 5);
    
    std::cout << "Original min glow (before modification): " << original_nova.minGlow() << std::endl;
    std::cout << "Original max glow (before modification): " << original_nova.maxGlow() << std::endl;

    // Call by value: pass the Nova object to the modify_nova function
    modify_nova(original_nova);

    std::cout << "Original min glow (after call by value): " << original_nova.minGlow() << std::endl;
    std::cout << "Original max glow (after call by value): " << original_nova.maxGlow() << std::endl;

    // Assignment: create a new Nova object and assign the original_nova to it
    Nova assigned_nova(30, 15, 300, 6);
    assigned_nova = original_nova;

    std::cout << "Assigned min glow: " << assigned_nova.minGlow() << std::endl;
    std::cout << "Assigned max glow: " << assigned_nova.maxGlow() << std::endl;
}

void test_nova_move_value(){
    std::cout << "Min Glow of Nova 1: " << novas[1]->minGlow() << std::endl;
    std::cout << "Max Glow of Nova 1: " << novas[1]->maxGlow() << std::endl;

    Nova* novar(std::move(novas[1]));
    std::cout << "novar(moved nova)  Min Glow " << novar->minGlow() << std::endl;
    std::cout << "novar(moved nova)  Max Glow " << novar->maxGlow() << std::endl;
}

void test_nova_move_assignment(){
    std::cout << "Min Glow of Nova 2: " << novas[2]->minGlow() << std::endl;
    std::cout << "Max Glow of Nova 2: " << novas[2]->maxGlow() << std::endl;

    Nova* noval = std::move(novas[2]);
    std::cout << "noval(moved nova)  Min Glow " << noval->minGlow() << std::endl;
    std::cout << "noval(moved nova)  Max Glow " << noval->maxGlow() << std::endl;
}
