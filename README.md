# OpenGL Asteroids Game

This is a simple 2D **Asteroids Shooter Game** built with **OpenGL** and **GLUT** in C++. You control a spaceship that rotates and shoots bullets to destroy asteroids. The game progresses through increasing levels of difficulty and ends when you either complete all levels or run out of bullets.

## 🚀 Features

- Multiple Levels (up to 10)
- Rotating spaceship shooter
- Moving asteroids with screen wrapping
- Bullet-asteroid collision detection
- Score tracking and level display
- Game Over screen for:
  - Bullet exhaustion
  - Level completion
- Optional grid background toggle

## 🎮 Controls

| Key          | Action                        |
|--------------|-------------------------------|
| `Left Arrow` | Rotate ship left              |
| `Right Arrow`| Rotate ship right             |
| `Space`      | Shoot bullet                  |
| `Esc`        | Exit game                     |

## 🧱 Dependencies

- C++ Compiler (GCC/Clang/MSVC)
- OpenGL Utility Toolkit (GLUT)
  - Linux: `freeglut`
  - Windows: `freeglut` or `GLUT32.lib`
  - macOS: `GLUT.framework`

## 🔧 Compilation Instructions

### Linux / macOS

```bash
g++ -o asteroid_game main.cpp -lGL -lGLU -lglut
./asteroid_game
```

### Windows (MinGW)

Make sure you have the required GLUT and OpenGL libraries.

```bash
g++ -o asteroid_game.exe main.cpp -lopengl32 -lglu32 -lfreeglut
.\asteroid_game.exe
```

## 📁 File Structure

```
.
├── main.cpp        # Main game source code
└── README.md       # This file
```

## 🧠 Game Logic Overview

- **Asteroid Class**: Holds position, color, and velocity. Spawns with random values.
- **Bullet Class**: Tracks direction and position of fired bullets.
- **Collision Detection**: Removes asteroids and bullets on collision.
- **Leveling System**: Increases asteroid count each level.
- **Grid Mode**: Toggle with right-click menu to change background.

## 🛠️ To-Do

- Add sound effects and background music
- Add mouse support
- Power-ups or extra lives
- Scoreboard or high score save

