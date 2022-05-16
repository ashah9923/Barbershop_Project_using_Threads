# Barber Shop Project
This project was made using CMake. The aim of this project was to simulate a producer-consumer problem with multithreading. The aims of the project are as follows:
* If there are no customers, the barber falls asleep in the chair
* A customer must wake the barber if he is asleep
* If a customer arrives while the barber is working, if all chairs are occupied, the customer leaves and sits in an empty chair if it's available
* When the barber finishes a haircut, he inspects the waiting room to see if there are any waiting customers and falls asleep if there are none

