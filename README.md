# Lightsteam Engine

This engine is being developed in C++ for the 3rd year class on *Engine development* at *UPC CITM* bachelor's degree in *Video game Design and development*.

Project's Github: https://github.com/MrIllas/Lightsteam-Engine

### Developed by

Robert Recordà i Illas [(MrIllas)](https://github.com/MrIllas)

## Features

The current version of the engine has the the following features:
	- Load of fbx by drag & drop.
	- Load of image/textures by drag & drop.
	- Custom shader with simple light.
	- Translation, rotation and scaling of objects (without parent-to-child).
	- Scene free camera movement.
	- Transformation, Mesh Renderer and Texture components.
	- Enable/Disable of components

## Controls and features

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

-  Textures:

    - The checkbox "Checkers textures" display a debug texture.
    - The texture component also displays the path from where the texture has been loaded, the size in pixels and the id of the texture.

**Note:** When the selected node doesn't have any loaded texture, the checkers texture is used instead.
**Note:** The "Texture ID" can be the same in more than one node, the engine reuses the texture instead of loading it again if it detects that the path of the texture is the same.


### Drag and Drop functionality

- Supported formats:
	- For 3d object: .fbx.
	- For image/texture: .png, .dds.

 - When draging an external file into the engine, the engine detects if the file extension is supported, in case of trying to load a non-supported extension, it will be notified on the console.
	- If the extension is one of the supported extensions for 3d objects
	- If the extensios is one of the supported extensions for image/textures, the texture is going to load on the selected node (only if the node has a Texture component).	
 - Loading an fbx automatically creates a hierarchy of game objects with a Transform, Mesh Renderer and Texture component.
 - To load a texture into a mesh renderer, select the desired node from the hierarchy.
 