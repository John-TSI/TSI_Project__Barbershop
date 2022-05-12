#ifndef SHOP_HPP
#define SHOP_HPP

#include<mutex>
#include<condition_variable>
#include<memory>
#include<queue>
#include"barber.hpp"
#include"customer.hpp"

using std::unique_lock; using std::mutex;


class Barbershop
{
    std::mutex queueMutex, terminalMutex;
    std::condition_variable sleepCV;

    int maxQueueSize = 6;
    std::unique_ptr<Barber> pBarber;
    std::queue<std::unique_ptr<Customer>> queue = {};

    public:
        // --- constructors ---
        Barbershop();

        // --- barber actions ---
        void BarberSleeps(unique_lock<mutex>&, unique_lock<mutex>&);
        void BarberCutsHair(unique_lock<mutex>&, unique_lock<mutex>&);

        // --- customer actions ---
        void CustomerWakesBarber(unique_lock<mutex>&);
        void CustomerWaits(unique_lock<mutex>&);
        void CustomerLeaves(unique_lock<mutex>&);

        // --- processes ---
        void BarberProcess();
        void CustomerProcess();


    /*     void BarberThread();
        void CustomerThread(); */
    /*     std::thread BarberThread();
        std::thread CustomerThread(); */
};

#endif
