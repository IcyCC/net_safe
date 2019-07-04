//
// Created by zhandong on 2019-07-04.
//

#include "../src/CA.h"
#include <string>
#include <fstream>
#include <iostream>

int main() {
    CA ca = CA("10651_56153", "45257_227891_31939 44085 10430 44085 21968 30565 10430 10430 21968 34470 47454 20006 ");
    std::string result = ca.Dumps();
    CA ca1;
    ca1.Loads(result);
    return 0;
}