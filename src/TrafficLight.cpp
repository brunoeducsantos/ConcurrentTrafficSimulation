#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono> 

/* Implementation of class "MessageQueue" */


// template <typename T>
// T MessageQueue<T>::receive()
// {
//     // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
//     // to wait for and receive new messages and pull them from the queue using move semantics. 
//     // The received object should then be returned by the receive function. 
// }

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lock(_mut);
    _queue.push_back(std::move(msg)); 
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}
/*
void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}
*/
TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    TrafficObject::threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    auto start = std::chrono::steady_clock::now();
    while (true)
    {
        auto duration = std::chrono::duration_cast< std::chrono::seconds> 
                            (std::chrono::steady_clock::now() - start);
        int duration_traffic_light= rand() % 2 + 4; 
        if(duration.count()== duration_traffic_light) {
            _currentPhase= static_cast<TrafficLightPhase>(rand() % TrafficLightPhase::green);
            start= std::chrono::steady_clock::now();
            messageQueue_.send(std::move(_currentPhase));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
     
}

