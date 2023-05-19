#include <iostream>
#include "lumen.h"
#include "nova.h"
using namespace std;
int main() {
    // Create two Nova objects with same properties
    Nova nova1(5, 10, 20, 3);
    Nova nova2(5, 10, 20, 3);

    // Test equality
    if (nova1 == nova2) {
        std::cout << "Nova1 and Nova2 are equal" << std::endl;
    } else {
        std::cout << "Nova1 and Nova2 are not equal" << std::endl;
    }

    // Modify nova2
    nova2.glow(2);

    // Test inequality
    if (nova1 != nova2) {
        std::cout << "Nova1 and Nova2 are not equal" << std::endl;
    } else {
        std::cout << "Nova1 and Nova2 are equal" << std::endl;
    }

    // Create two new Nova objects for comparing
    Nova nova3(1, 2, 3, 2);
    Nova nova4(2, 3, 4, 2);

    // Test '<' operator
    if (nova3 < nova4) {
        std::cout << "Nova3 is less than Nova4" << std::endl;
    } else {
        std::cout << "Nova3 is not less than Nova4" << std::endl;
    }

    // Test '>' operator
    if (nova3 > nova4) {
        std::cout << "Nova3 is greater than Nova4" << std::endl;
    } else {
        std::cout << "Nova3 is not greater than Nova4" << std::endl;
    }

    // Test '<=' operator
    if (nova3 <= nova4) {
        std::cout << "Nova3 is less than or equal to Nova4" << std::endl;
    } else {
        std::cout << "Nova3 is not less than or equal to Nova4" << std::endl;
    }

    // Test '>=' operator
    if (nova3 >= nova4) {
        std::cout << "Nova3 is greater than or equal to Nova4" << std::endl;
    } else {
        std::cout << "Nova3 is not greater than or equal to Nova4" << std::endl;
    }

    return 0;
}