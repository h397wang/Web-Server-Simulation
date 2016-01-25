# Web-Server-Simulation
ECE 150 Project

Server's database is a linked list of URIs, each of which has a name, (positive size), and non-negative priority
Client requests are provided as a linked list as well, each request also has a timestamp associated with it.

How the server works
A client request arrives and is queued instantly. Once queued, the client-request is then ready to be processed by the server. Pending requests are processed based on the priority of the URI being requested. There is a transmission delay in the processing of requests, this time is dependent on the size of the URI.

The objective of this project is to implement a function that returns the maximum delay incurred by a client-request from the time it arrives till the time it is processed by the server. 

The design of the function implements a discreet event simulation of the server. Only one of three events would happen at any given moment, time progresses, requests are queued, and a pending  request is serviced.

Will upload pdf of project assingment if found. 
