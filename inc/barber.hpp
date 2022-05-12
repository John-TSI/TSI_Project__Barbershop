#ifndef BARBER_HPP
#define BARBER_HPP


class Barber
{
    bool isSleeping = false;

    public:
        // --- getters/setters ---
        bool GetIsSleeping() const;
        void SetIsSleeping(bool);

        // --- other methods ---
        void GoToSleep();
        void WakeUp();
};

#endif