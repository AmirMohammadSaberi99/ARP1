#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>
#include "../headers/User_interface.h"
#include <math.h>
#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"

void handleSIGINT(int signal);

// Drone properties
double droneMass = 1.0;              // Mass of the drone (in kilograms)
double airFrictionCoefficient = 0.1; // Coefficient of air friction
double timeInterval = 0.1;           // Simulation time step (in seconds)
double forceX = 0;                   // Force in the x direction
double forceY = 0;                   // Force in the y direction

// Initial state
double initialXPosition = 8.0;   // Initial position in the x direction (in meters)
double initialYPosition = 22.0;  // Initial position in the y direction (in meters)
double initialXVelocity = 0.0;   // Initial velocity in the x direction (in meters per second)
double initialYVelocity = 0.001; // Initial velocity in the y direction (in meters per second)

// Function to update the drone's position and velocity based on forces and constraints
void integratgeDroneDynamic()
{
    // Calculate accelerations
    double xAcceleration = (forceX - airFrictionCoefficient * initialXVelocity) / droneMass;
    double yAcceleration = (forceY - airFrictionCoefficient * initialYVelocity) / droneMass;

    // Update velocities using accelerations and time interval
    initialXVelocity += xAcceleration * timeInterval;
    initialYVelocity += yAcceleration * timeInterval;

    // Update positions using velocities, accelerations, and time interval
    initialXPosition += initialXVelocity * timeInterval + 0.5 * xAcceleration * pow(timeInterval, 2);
    initialYPosition += initialYVelocity * timeInterval + 0.5 * yAcceleration * pow(timeInterval, 2);

    // Check and handle boundary conditions
    if ((initialXPosition < DRONE_X_LOWER_BOUND) || (initialXPosition > DRONE_X_UPPER_BOUND))
    {

        initialXVelocity = -initialXVelocity;

    }
    if (initialYPosition < DRONE_Y_LOWER_BOUND || initialYPosition > DRONE_Y_UPPER_BOUND)
    {

          initialYVelocity = -initialYVelocity; // Reverse the velocity upon hitting X boundary
}
}
// Function to control the movement of the drone based on user input
void controlDroneMotion(CartesianPoint *currentDronePosition, char movementCommand)
{
    // Adjust forces based on user input
    switch (movementCommand)
    {
    case 'e':
        // Move left
        forceX--;
        break;
    case 'c':
        // Move right
        forceX++;
        break;
    case 's':
        // Move down
        forceY--;
        break;
    case 'f':
        // Move up
        forceY++;
        break;
    case 'r':
        // Move diagonally up-right
        forceX += -sqrt(2) / 2;
        forceY += sqrt(2) / 2;
        break;
    case 'x':
        // Move diagonally down-right
        forceX += sqrt(2) / 2;
        forceY += -sqrt(2) / 2;
        break;
    case 'v':
        // Move diagonally up-left
        forceX += sqrt(2) / 2;
        forceY += sqrt(2) / 2;
        break;
    case 'w':
        // Move diagonally down-left
        forceX += -sqrt(2) / 2;
        forceY += -sqrt(2) / 2;
        break;
    case 'd':
        // Stop all motion
        forceX = 0;
        forceY = 0;
        initialXVelocity = 0;
        initialYVelocity = 0;
        // Log the drone's process information
        logProcessInfo("aerial_drone.txt", "w+");
        break;
    }

    // Solve the dynamic equation by the Taylor series expansion
    integratgeDroneDynamic();

    // Update location
    currentDronePosition->xCoordinate = initialXPosition;
    currentDronePosition->yCoordinate = initialYPosition;
}

int main()
{
    /*Establishing a communication channel through shared memory to facilitate real-time interaction and data exchange between processes.*/
    LogFile_creation("./drone.txt", "w+");
    // Log initial process information
    logProcessInfo("drone.txt", "w+");

    // Handle SIGINT signal with cleanupAndTerminate function
    signal(SIGINT, handleSIGINT);

    // start a shared memory instance
    communicationChannel_fd = shm_open("/User_interface", O_RDWR, 0666);
    userInterfaceBuffer = (UserInterface *)mmap(NULL, sizeof(UserInterface), PROT_READ | PROT_WRITE, MAP_SHARED, communicationChannel_fd, 0);

    for (;;)
    {
        // Adjust the drone's position according to user input
        char ch = userInterfaceBuffer->lastKeyPressed;
        controlDroneMotion(&(userInterfaceBuffer->dronePosition), ch);
        usleep(100000); // Adjust usleep duration for desired update rate (e.g., 10 milliseconds)
    }

    munmap(userInterfaceBuffer, sizeof(UserInterface));
    close(communicationChannel_fd);
    return 0;
}

// /*
// Signal handler for handling SIGINT (Ctrl+C)
// */
// void handleSIGINT(int signal) {
//     if (signal == SIGINT) {
//         printf("Received SIGINT Signal! Cleaning up and terminating.\n");

//         /* Clean up shared resources */
//         munmap(userInterfaceBuffer, sizeof(UserInterface));
//         close(communicationChannel_fd);

//         /* Terminate the process */
//         kill(getpid(), SIGKILL);
//     }
// }

/*
Function to perform cleanup and terminate the process
*/
void cleanupAndTerminate()
{
    printf("Cleaning up and terminating.\n");

    /* Clean up shared resources */
    munmap(userInterfaceBuffer, sizeof(UserInterface));
    close(communicationChannel_fd);

    /* Terminate the process */
    exit(EXIT_SUCCESS);
}

/*
Signal handler for handling SIGINT (Ctrl+C)
*/
void handleSIGINT(int signal)
{
    if (signal == SIGINT)
    {
        printf("Received SIGINT Signal! ");
        cleanupAndTerminate();
    }
}
