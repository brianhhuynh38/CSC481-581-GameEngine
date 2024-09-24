#pragma once

class Thread
{
    bool busy; // a member variable used to indicate thread "status"
    int i; // an identifier
    Thread* other; // a reference to the "other" thread in this example
    std::mutex* _mutex; // the object for mutual exclusion of execution
    std::condition_variable* _condition_variable; // for messaging between threads

public:
    Thread(int i, Thread* other, std::mutex* _mutex, std::condition_variable* _condition_variable);

    bool isBusy();

    void run();

};
