#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include<string>


class Customer
{
    std::string name = "";

    public:
        // --- constructors ---
        Customer();  // may not be needed soon
        explicit Customer(const std::string&);

        // --- getters/setters
        std::string GetName() const;
        void SetName(std::string);
};

#endif