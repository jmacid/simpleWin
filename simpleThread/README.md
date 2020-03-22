# simpleThread

The objetive of this project is to create a thread in Windows as simple as possible.


## How it works

The code creates __MAX_COUNT__ (2) threads and each one runs the _counting_function()_ funtion. This function add a parameter value to a global variable.

In the image the thread view shows the threads that are running:
![alt text][img/threadsOpened.png]


This code is great. However a problem happends. Despite the fact that there is nothing random, the sum is not equal to __MAX_COUNT__ (200000000). 
![alt text][img/raceError.png]

This is because both threads are trying to change the value of the variable _sum_ at the same time. This behavior is called _race_ _condition_.



