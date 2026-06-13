# State of Code

This document is a quick guide to the structure of the project, the role of each source file, and the flow of the application.

## 1. Project overview

This is a C++ OpenGL/GLUT computer graphics project that renders a 3D world using model text files stored in the Model/ folder. The main runtime loop is defined in CNAmain.cpp, and the actual world composition is assembled in CNAWorld.cpp.

The project is organized into:
- Main OpenGL entry and input handling
- World composition and scene setup
- Character/environment model loaders
- Texture loading helper
- Asset model text files under Model/

---

## 2. File map

### Entry and runtime control

1. CNAmain.cpp
   - Main program entry point.
   - Initializes GLUT, creates the window, sets OpenGL state, and starts glutMainLoop().
   - Contains user input callbacks:
     - keyboard input
     - arrow key / function key input
     - mouse drag handling
   - Calls myInit(), myViewingInit(), myLightingInit(), and myvirtualworld.init().
   - Also prints a welcome/help message to the console.

2. CNAmain.hpp
   - Header for the runtime system.
   - Declares callback functions and utility structures:
     - MyWindow
     - MyWorld
     - MyViewer
     - MySetting
     - MyAxis
   - Holds default camera, movement, and axis-drawing logic used by the main program.

### World composition

3. CNAWorld.cpp
   - Main scene assembly file.
   - Loads all model assets for:
     - Kinger
     - Gloinks
     - Environment
   - Loads texture images for Kinger pieces.
   - Defines MyVirtualWorld::init(), MyVirtualWorld::draw(), and MyVirtualWorld::tickTime().
   - Current draw flow renders the environment and the Gloinks world objects.

4. CNAWorld.hpp
   - Declares the main world controller class MyVirtualWorld.
   - Holds the three major scene components:
     - ProjectKinger::Kinger kinger
     - ProjectGloinks::Gloinks gloinks
     - ProjectEnvironment::Environment environment
   - Serves as the bridge between the rendering loop and the object-specific model classes.

### Character and object model systems

5. Kinger.hpp
   - Declares the Kinger character model system.
   - Contains 10 model parts such as:
     - head
     - head piece
     - left/right eye
     - body
     - cloth
     - left hand
     - right hand with gun
     - bucket
     - bucket handle
   - Also declares texture IDs for selected parts.

6. Kinger.cpp
   - Implements the Kinger loader and renderer.
   - Each loadXXX() function loads one OBJ-style text model from Model/Kinger/.
   - Each drawXXX() function renders one part with OpenGL transforms, culling, and texture handling.
   - The main draw() function renders the full character composition.

7. Gloinks.hpp
   - Declares the Gloinks object system.
   - Contains six model components:
     - BowlingPin
     - Circle
     - Cube
     - Moon
     - Star
     - Triangular

8. Gloinks.cpp
   - Implements the Gloinks loaders and drawing functions.
   - Each object is loaded from Model/Gloinks/ and drawn individually.
   - The main draw() function renders all Gloinks assets together.

9. Environment.hpp
   - Declares the Environment scene system.
   - Contains nine environment model parts:
     - SkyBox
     - Ground
     - CastleWall
     - Cube
     - CubeGrouped
     - IrregularCube
     - Pillar
     - Roof
     - Sphere

10. Environment.cpp
   - Implements the environment loader and renderer.
   - Each loadXXX() function calls ObjModel::loadFromObjText().
   - Each drawXXX() function applies translations, scaling, and color settings before drawing.
   - The main draw() function renders the whole environment scene.

### Generic model loader

11. ObjModel.hpp
   - Declares the reusable OBJ-style model loader.
   - Contains data structures:
     - Vec3 for vertices and normals
     - Vec2 for texture coordinates
     - FaceIndex for face references
     - TriangleFace for triangle faces
   - Declares the class ObjModel with:
     - clear()
     - loadFromObjText()
     - draw()

12. ObjModel.cpp
   - Implements the OBJ text parser.
   - Reads plain text model files in OBJ-like format.
   - Parses:
     - v = vertex
     - vn = normal
     - vt = texture coordinate
     - f = triangle face
   - Stores all data in vectors and renders it using OpenGL triangle primitives.

### Texture helper

13. TextureLoader.hpp
   - Declares a simple static texture loader interface.
   - Uses OpenGL GLuint and std::string.

14. TextureLoader.cpp
   - Loads image files into OpenGL textures.
   - Uses stb_image.h to decode PNG/JPG textures.
   - Returns a texture ID that can be bound during rendering.

### Placeholder / unfinished files

15. Caine.cpp
   - Currently empty / placeholder.

16. Caine.hpp
   - Currently empty / placeholder.

These files appear to be reserved for future use and are not currently contributing to the active scene.

---

## 3. Runtime flow

1. main() in CNAmain.cpp starts the program.
2. myInit() creates the GLUT window and sets up OpenGL defaults.
3. myvirtualworld.init() in CNAWorld.cpp loads all model files and textures.
4. The GLUT display callback calls myDisplayFunc().
5. myDisplayFunc() renders the environment and object groups.
6. Keyboard/mouse callbacks allow the user to move and rotate the world.

---

## 4. Asset folders

### Model/Environment
Contains environment meshes such as:
- CastleWall.txt
- Cube.txt
- CubeGrouped.txt
- Ground.txt
- IrregularCube.txt
- Pillar.txt
- Roof.txt
- Sphere.txt
- Square SkyBox.txt

### Model/Gloinks
Contains the Gloinks object models.

### Model/Kinger
Contains the Kinger character models and texture-related assets.

---

## 5. Important notes

- The project is built around OBJ-style text model files rather than standard .obj binary files.
- The model loader is generic and reusable across characters and environment objects.
- Kinger rendering is partially set up with texture IDs, but the active scene currently emphasizes environment and Gloinks rendering.
- Caine appears unfinished and is not currently wired into the main scene.
- The project depends on OpenGL/GLUT and the stb_image texture loader.

---

## 6. Suggested reading order

If you want to understand the project quickly, read these files in this order:
1. CNAmain.cpp
2. CNAWorld.cpp
3. CNAWorld.hpp
4. ObjModel.cpp
5. Environment.cpp
6. Kinger.cpp
7. Gloinks.cpp
8. TextureLoader.cpp

This order gives the clearest picture of how the project starts, loads assets, and renders the scene.
