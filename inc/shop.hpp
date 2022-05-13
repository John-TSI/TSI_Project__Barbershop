#ifndef SHOP_HPP
#define SHOP_HPP

#include<thread>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<queue>
#include"barber.hpp"

using std::unique_lock; using std::mutex;
using std::condition_variable;
using std::unique_ptr;


class Barbershop
{
    mutex queueMutex, coutMutex;
    condition_variable sleepCV;

    int numBarbers = 0, maxQueueSize = 6;
    std::vector<unique_ptr<Barber>> barberVec = {};
    std::queue<unique_ptr<Customer>> queue = {};
    std::vector<std::thread> threadVec = {};


    public:
        // --- barber actions ---
        void BarberSleeps(unique_ptr<Barber>&, unique_lock<mutex>&, unique_lock<mutex>&);
        void BarberCutsHair(unique_ptr<Barber>&, unique_lock<mutex>&, unique_lock<mutex>&);

        // --- customer actions ---
        void CustomerWakesBarber(unique_lock<mutex>&);
        void CustomerWaits(unique_lock<mutex>&);
        void CustomerLeaves(unique_lock<mutex>&);

        // --- processes ---
        void BarberProcess(unique_ptr<Barber>&);
        void CustomerProcess();

        // --- threads ---
        void CreateBarberThread();
        void CreateCustomerThread();
        void JoinAllThreads();
        void Run();
};

#endif
