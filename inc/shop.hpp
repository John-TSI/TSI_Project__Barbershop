#ifndef SHOP_HPP
#define SHOP_HPP

#include<mutex>
#include<condition_variable>
#include<memory>
#include<queue>
#include"barber.hpp"
#include"customer.hpp"


static std::mutex queueMutex, terminalMutex;
static std::condition_variable sleepCV;


struct Barbershop
{
    // --- attributes ---
    int maxQueueSize = 6;
    bool sleeping = false;
    Barber barber;
    std::queue<std::unique_ptr<Customer>> queue = {};

    // --- methods ---
    Barbershop();
    void BarberProcess();
    void CustomerProcess();

/*     void BarberThread();
    void CustomerThread(); */
/*     std::thread BarberThread();
    std::thread CustomerThread(); */
};

#endif
