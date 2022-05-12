#include<iostream>
#include<time.h>
#include<thread>
#include<chrono>
#include"../inc/shop.hpp"


Barbershop::Barbershop() :
barber(*std::make_unique<Barber>())
{
    srand(time(NULL)); // seed rand()
}

void Barbershop::BarberProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(terminalMutex);
        if(queue.size() == 0)
        {
            sleeping = true;
            std::cout << "The barber finds no customers waiting, and has gone to sleep.\n";
            terminalLock.unlock();
            sleepCV.wait(queueLock);
        }
        else
        {
            queue.pop();
            queueLock.unlock();
            std::cout << "The barber is cutting a customer's hair; " << queue.size() << " customers are waiting.\n";
            terminalLock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds((rand()%2)+2));
            terminalLock.lock();
            std::cout << "The barber has finished a haircut and finds " << queue.size() << " customers waiting.\n";  
        }
    }   
}

void Barbershop::CustomerProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(terminalMutex);
        if(sleeping)
        { 
            queue.push(std::make_unique<Customer>());
            queueLock.unlock();
            sleeping = false;
            sleepCV.notify_one();
            std::cout << "A customer arrives and wakes the barber.\n";
        }
        else if(queue.size() < maxQueueSize)
        {
            queue.push(std::make_unique<Customer>());
            queueLock.unlock();
            std::cout << "A customer has joined the queue; there are " << queue.size() << " customers waiting.\n";
        }
        else
        {
            queueLock.unlock();
            std::cout << "A customer has been unable to join the queue.\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds((rand()%4)+4));
    }
}

/* void Barbershop::BarberThread()
{
    std::thread barberThread(&Barbershop::BarberProcess, this);
}
void Barbershop::CustomerThread()
{
    std::thread customerThread(&Barbershop::CustomerProcess, this);
} */

/* std::thread Barbershop::BarberThread()
{
    std::thread barberThread(&Barbershop::BarberProcess, this);
    return barberThread;
}
std::thread Barbershop::CustomerThread()
{
    std::thread customerThread(&Barbershop::CustomerProcess, this);
    return customerThread;
} */