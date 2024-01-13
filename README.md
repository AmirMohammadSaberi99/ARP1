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



