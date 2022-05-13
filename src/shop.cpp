#include<iostream>
#include<chrono>
#include"../inc/shop.hpp"


// --- barber actions ---
void Barbershop::BarberSleeps(unique_ptr<Barber>& pB, unique_lock<mutex>& qL, unique_lock<mutex>& cL)
{
    pB->GoToSleep();
    cL.unlock();
    sleepCV.wait(qL);
}

void Barbershop::BarberCutsHair(unique_ptr<Barber>& pB, unique_lock<mutex>& qL, unique_lock<mutex>& cL)
{   
    pB->SetCurrentCustomer(queue.front());
    queue.pop();
    qL.unlock();
   // std::cout << "The barber is cutting " << pB->GetCurrentCustomer()->GetName() << "'s hair; " << queue.size() << " customers are waiting.\n\n";
    std::cout << "Barber " << pB->GetID() << " is cutting " << pB->GetCurrentCustomer()->GetName() << "'s hair; " << queue.size() << " customers are waiting.\n\n";
    cL.unlock();
    std::this_thread::sleep_for(std::chrono::seconds( (rand()%2)+2 ));
    cL.lock();
    //std::cout << "The barber has finished " << pB->GetCurrentCustomer()->GetName() << "'s haircut and finds " << queue.size() << " customers waiting.\n\n";
    std::cout << "Barber " << pB->GetID() << " has finished " << pB->GetCurrentCustomer()->GetName() << "'s haircut and finds " << queue.size() << " customers waiting.\n\n";
}


// --- customer actions ---
void Barbershop::CustomerWakesBarber(unique_lock<mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    barberVec.back()->WakeUp(queue.back()->GetName());
    sleepCV.notify_one();
}

void Barbershop::CustomerWaits(unique_lock<mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    std::cout << queue.back()->GetName() << " has joined the queue; there are now " << queue.size() << " customers waiting.\n\n";
}

void Barbershop::CustomerLeaves(unique_lock<mutex>& qL)
{
    qL.unlock();
    std::cout << "A customer has been unable to join the queue.\n\n";
}


// --- processes ---
void Barbershop::BarberProcess(unique_ptr<Barber>& pBarber)
{
    srand(time(NULL));
    while(true)
    {
        unique_lock<mutex> queueLock(queueMutex), coutLock(coutMutex);
        if(queue.size() == 0)
        {
            BarberSleeps(pBarber, queueLock, coutLock);
        }
        else
        {
            BarberCutsHair(pBarber, queueLock, coutLock);
        }
    }   
}

void Barbershop::CustomerProcess()
{
    srand(time(NULL));
    while(true)
    {
        unique_lock<mutex> queueLock(queueMutex), coutLock(coutMutex);
        if(barberVec.back()->GetIsSleeping())
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


// --- threads ---
void Barbershop::CreateBarberThread()
{
    barberVec.push_back(std::make_unique<Barber>(numBarbers));
    numBarbers++;
    std::thread barberThread(&Barbershop::BarberProcess, this, std::ref(barberVec.back()) );
    threadVec.push_back(move(barberThread));
}

void Barbershop::CreateCustomerThread()
{
    std::thread customerThread(&Barbershop::CustomerProcess, this);
    threadVec.push_back(move(customerThread));   
}

void Barbershop::JoinAllThreads()
{
    std::vector<std::thread>::iterator it = threadVec.begin();
    while(it!=threadVec.end())
    {
        std::thread& th = *it;
        th.join();
        ++it;
    }
}

void Barbershop::Run()
{
    CreateBarberThread();
    //CreateBarberThread();
    CreateCustomerThread();
    JoinAllThreads();
}