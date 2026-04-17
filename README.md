# 🌀 Labyrinthine

Labyrinthine is a challenging maze-navigation game built with **C++** and the **Raylib** library. Players control a glowing ball through procedurally generated mazes filled with obstacles, mechanics, and layered difficulty.

With **35+ progressively harder levels**, the game introduces new systems such as timers, move limits, teleporters, and fog-of-war to keep gameplay fresh and engaging.

---

## ✨ Features

### 🧩 Procedural Maze Generation
Every level is uniquely generated using a **recursive backtracking algorithm**, ensuring no two runs feel the same.

### 📈 Progressive Difficulty
Over **35 handcrafted levels** that gradually combine mechanics and increase complexity.

### 🎮 Game Modes
- ⏱ **Timed Mode** – Reach the goal before time runs out  
- 🔢 **Limited Moves Mode** – Solve the maze within a fixed number of moves  
- 🌀 **Teleporter Mode** – Use teleporters to navigate complex layouts  
- 🌫 **Fog of War** – Limited visibility reveals only nearby areas  

### 💾 Save & Continue
Progress is automatically saved so players can continue from where they left off.

### 🎯 Intuitive Controls
Supports both keyboard and mouse input for smooth gameplay.

### 🎨 Clean Visual Style
Minimalist glowing design with smooth transitions and animations.

---

## 🎮 Gameplay

Your objective is simple:

> Guide the glowing ball from the **start (top-left)** to the **goal (bottom-right)**.

Movement is restricted to intersections where you choose between available paths.

As levels progress, mechanics are layered together, requiring more strategic thinking and planning.

---

## 🕹 Controls

### Keyboard
- `W` / `↑` – Move Up  
- `A` / `←` – Move Left  
- `S` / `↓` – Move Down  
- `D` / `→` – Move Right  

### Mouse
- Click directional nodes at intersections to move the ball

### Menu
- `ESC` – Return to main menu

---

## 🛠 Building from Source

### Requirements
- Microsoft Visual Studio 2019 or later  
- C++ Desktop Development workload  
- Raylib (included and preconfigured in the project)

---

### Build Steps

1.  Clone the repository to your local machine:
    ```bash
    git clone https://github.com/hhchandar20/Labyrinthine.git
    ```
2.  Navigate to the `Labyrinthine` sub-directory.
3.  Open the `Labyrinthine.sln` solution file in Visual Studio.
4.  In the Solution Explorer, right-click on the `Labyrinthine.PL` project and select "Set as StartUp Project".
5.  Build and run the solution (press `F5` or `Ctrl+F5`).

---

## 🏗 Architecture

The project follows a clean **3-layer architecture** to separate concerns:

### 📦 Labyrinthine.DAL (Data Access Layer)
Responsible for core data structures and persistence:
- `Ball`
- `Cell`
- `Teleport`
- `SaveManager`

### 🧠 Labyrinthine.BLL (Business Logic Layer)
Handles game logic and core systems:
- `MazeGenerator` (procedural maze creation)
- `BallService` (movement, rules, interactions)

### 🖼 Labyrinthine.PL (Presentation Layer)
Handles everything related to the game experience:
- Rendering with Raylib
- Input handling
- Game loop
- UI and state management

---

## ⭐ Support

If you like this project, consider giving it a star ⭐ to support development!
