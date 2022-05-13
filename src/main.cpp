#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <condition_variable>

std::mutex dataMutex;                   // Used to state if we are using a resourse (used with lock so we can lock access to a resource)
std::condition_variable myConditionVar; // Can wait and notify
std::queue<std::string> customerQueue;  // created queue that holds names for the customers
bool isBarberShopBusy = false;          // this bool will be flipped when a customer is created
bool shopIsOpen;                        // this bool was used to break the while loop within the barberthread
std::vector<std::string> customersnames = {"Vince","Hershey","Hunter","Egor","Freckles"
                                      "Danni","Apollo","Napoleon","Ginny","Gucci",
                                      "Fudge", "Cupcake", "Butternut", "Amore", "Nellie", 
                                      "Precious", "Vanilla", "Noisette", "Paisley", "Brie"};
int currentCustomer = 0;                // keeps check of which customer name is being used in the customer func

//PRODUCER
/**
 * @brief 
 * An instance of a customer thread that adds a customer name to the customerQueue and notifies barber when added
 */
void customer()
{
    srand(time(NULL));
    std::unique_lock<std::mutex> MyLock(dataMutex); // Create a lock so we can lock editing the data

    customerQueue.push(customersnames[currentCustomer % customersnames.size()]);            // Update the data - puts customername in customer queue
    currentCustomer++;
    std::cout << customerQueue.back() << " Added to the queue" << std::endl;

    isBarberShopBusy = true;                        // We have done the update to data.

    myConditionVar.notify_one();            // Notify any thread using the condition var to wait() that we are now ready.
}

//CONSUMER
/**
 * @brief 
 * An instance of barber thread that waits for a customer to be in the queue and then removes that customer at the end
 * 
 */
void barber()
{
    while(shopIsOpen || customerQueue.size() != 0)
    {
        std::unique_lock<std::mutex> MyLock(dataMutex); // Create a lock so we can lock editing the data
        std::cout << "\nBarber looks into waiting room" << std::endl;

        // If producer hasn't notified us it's complete
        if (!isBarberShopBusy)
        {
            std::cout << "There are no customers! Barber Sleeps. " << std::endl;
            myConditionVar.wait(MyLock);    // Will continue when notified. temoraritly drops lock till notified where it then picks it up
            std::cout << customerQueue.front() << " wakes the barber for a cut " << std::endl;
        }
        if(customerQueue.size() != 0)
        {
            std::cout << customerQueue.front() << " goes in for a trim " << std::endl;
            //unlock thread to let customers be created whilst barber cuts hair
            MyLock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 10 + 5));
            //lock thread again to finish carrying out barber thread
            MyLock.lock();
            std::cout << "Barber has cut hair of: " << customerQueue.front() <<  std::endl;
            customerQueue.pop();    // removes customer from queue
            if(customerQueue.size() == 0)   // (1 equals sets, 2 equals checks) condition checks if there is noone in the queue
            {
                isBarberShopBusy = false;
            }
        }
    }
}


int main()
{
    system("cls");
    fputs("\x1b[H\x1b[2J\x1b[3J", stdout);
    fflush(stdout);
    srand(time(NULL));
    
    shopIsOpen = true;
    // Creat a thread for the Producer and consumer
    std::thread barberThread(barber);
    //set a counter to 0
    int count = 0;
    //loop that creates 10 customers
    while(count < 10)
    {
        //waits between 3 and 10 secs to run the customerThread
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 8 + 3));
        std::thread customerThread(customer);
        customerThread.detach();    // lets the customer thread run in the background whilst main and barber continue
        count++;
    }
    shopIsOpen = false;     //sets boolean to false which breaks the while loop condition in barberThread

    barberThread.join();  // 
    std::cout << "\nBarber closes up shop." << std::endl;

    return 0;
}
