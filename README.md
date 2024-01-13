# ARP1
ARP-Assignment1
# Drone Simulation Project

## Overview
This C program provides a comprehensive simulation of a drone's dynamics in a 2D environment, focusing on movement and boundary interactions. It is designed to model physical attributes of a drone and respond to user inputs, making it an excellent tool for understanding basic drone physics and control mechanisms.

## Key Features

### Drone Properties
- **Physical Attributes**: The program defines essential properties of the drone, such as mass and air friction coefficient.
- **Forces**: Specifies the forces acting in the x and y directions to control the drone's movement.

### Motion Dynamics
- **Acceleration Calculation**: The function `integratgeDroneDynamic` calculates the drone's acceleration based on current forces and air friction.
- **Position and Velocity Updates**: This function also updates the drone's position and velocity according to the calculated accelerations.

### Boundary Interaction
- **Collision Handling**: The drone's velocity is adjusted when it reaches predefined spatial boundaries, simulating a collision by reversing its direction.

### User Control
- **User Input Interpretation**: The `controlDroneMotion` function maps user inputs to corresponding directional forces, enabling control over the drone's movement within the simulation.

### Main Loop
- **Communication Channel**: Establishes a shared memory channel for real-time interaction and data exchange.
- **Continuous Position Update**: The main loop of the program continuously updates the drone's position based on user inputs.
- **Signal Handling**: Includes a SIGINT signal handler for graceful termination of the program.

# C Server Application

## Overview
This project is a C-based server application that highlights key aspects of network communication and shared memory management. It is designed to handle client-server interactions and inter-process communication efficiently.

## Features

### Socket Management
- Sets up and manages a network socket.
- Binds the socket to an IP address and listens for client requests.
- Accepts incoming connections and handles client requests.

### Shared Memory Handling
- Establishes and manages a named shared memory segment.
- Facilitates inter-process communication and data sharing.

### Signal Handling
- Implements signal handlers for SIGINT (Ctrl+C) signals.
- Ensures a graceful shutdown process, including the release of resources and closing of network connections.

### Logging
- Includes functions to create log files and record server activity, aiding in monitoring and debugging.

