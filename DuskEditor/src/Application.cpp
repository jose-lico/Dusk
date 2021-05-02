#include "DuskEngine.h"

#include <iostream>

int main() 
{
    DuskEngine::DuskEngine d;
    int sum = d.Sum(5, 6);
    d.Greeting();

    std::cout << sum << std::endl;

    return 0;
}