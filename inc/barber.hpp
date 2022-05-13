#ifndef BARBER_HPP
#define BARBER_HPP

#include<memory>
#include"customer.hpp"

using std::unique_ptr;


class Barber
{
    int barberID = 0;
    bool isSleeping = false;
    unique_ptr<Customer> pCurrentCustomer = nullptr;

    public:
        // --- constructors ---
        explicit Barber(int);

        // --- getters/setters ---
        int GetID() const;
        void SetID(const int&);
        bool GetIsSleeping() const;
        void SetIsSleeping(bool);
        unique_ptr<Customer>& GetCurrentCustomer();
        void SetCurrentCustomer(unique_ptr<Customer>&);

        // --- other methods ---
        void GoToSleep();
        void WakeUp(const std::string&);
};

#endif