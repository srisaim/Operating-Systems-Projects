Project 4 Report

Name: Sri Sai M. Valluru
Student ID: 01852662
Username: srisaim

Quick Overview:
Project 4 is designed to simulate a queueing system in a multi-threaded environment, which replicates the process of customer arrival and service in a queue. The main goal of this project is to understand and analyze the behavior of queues under various conditions, like M/M/1 and M/M/n queue models.

My Approach:
First, setting up the overall environment and understanding the project.
Customer Arrival (Thread1), which simulates customer arrivals at random intervals based on the inter-arrival time (lambda).
Server Processing (Thread2), which handles customer service, simulating the time taken to serve a customer (mu).
Queue Observer (Thread3), which observes and records the queue's length at regular intervals for statistical analysis.
For the queue implementation, a FIFO queue was implemented using linked lists, with customers enqueued on arrival and dequeued when served.
For synchronization, mutexes and condition variables were used to manage access to shared resources, like the customer queue, and synchronize thread activities.
For calculating the statistics, the system calculates and outputs statistics like average waiting time, service time, and queue length, along with their standard deviations and server utilization.
Finally, my code also supports command-line arguments to allow dynamic configuration of parameters like lambda, mu, number of customers, and number of servers.

Challenges Faced:
Ensuring correct synchronization between threads to avoid deadlocks and race conditions was challenging, especially in managing the customer queue.
Implementing the logic to correctly handle multiple servers (M/M/n model) and ensure that all servers terminate appropriately after serving the designated number of customers was complex.
Balancing between correctness and efficiency, especially in terms of simulation speed and resource management, was also difficult.
Implementing error handling and input validation for command-line arguments was challenging, particularly in ensuring system stability (λ < μ x numServer).
I could not provide real-time updates on queue length during the simulation, so I displayed the message saying "In the process of executing and calculating, please wait:" while the program was computing.

Things Learned:
I gained insights into the practical application of multi-threading and synchronization in C.
I developed a deeper understanding of queueing theory and its simulation.
Also, I enhanced problem-solving skills, particularly in debugging and optimizing multi-threaded applications.

Conclusion:
Overall, I was able to complete the project to some degree. Inputs like:
./q 
./q -l 5 -m 7 -c 1000 -s 1
or any input for 1 server worked perfectly fine. However I had difficulty trying to revise my code for it to work with multiple servers, 2-5. The input:
./q -l 5 -m 2 -c 1000 -s 4 
ran indefinitely and I wasn't able to resolve the issue completely. Despite the challenges faced, the project successfully demonstrates the core principles of queueing theory in a simulated environment.

Note: Simulation for 1 server approx ran for 1 min and 30 sec (200 seconds).
