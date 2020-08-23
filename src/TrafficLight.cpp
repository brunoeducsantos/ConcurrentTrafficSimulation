#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <chrono> 

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mut);
    _condition.wait(lck, [this] { return !_queue.empty(); });
    T msg = std::move(_queue.back());
    _queue.pop_back();
    return msg; 
}

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

void TrafficLight::waitForGreen()
{
    
    while(true){
        if(messageQueue_.receive()==TrafficLightPhase::green) return;
    }

}

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
            if(_currentPhase==TrafficLightPhase::red){
                _currentPhase=TrafficLightPhase::green;
            }
            else{
                _currentPhase=TrafficLightPhase::red;
            }
            start= std::chrono::steady_clock::now();
            messageQueue_.send(std::move(_currentPhase));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
     
}

