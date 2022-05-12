#ifndef BARBER_HPP
#define BARBER_HPP


class Barber
{
    // --- attributes ---
    bool isSleeping = false;

    public:
        bool GetIsSleeping() const;
        void SetIsSleeping(bool);
};

#endif