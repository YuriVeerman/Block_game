# BlockGame

Making a block game engine
# Block Game

A simple Minecraft-inspired block world written in C using OpenGL and GLFW.

## Features
- 3D block rendering
- Textured blocks
- Basic player movement (WASD, mouse look)
- Multiple block types
- Simple world loading

## Controls
- **W/A/S/D**: Move player
- **Mouse**: Look around
- **Space**: Move down
- **Left Shift**: Move up
- **Escape**: Pause menu

## Build & Run
1. Install dependencies:
	- GLFW
	- OpenGL
	- GLUT (for menu text)
2. Build with `make`:
	```sh
	make
	```
3. Run the game:
	```sh
	./main
	```

## Project Structure
- `src/` — Source code
- `include/` — Header files
- `assets/` — Block definitions, textures, worlds
- `tests/` — Unit tests

## License
MIT

---
Made by Yuri Veerman