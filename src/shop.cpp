#include<iostream>
#include<time.h>
#include<thread>
#include<chrono>
#include"../inc/shop.hpp"


// --- constructors ---
Barbershop::Barbershop() :
pBarber(std::make_unique<Barber>())
{
    srand(time(NULL)); // seed rand()
}


// --- barber actions ---
void Barbershop::BarberSleeps(unique_lock<mutex>& qL, unique_lock<mutex>& tL)
{
    pBarber->GoToSleep();
    tL.unlock();
    sleepCV.wait(qL);
}

void Barbershop::BarberCutsHair(unique_lock<mutex>& qL, unique_lock<mutex>& tL)
{
    queue.pop();
    qL.unlock();
    std::cout << "The barber is cutting a customer's hair; " << queue.size() << " customers are waiting.\n";
    tL.unlock();
    std::this_thread::sleep_for(std::chrono::seconds((rand()%2)+2));
    tL.lock();
    std::cout << "The barber has finished a haircut and finds " << queue.size() << " customers waiting.\n";  
}


// --- customer actions ---
void Barbershop::CustomerWakesBarber(std::unique_lock<std::mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    pBarber->WakeUp();
    sleepCV.notify_one();
}

void Barbershop::CustomerWaits(std::unique_lock<std::mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    std::cout << "A customer has joined the queue; there are " << queue.size() << " customers waiting.\n";
}

void Barbershop::CustomerLeaves(std::unique_lock<std::mutex>& qL)
{
    qL.unlock();
    std::cout << "A customer has been unable to join the queue.\n";
}


// --- processes ---
void Barbershop::BarberProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(terminalMutex);
        if(queue.size() == 0)
        {
            BarberSleeps(queueLock, terminalLock);
        }
        else
        {
            BarberCutsHair(queueLock, terminalLock);
        }
    }   
}

void Barbershop::CustomerProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(terminalMutex);
        if(pBarber->GetIsSleeping())
        { 
            CustomerWakesBarber(queueLock);
        }
        else if(queue.size() < maxQueueSize)
        {
            CustomerWaits(queueLock);
        }
        else
        {
            CustomerLeaves(queueLock);
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