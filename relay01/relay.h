#ifndef RELAY_H
#define RELAY_H

class Relay
{
public:
    const int relay_pin;
    Relay(const int relay_pin)
        : relay_pin(relay_pin)
    {
        pinMode(relay_pin, OUTPUT);
        turn_off();
    }

    void turn_on()
    {
        digitalWrite(relay_pin, LOW);
        is_turned_on = true; 
    }

    void turn_off()
    {
        digitalWrite(relay_pin, HIGH);
        is_turned_on = false; 
    }

    void toggle()
    {
        if (is_turned_on) {
            turn_off();
            return;
        }

        turn_on();
    }

    bool get_is_turned_on() 
    {
        return is_turned_on;
    }

private:
    bool is_turned_on = false;
};
#endif // RELAY_H
