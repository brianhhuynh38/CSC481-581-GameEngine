#pragma once

/**
* The build for this class was taken from the example code from Moodle for 
* ThreadExample.cpp. This was extended into a header and cpp file in order to 
* increase readability for both functions and documentation:
* 
* https://moodle-courses2425.wolfware.ncsu.edu/pluginfile.php/499841/mod_resource/content/4/ThreadExample.cpp
* 
*/
class Thread
{
    bool busy; // a member variable used to indicate thread "status"
    int i; // an identifier
    Thread* other; // a reference to the "other" thread in this example
    std::mutex* _mutex; // the object for mutual exclusion of execution
    std::condition_variable* _condition_variable; // for messaging between threads

public:

    /**
    * Constructor for the networking thread for both server and client
    * 
    * @param i: The ID of the thread (0 for server thread, 1 or higher for client threads)
    * @param other: The thread that this one references, this should be the thread that runs the server
    * @param _mutex: The mutex lock for the thread, this should use the mutex from the Timeline
    * @param _condition_variable: The condition variable for the mutex that controls when 
    */
    Thread(int i, Thread* other, std::mutex* _mutex, std::condition_variable* _condition_variable);

    /**
    * Locks the mutex, then sets thread to busy
    */
    bool isBusy();

    /**
    * 
    */
    void run();

};
