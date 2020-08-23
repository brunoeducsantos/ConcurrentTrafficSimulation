#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

//Traffic light status
enum TrafficLightPhase{
    red=0,
    green=1
};

template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T && stat);

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mut;
};


class TrafficLight: public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();
    // typical behaviour methods
    void simulate();
    void waitForGreen();

private:
    // typical behaviour methods
    TrafficLightPhase _currentPhase ;
    void cycleThroughPhases();
    MessageQueue<TrafficLightPhase> messageQueue_;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif