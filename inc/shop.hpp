#ifndef SHOP_HPP
#define SHOP_HPP

#include<mutex>
#include<condition_variable>
#include"customer.hpp"


std::mutex queueMutex, terminalMutex;
std::condition_variable sleepCV;


struct Barbershop
{
    // --- attributes ---

    // --- methods ---
};

#endif
