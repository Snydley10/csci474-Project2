//compile program: g++ project2.cpp -pthread -o project2

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
This is a multi-threaded program that utilizes semaphores to simulate a
handshake procedure between a client and server. The client/server interactions
are simulated by printing each action to the console.
*/

//create new semaphores for client and server
sem_t client_sem, server_sem;

//client function that simulates client-side logic of the handshake procedure with semaphores
void * client(void *)
{
    printf("The client sends a connection signal to the server.\n");
    sem_post(&server_sem); // send connection signal to server thread
    sem_wait(&client_sem); // wait for server thread acknowledgement
    sleep(0.01); //ensures print order is correct for acknowledgement
    printf("The client receives an acknowledgement from the server.\n");
    printf("The client sends a final confirmation to the server.\n");
    sem_post(&server_sem); // signal confirmation to server thread
    return NULL;
}

//server function that simulates server-side logic of the handshake procedure
void * server(void *)
{
    sem_wait(&server_sem); // wait for connection signal from client thread
    printf("The server receives a connection signal from the client.\n");
    sem_post(&client_sem); // acknowledge the client thread
    printf("The server sends an acknowledgement to the client.\n");
    sem_wait(&server_sem); // wait for final confirmation from client thread
    printf("The server receives a final confirmation from the client.\n");
    return NULL;
}

//main function to initialize semaphores and threads and then run client-server handshake
int main()
{
    //semaphore initialization
    sem_init(&client_sem, 0, 0);
    sem_init(&server_sem, 0, 0);
    //create and run threads for client/server handshake
    pthread_t client_thread, server_thread;
    pthread_create(&client_thread, NULL, client, NULL);
    pthread_create(&server_thread, NULL, server, NULL);
    //wait for client and server threads to finish before terminating program
    pthread_join(client_thread, NULL);
    pthread_join(server_thread, NULL);
    //terminate semaphores and threads
    sem_destroy(&client_sem);
    sem_destroy(&server_sem);
    pthread_exit(NULL);
    return 0;
}
