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
known at compile-time (as defined by the constants num_novas and num_lumens). 
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
#include <memory>
#include <vector>
#include "nova.h"

using namespace std;

const int num_novas = 5;
const int num_lumens = 5;

vector<unique_ptr<Nova>> novas;
Luminosity luminate;

void display_nova_glows();
void display_glow_values(const Nova& nova);

void initialize_novas(int num_novas, int num_lumens);
void glow_novas(int num_novas, int lumens_to_glow);
void glow_novas_50_times(int num_novas, int lumens_to_glow);

void modify_nova(Nova& nova_object);
void test_nova_copying_call_by_value_and_assignment(vector<unique_ptr<Nova>>& novav);

void test_nova_move_assignment();
void test_nova_move_value();

void testOperatorOverloads(vector<unique_ptr<Nova>>& novav);

void testCreateAndRemoveNova(vector<unique_ptr<Nova>>& novas);

int main() {
    initialize_novas(num_novas, num_lumens);

    std::cout << "********* Initializing all Novas ********* " << std::endl;
    glow_novas(num_novas, 5);
    std::cout << std::endl;
    display_nova_glows();

    std::cout << "******** Glowing Lumens in Nova 50 times *******" << std::endl;
    glow_novas_50_times(num_novas, 5);
    std::cout << std::endl;
    display_nova_glows();

    std::cout << "********* Copy *********" << std::endl;
    test_nova_copying_call_by_value_and_assignment(novas);
    std::cout << std::endl;

    std::cout << "********* Move *********" << std::endl;
    test_nova_move_value();
    std::cout << std::endl;
    test_nova_move_assignment();

    cout << "Testing operators" << endl;
    testOperatorOverloads(novas);



    cout << "Testing Add and Remove" << endl;
    testCreateAndRemoveNova(novas);

    return 0;


}

void display_nova_glows() {
    // Display the glow values for each Nova object
    for (size_t i = 0; i < novas.size(); ++i) {
        std::cout << "Glow values for Nova " << i + 1 << ":" << std::endl;
        display_glow_values(*novas[i]);
    }
}

void display_glow_values(const Nova& nova) {
    std::cout << "Min glow value: " << nova.minGlow() << std::endl;
    std::cout << "Max glow value: " << nova.maxGlow() << std::endl;
    std::cout << std::endl;
}

void initialize_novas(int num_novas, int num_lumens) {
    std::srand(std::time(nullptr)); // Initialize the random seed
    
    for (int i = 0; i < num_novas; i++) {
        int initial_brightness = rand() % 11 + 10;  // Random brightness between 10 and 20
        int initial_size = rand() % 2 + i + 2;      // Random size between (2 + i) and (3 + i)
        int initial_power = rand() % 11 + 50 + 10*i; // Random power between (50 + 10*i) and (60 + 10*i)

        novas.push_back(std::make_unique<Nova>(&luminate, initial_brightness, initial_size, initial_power, num_lumens));
    }
}

void glow_novas(int num_novas, int lumens_to_glow) {
    for (auto& nova : novas) {
        nova->glow(lumens_to_glow);
    }
}

void glow_novas_50_times(int num_novas, int lumens_to_glow) {
    for (auto& nova : novas) {
        for (int j = 0; j < 50; ++j) {
            nova->glow(lumens_to_glow);
        }
    }
}

void modify_nova(Nova& nova_object) {
    // Modify nova_object
    // Note: Modifications will affect the original Nova object due to call-by-reference
    nova_object.glow(3);
}

void test_nova_copying_call_by_value_and_assignment(vector<unique_ptr<Nova>>& novav) {
    Nova nova_copy = *novav[0];

    cout << "Min glow of original is: " << novav[0]->minGlow() << endl;
    cout << "Max glow of original is: " << novav[0]->maxGlow() << endl;

    cout << "Min glow of copied is: " << nova_copy.minGlow() << endl;
    cout << "Max glow of copied is: " << nova_copy.maxGlow() << endl;

}

void test_nova_move_assignment() {
    if (novas[0]) {
        Nova nova_move = move(*novas[0]); // move assignment
        novas[0].reset(); // It's a good practice to reset the unique_ptr after move
        cout << "Moved Nova glow values:" << endl;
        display_glow_values(nova_move);
    }
}

void test_nova_move_value() {
    if (novas[1]) {
        Nova nova_move(move(*novas[1])); // move constructor to move object
        novas[1].reset(); // It's a good practice to reset the unique_ptr after move
        cout << "Moved Nova glow values:" << endl;
        display_glow_values(nova_move);
    }
}

void testSharedPointer(vector<unique_ptr<Nova>>& novav){
    shared_ptr<Nova> nova_shared(novav.back().release());
    shared_ptr<Nova> test_nova_shared1 = nova_shared;
    shared_ptr<Nova> test_nova_shared2 = nova_shared;

    cout << "Initial Shared Nova: " << endl;

    cout << "Nova Shared use count is: " << nova_shared.use_count() << endl;
    cout << "Nova Shared1 use count is: " << test_nova_shared1.use_count() << endl;
    cout << "Nova Shared2 use count is: " << test_nova_shared2.use_count() << endl;

    nova_shared.reset();

    cout << "Nova Shared Resetted" << endl;

    cout << "Nova Shared use count is: " << nova_shared.use_count() << endl;
    cout << "Nova Shared1 use count is: " << test_nova_shared1.use_count() << endl;
    cout << "Nova Shared2 use count is: " << test_nova_shared2.use_count() << endl;
}

void testOperatorOverloads(vector<unique_ptr<Nova>>& novas) {
    // Assume that we have at least 5 Nova instances

    // Testing addition operator
    Nova sum = *novas[2] + *novas[3];
    cout << "Testing + operator:" << endl;
    display_glow_values(sum);

    // Testing addition operator for mixed mode
    Nova sum_mixed = *novas[2] + 5;
    cout << "Testing + operator with mixed mode:" << endl;
    display_glow_values(sum_mixed);
    
    // Testing subtraction operator
    Nova diff = *novas[4] - *novas[3];
    cout << "Testing - operator:" << endl;
    display_glow_values(diff);

    // Testing subtraction operator for mixed mode
    Nova diff_mixed = *novas[4] - 3;
    cout << "Testing - operator with mixed mode:" << endl;
    display_glow_values(diff_mixed);

    // Testing assignment operator
    Nova assigned = *novas[4];
    cout << "Testing = operator:" << endl;
    display_glow_values(assigned);

    // Testing equality and inequality operators
    if (*novas[4] == *novas[3])
        cout << "Testing == operator: Novas are equal" << endl;
    else
        cout << "Testing == operator: Novas are not equal" << endl;

    if (*novas[4] != *novas[3])
        cout << "Testing != operator: Novas are not equal" << endl;
    else
        cout << "Testing != operator: Novas are equal" << endl;

    // Testing less than, greater than, less than or equal to, and greater than or equal to operators
    if (*novas[3] < *novas[2])
        cout << "Testing < operator: Nova 3 is less than Nova 2" << endl;
    else
        cout << "Testing < operator: Nova 3 is not less than Nova 2" << endl;

    if (*novas[2] > *novas[4])
        cout << "Testing > operator: Nova 2 is greater than Nova 4" << endl;
    else
        cout << "Testing > operator: Nova 2 is not greater than Nova 4" << endl;

    if (*novas[3] <= *novas[2])
        cout << "Testing <= operator: Nova 3 is less than or equal to Nova 2" << endl;
    else
        cout << "Testing <= operator: Nova 3 is not less than or equal to Nova 2" << endl;

    if (*novas[2] >= *novas[4])
        cout << "Testing >= operator: Nova 2 is greater than or equal to Nova 4" << endl;
    else
        cout << "Testing >= operator: Nova 2 is not greater than or equal to Nova 4" << endl;

    // Testing increment operators
    Nova& nova_ref = *novas[4];
    ++nova_ref;
    cout << "Testing prefix increment operator:" << endl;
    display_glow_values(nova_ref);

    nova_ref++;
    cout << "Testing postfix increment operator:" << endl;
    display_glow_values(nova_ref);

    // Testing decrement operators
    --nova_ref;
    cout << "Testing prefix decrement operator:" << endl;
    display_glow_values(nova_ref);

    nova_ref--;
    cout << "Testing postfix decrement operator:" << endl;
    display_glow_values(nova_ref);
}


void testCreateAndRemoveNova(vector<unique_ptr<Nova>>& novas) {
    // Create a new Nova
    cout << "\nCreating a new Nova..." << endl;
    int initial_brightness = rand() % 11 + 10;  // Random brightness between 10 and 20
    int initial_size = rand() % 2 + 2;      // Random size between (2 + i) and (3 + i)
    int initial_power = rand() % 11 + 50 + 10 * 2; // Random power between (50 + 10*i) and (60 + 10*i)
    unique_ptr<Nova> newNova = make_unique<Nova>(&luminate, initial_brightness, initial_size, initial_power, num_lumens);
    novas.push_back(move(newNova));  // add new Nova to the vector
    cout << "A new Nova created and added. Total Novas now: " << novas.size() << "\n";

    // Display glow values of the newly created Nova
    display_glow_values(*novas.back());

    // Remove a Nova
    cout << "\nRemoving a Nova..." << endl;
    if (!novas.empty()) {
        novas.pop_back();  // removes the last Nova from the vector
        cout << "A Nova removed. Total Novas now: " << novas.size() << "\n";
    } else {
        cout << "No Nova to remove.\n";
    }
}
