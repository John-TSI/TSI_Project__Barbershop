#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include<string>


static std::string Names[8] = {"Alf", "Bob", "Carl", "Dave", "Elvis", "Fred", "Gary", "Harold"};

class Customer
{
    std::string name = "";

    public:
        // --- constructors ---
        Customer();

        // --- getters/setters
        std::string GetName() const;
        void SetName(const std::string&);
};

#endif