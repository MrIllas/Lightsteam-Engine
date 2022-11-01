# Lightsteam Engine


https://github.com/MrIllas/Lightsteam-Engine

## Controls

### Hierarchy

Only inside the window hierarchy.

 - Mouse Right click -> Opens a pop-up menu that allows to create new nodes (Empty, Spatial and primitives).
 - Left click + drag -> Any node of the hierarchy can be moved to another node.

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

### Console

 - Clear button -> Clear the list of logs.
 - Log types -> Opens a menu that allows to filter the different log types.
 - Options -> Opens a menu with extra options for the console.
	- Collapse: Collapses all the logs that are exactly the same, adding a count number next to the log.
	- Time: Shows the time when the log has been registered. When collapsed, it shows the time of the last log.
	
### Inspector

Shows all the components of the selected node (Game object).

All components and inspector have an 'Active' checkbox that turn off and on the component.

Note: The scene node doesn't have any node, not even the transform node, since it doesn't need to be moved.

#### Transform

Shows the position, rotation and scale and scale. It can be modified by pressing a field and dragging the mouse to the left or to the right.

#### Mesh Renderer

The checkbox "Display normals" draws a line representing every vertex normal of the mesh. It also adds two new options to the component.
	- Checkbox "Face normals" draws the face normals instead of the vertex normals.
	- The Dragbar "Line Magnitude" modifies the length of the lines.
	
The mesh render component also shows the number of vertices, indices and face of the mesh.

#### Textures

The checkbox "Checkers textures" display a debug texture.

The texture component also displays the path from where the texture has been loaded, the size in pixels and the id of the texture.

Note: When the selected node doesn't have any loaded texture, the checkers texture is used instead.
Note: The "Texture ID" can be the same in more than one node, the engine reuses the texture instead of loading it again if it detects that the path of the texture is the same.


### Drag and Drop functionality

When draging an external file into the engine, the engine detects if the file extension is supported.