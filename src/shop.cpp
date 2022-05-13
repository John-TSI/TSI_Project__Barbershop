#include<iostream>
#include<thread>
#include<chrono>
#include"../inc/shop.hpp"


// --- constructors ---
Barbershop::Barbershop() :
pBarber(std::make_unique<Barber>(numBarbers))
{
    srand(time(NULL)); // seed rand()
}


// --- threads ---
/* void Barbershop::CreateBarberThread()
{
    unique_ptr<Barber> u_ptr = std::make_unique<Barber>(numBarbers);
    barberVec.push_back(u_ptr);
    numBarbers++;
    std::thread barberThread(&Barbershop::BarberProcess, this); // , std::ref(barberVec.back())
    barberThread.detach();
} */


// --- barber actions ---
void Barbershop::BarberSleeps(unique_lock<mutex>& qL, unique_lock<mutex>& cL)
{
    pBarber->GoToSleep();
    cL.unlock();
    sleepCV.wait(qL);
}

void Barbershop::BarberCutsHair(unique_lock<mutex>& qL, unique_lock<mutex>& cL)
{   
    pBarber->SetCurrentCustomer(queue.front());
    queue.pop();
    qL.unlock();
    std::cout << "The barber is cutting " << pBarber->GetCurrentCustomer()->GetName() << "'s hair; " << queue.size() << " customers are waiting.\n\n";
    cL.unlock();
    std::this_thread::sleep_for(std::chrono::seconds( (rand()%2)+2 ));
    cL.lock();
    std::cout << "The barber has finished " << pBarber->GetCurrentCustomer()->GetName() << "'s haircut and finds " << queue.size() << " customers waiting.\n\n";
}


// --- customer actions ---
void Barbershop::CustomerWakesBarber(std::unique_lock<std::mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    pBarber->WakeUp(queue.back()->GetName());
    sleepCV.notify_one();
}

void Barbershop::CustomerWaits(std::unique_lock<std::mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    std::cout << queue.back()->GetName() << " has joined the queue; there are " << queue.size() << " customers waiting.\n\n";
}

void Barbershop::CustomerLeaves(std::unique_lock<std::mutex>& qL)
{
    qL.unlock();
    std::cout << "A customer has been unable to join the queue.\n\n";
}


// --- processes ---
void Barbershop::BarberProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> coutLock(coutMutex);
        if(queue.size() == 0)
        {
            BarberSleeps(queueLock, coutLock);
        }
        else
        {
            BarberCutsHair(queueLock, coutLock);
        }
    }   
}

void Barbershop::CustomerProcess()
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(coutMutex);
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
        std::this_thread::sleep_for(std::chrono::seconds( (rand()%4)+4 ));
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