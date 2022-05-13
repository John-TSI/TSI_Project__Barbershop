#include<iostream>
#include"../inc/barber.hpp"


// --- constructors ---
Barber::Barber(int barberID) : barberID(barberID) {}


// --- getters/setters ---
int Barber::GetID() const { return barberID; }
void Barber::SetID(const int& input) { barberID = input; }
bool Barber::GetIsSleeping() const { return isSleeping; }
void Barber::SetIsSleeping(bool input) { isSleeping = input; }
unique_ptr<Customer>& Barber::GetCurrentCustomer() { return pCurrentCustomer; }
void Barber::SetCurrentCustomer(unique_ptr<Customer>& u_ptr) { pCurrentCustomer = std::move(u_ptr); }


// --- other methods ---
void Barber::GoToSleep()
{
    isSleeping = true;
    //std::cout << "The barber finds no customers waiting, and has gone to sleep.\n\n";
    std::cout << "Barber " << barberID << " finds no customers waiting, and has gone to sleep.\n\n";
}

void Barber::WakeUp(const std::string& customer)
{
    isSleeping = false;
    //std::cout << customer << " arrives and wakes the barber.\n\n";
    std::cout << customer << " arrives and wakes barber " << barberID << ".\n\n";
}