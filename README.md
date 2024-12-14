# Sobrassada Engine

This is a simple engine that allows the visualization of GLTF models alongside its texture. 

It also has a simple UI with information and configuration of the several modules that compose the engine.

Link to the Git repository: https://github.com/IDarkgenesis/BasicEngine_Master

Author: Bartomeu Perelló Comas

## Usage

### Camera
There are several ways of moving and rotating the camera.

Camera movement:
- **Hold Right Click** + **WASD**, allows the camera to move in its front and right axis.
- **Hold Right Click** + **QE**, allows the control of the camera elevation.
- **Hold Right Click** and move the **mouse scroll wheel**, to zoom in and out.
- **Hold SHIFT** while moving to double the movement speed of the camera.

Camera rotation:
- **Hold Right Click** and **move the mouse** to rotate the camera.
- Use of **UP**, **RIGHT**, **LEFT** and **DOWN** keys to rotate the camera.
- **Hold Left ALT** and **move the mouse** to make the camera orbit.

Camera focus:
- Press the **F** key to make the camera focus the current loaded geometry.

### Geometry and texture loading

Different GLTF files can be loaded to the viewer by drag and dropping the .gltf file to the viewport. When a .gltf is loaded, the previous geometry is removed, so only one model can be loaded at a time.

For textures, it can load any .ppm, .png and .jpg file, if any of these files is dropped it will be loaded to the model and will be selected for rendering.

### Additional features

- Added the capability to select the texture to use for rendering of all the loaded ones. Once a new model is loaded, previous loaded textures are removed.
