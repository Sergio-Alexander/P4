#include "lumen.h"
#include <iostream>
using namespace std;

int main(){

    Lumen bulb (5, 10, 15);

    bulb.glow();
    bulb.glow();

    cout << bulb.brightness << endl;
    cout << bulb.power << endl;
    cout << bulb.size << endl;
    cout << bulb.power_copy << endl;
    cout << bulb.brightness_copy << endl;

    // Lumen bulb2 (1, 2, 3);
    // bulb2.glow();

    // Lumen bulb3 = bulb + bulb2;
    // cout << bulb3.brightness << endl; // 6
    // cout << bulb3.power << endl; // 18
    // cout << bulb3.size << endl; // 12
    // cout << bulb3.power_copy << endl; //15
    // cout << bulb3.brightness_copy << endl; // 5
    // cout << bulb3.glow_request << endl;

    // Lumen bulb4 = bulb + 420;
    // cout << bulb4.brightness << endl; // 6
    cout << endl;
    ++bulb;
    cout << bulb.brightness << endl;
    cout << bulb.power << endl;
    cout << bulb.size << endl;
    cout << bulb.power_copy << endl;
    cout << bulb.brightness_copy << endl;

    int i = 10;

    int j = ++i;

    cout << j << endl;


    return 0;
}