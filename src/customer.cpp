#include<time.h>
#include"../inc/customer.hpp"


// --- constructors ---
//Customer::Customer() : name( Names[rand()%8] ) {}

Customer::Customer() 
{
    srand(time(NULL));
    name = Names[rand()%8];
}


// --- getters/setters ---
std::string Customer::GetName() const { return name; }
void Customer::SetName(const std::string& input) { name = input; }