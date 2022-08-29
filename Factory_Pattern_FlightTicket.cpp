#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
/**
 * @brief define enum
 * 
 */
enum Seat {Economy, Premium, Business};
enum Airline {Delta, United, SouthWest};
/**
 * @brief define key-value map
 * 
 */
unordered_map<string, Airline> airlines {
    {"Delta", Delta},
    {"United", United},
    {"SouthWest", SouthWest}
};
unordered_map<string, Seat> seats {
    {"Economy", Economy},
    {"Premium", Premium},
    {"Business", Business}
};
/**
 * @brief ticket structure
 * 
 */
struct Ticket {
    Airline airline;
    Seat seat;
    float distance;
};

/**
 * @brief base class design
 * 
 */
class AirlineCalculator {
public:
    // Factory pattern
    static AirlineCalculator* create(Airline airline);
    // calculate total cost
    virtual float calculate(const Ticket& ticket) const = 0;
    virtual ~AirlineCalculator() = default;
protected:
    virtual float getOpCost(Ticket ticket) const {
        float opCost = 0;
        switch (ticket.seat) {
            case (Economy):
                opCost = getEconomyOpCost(ticket.distance);
                break;
            case (Premium):
                opCost = getPremiumOpCost(ticket.distance);
                break;
            case (Business):
                opCost = getBusinessOpCost(ticket.distance);
                break;
        }
        return opCost;
    }
    virtual float getEconomyOpCost(float distance) const {
        return 0.0f;
    }
    virtual float getPremiumOpCost(float distance) const {
        return 25.0f;
    }
    virtual float getBusinessOpCost(float distance) const {
        return 50.0f + 0.25f * distance;
    }
};

class DeltaCalculator:public AirlineCalculator {
public:
    float calculate(const Ticket& ticket) const override{
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.5f;
    }
    // Meyer's Singleton
    static AirlineCalculator* getInstance() {
        static DeltaCalculator delta;
        return &delta;
    }
    virtual ~DeltaCalculator() = default;
private:
    DeltaCalculator() = default;    
};

class UnitedCalculator:public AirlineCalculator {
public:
    float calculate(const Ticket& ticket) const override {
        float opCost = getOpCost(ticket);
        return opCost + ticket.distance * 0.75f;
    }
    static AirlineCalculator* getInstance() {
        static UnitedCalculator united;
        return &united;
    }
    virtual ~UnitedCalculator() = default;
private:
    UnitedCalculator() = default;
protected:
    float getPremiumOpCost(float distance) const override {
        return 25.0f + distance * 0.1f;
    }
};

class SouthWestCalculator:public AirlineCalculator {
public:
    float calculate(const Ticket& ticket) const override {
        return ticket.distance * 1.0f;
    }
    static AirlineCalculator* getInstance() {
        static SouthWestCalculator southwest;
        return &southwest;
    }
    virtual ~SouthWestCalculator() = default;
private:
    SouthWestCalculator() = default;
};
// Factory pattern
AirlineCalculator* AirlineCalculator::create(Airline airline) {
    // Singleton pattern
    if (airline == Delta) return DeltaCalculator::getInstance();
    else if (airline == United) return UnitedCalculator::getInstance();
    else if (airline == SouthWest) return SouthWestCalculator::getInstance();
    else return nullptr;
}

static Ticket parse_ticket(const string& s) {
    // split by space
    vector<string> arr;
    stringstream ss(s);
    string token;
    while (getline(ss, token, ' ')) {
        arr.push_back(token);
    }
    assert(arr.size() == 3);
    assert(airlines.count(arr[0]));
    assert(seats.count(arr[2]));
    Ticket ticket;
    ticket.airline = airlines[arr[0]];
    ticket.distance = stof(arr[1]);
    ticket.seat = seats[arr[2]];
    return ticket;
}

// wrapper
vector<float> process_tickets(vector<string>& tickets) {
    vector<float> costs;
    for (auto& ticket : tickets) {
        Ticket t = parse_ticket(ticket);
        AirlineCalculator* flight = AirlineCalculator::create(t.airline);
        costs.push_back(flight->calculate(t));
    }
    return costs;
}

int main() {
    vector<string> input{"United 150.0 Premium", "United 120.0 Economy",
    "United 100.0 Business","Delta 60.0 Economy","Delta 60.0 Premium",
    "Delta 60.0 Business", "SouthWest 1000.0 Economy", 
    "SouthWest 4000.0 Economy"};
    vector<float> costs = process_tickets(input);
    for (int i=0; i<input.size(); i++) {
        cout << input[i] << " cost: $ " << costs[i] << endl;
    }
}
