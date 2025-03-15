# Snake Game

This project is a classic Snake Game implemented using the [Enviro framework](https://github.com/klavinslab/enviro) by the Klavins Lab. The game features a snake that moves around a bounded map, eats food to increase its score, and ends when the timer runs out. The snake is controlled using keyboard inputs, and the game includes physics-based movement and collision detection.

## Features
- **Snake Movement**: The snake head is controlled with `W` (forward), `S` (backward), `A` (left), and `D` (right). Body segments follow the head automatically.
- **Food Collection**: Food items spawn randomly on the map. Eating food increases the score and teleports the food to a new location.
- **Game Timer**: The game lasts 50 seconds, after which it ends with a "Game Over" message displaying the final score.
- **Boundaries**: The snake is confined within a rectangular map (1200x900 units).
- **Score Display**: Real-time updates of time remaining and score are shown above the snake head.

## Project Structure
- **Configuration Files**:
  - `config.json`: Defines the game world, including the snake, food, and boundaries.
  - `food.json`: Specifies the properties of food objects (static, square-shaped).
  - `snake_segment.json`: Defines snake segments (dynamic, with sensors for the head).
  - `boundary.json`: Describes static boundary objects.

- **Source Files**:
  - `food.cc` / `food.h`: Implements food behavior, including collision detection and teleportation when eaten.
  - `snake_segment.cc` / `snake_segment.h`: Controls snake movement, input handling, scoring, and game logic.

## Prerequisites
- [Enviro](https://github.com/klavinslab/enviro) installed and configured.
- A C++ compiler (e.g., `g++`) and `make` for building the project.
- Basic understanding of JSON and C++.

## Installation
1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
2. Ensure Enviro is installed and added to your project path
3. Make
  ```bash
  esm start
  make
  enviro