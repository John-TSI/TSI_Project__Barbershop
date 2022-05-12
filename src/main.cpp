#include<iostream>
#include<thread>
#include"../inc/shop.hpp"


int main()
{
    Barbershop BS; 

    std::thread barberThread(&Barbershop::BarberProcess, &BS);
    std::thread customerThread(&Barbershop::CustomerProcess, &BS);

    barberThread.join();
    customerThread.join();

 /*    std::thread bThread = BS.BarberThread();
    std::thread cThread = BS.CustomerThread();

    bThread.join();
    cThread.join(); */

    return 0;
}