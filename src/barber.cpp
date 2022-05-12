#include<iostream>
#include"../inc/barber.hpp"


// --- getters/setters ---
bool Barber::GetIsSleeping() const { return isSleeping; }
void Barber::SetIsSleeping(bool input) { isSleeping = input; }

// --- other methods ---
void Barber::GoToSleep()
{
    isSleeping = true;
    std::cout << "The barber finds no customers waiting, and has gone to sleep.\n";
}

void Barber::WakeUp()
{
    isSleeping = false;
    std::cout << "A customer arrives and wakes the barber.\n";
}