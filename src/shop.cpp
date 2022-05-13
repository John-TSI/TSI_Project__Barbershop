#include<iostream>
#include<thread>
#include<chrono>
#include"../inc/shop.hpp"


// --- constructors ---
Barbershop::Barbershop()
{
    srand(time(NULL)); // seed rand()
    //barberVec.push_back(std::make_unique<Barber>(numBarbers));
}


// --- getters/setters ---
std::vector<unique_ptr<Barber>>& Barbershop::GetBarbers() { return barberVec; }


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
    std::cout << "The barber is cutting " << pB->GetCurrentCustomer()->GetName() << "'s hair; " << queue.size() << " customers are waiting.\n\n";
    cL.unlock();
    std::this_thread::sleep_for(std::chrono::seconds( (rand()%2)+2 ));
    cL.lock();
    std::cout << "The barber has finished " << pB->GetCurrentCustomer()->GetName() << "'s haircut and finds " << queue.size() << " customers waiting.\n\n";
}


// --- customer actions ---
void Barbershop::CustomerWakesBarber(std::unique_lock<std::mutex>& qL)
{
    queue.push(std::make_unique<Customer>());
    qL.unlock();
    barberVec.back()->WakeUp(queue.back()->GetName());
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
void Barbershop::BarberProcess(unique_ptr<Barber>& pBarber)
{
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> coutLock(coutMutex);
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
    while(true)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        std::unique_lock<std::mutex> terminalLock(coutMutex);
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
    CreateCustomerThread();
    JoinAllThreads();
}