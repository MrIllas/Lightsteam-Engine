# Lightsteam Engine

This engine is being developed in C++ for the 3rd year class on *Engine development* at *UPC CITM* bachelor's degree in *Video game Design and development*.

Project's Github: https://github.com/MrIllas/Lightsteam-Engine

### Developed by

Robert Recordà i Illas [(MrIllas)](https://github.com/MrIllas)

## Features

The current version (0.2) of the engine has the the following features:
	- Complete serialization.
	- Save/Load scenes.
	- Load images and meshes by dragging the file into the inspector.
	- Automatic generation of custom files from imported files.
	- Tons of bug fixing.
	- Frustum Culling for meshes.
	- Scene mouse picking.
	- Viewing of active resources from configuration segment.
	- Double camera.
	- Change active camera from configuration segment.
	- Move meshes with gizmo.


Version 0.1 - Previous version:
	- Load of fbx by drag & drop.
	- Load of image/textures by drag & drop.
	- Custom shader with simple light.
	- Translation, rotation and scaling of objects (without parent-to-child).
	- Scene free camera movement.
	- Transformation, Mesh Renderer and Texture components.
	- Enable/Disable of components

## Controls and features

### Game

 - NEW -> Visualize active camera component.

### Scene

 - NEW -> You can click fbx with the mouse on the scene viewport.

### Scene Camera Controls

The camera controls only work when the mouse is positioned inside the scene window.

 - F -> Only if a node from the hierarchy is selected, teleports the camera on front of the selected game object.

While having the 'Right click" button of the mouse pressed:
 - W -> Displaces the camera frontwards.
 - A -> Displaces the camera to the left.
 - S -> Displaces the camera backwards.
 - D -> Displaces the camera to the right.
 - Q -> Displaces the camera up.
 - E -> Displaces the camera down.
 
 - Holding shift key multiples by two the speed of the camera displacement.
 
 - Alt left/Right -> Only if a node from the hierarchy is selected, orbits the camera around the selected object.


### Hierarchy

 - Mouse Right click -> Opens a pop-up menu that allows to create new nodes (Empty, Spatial and primitives).
 - Left click + drag -> Any node of the hierarchy can be moved to another node.


### Console

 - Clear button -> Clear the list of logs.
 - Log types -> Opens a menu that allows to filter the different log types.
 - Options -> Opens a menu with extra options for the console.
	- Collapse: Collapses all the logs that are exactly the same, adding a count number next to the log.
	- Time: Shows the time when the log has been registered. When collapsed, it shows the time of the last log.
	
	
### Inspector

Shows all the components of the selected node (Game object).
All components and inspector have an 'Active' checkbox that turn off and on the component.

**Note:** The scene node doesn't have any node, not even the transform node, since it doesn't need to be moved.

- Transform: Shows the position, rotation and scale and scale. It can be modified by pressing a field and dragging the mouse to the left or to the right.

- Mesh Renderer: The checkbox "Display normals" draws a line representing every vertex normal of the mesh. It also adds two new options to the component.
	- Checkbox "Face normals" draws the face normals instead of the vertex normals.
	- The Dragbar "Line Magnitude" modifies the length of the lines.
	- The mesh render component also shows the number of vertices, indices and face of the mesh.
	- NEW -> can drag a fbx file to the button the inspector to switch the texture.

-  Textures:

    - The checkbox "Checkers textures" display a debug texture.
    - The texture component also displays the path from where the texture has been loaded, the size in pixels and the id of the texture.
	- NEW -> You can drag a texture file to the button image on the inspector to switch the texture.


### Drag and Drop functionality

 - NEW ->When draging an external file into the engine, the engine adds a copy of the file to the currently open content browser folder and performs a file check to import everything to the library.
 
 
### Content browser

 - NEW -> The square with names are folder that can be opened with a left click.
 - NEW -> The the black square with no names insides are files. You can hold the file to read it's complete name in case it's cutted by the content browser columns.
 - NEW -> Fbx files can be clicked to open and see the meshes inside.
 
