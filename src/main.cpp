#include<iostream>
#include<thread>
#include"../inc/shop.hpp"


int main()
{
    Barbershop BS; 

    //std::thread barberThread(&Barbershop::BarberProcess, &BS);
/*     std::thread barberThread(&Barbershop::BarberProcess, &BS, std::ref(BS.GetBarbers().back()));
    std::thread customerThread(&Barbershop::CustomerProcess, &BS);

    barberThread.join();
    customerThread.join(); */

/*     std::thread bThread = BS.CreateBarberThread();
    std::thread cThread = BS.CreateCustomerThread();

    bThread.join();
    cThread.join(); */

/*     std::thread bThread = BS.CreateBarberThread();
    std::thread cThread = BS.CreateCustomerThread();
    bThread.join();
    cThread.join(); */

  /*   BS.CreateBarberThread();
    BS.CreateCustomerThread();
    BS.JoinAllThreads(); */

    BS.Run();

    return 0;
}