# PicoSystem Demo(s)

Demo project for [Pimoroni Picosystem](http://wiki.picosystem.com/)  

Implements a very simple framework to be able to select which demo should run.  
Contains a few demos which were created to better understand the APIs and the device itself.  

## Demos
When started, list of demos should appear on screen, `UP`/`DOWN` used to select a demo to run.  
`B` is used to run the demo.  
`X` can be used to trigger additional info (FPS & battery percentage).  
To exit from a running demo, press `UP` and `X` simultaneously.  

### Drawer
Showcases etch-a-sketch like environment.  
Use `UP`/`DOWN`/`LEFT`/`RIGHT` to move cursor.  
Hold `B` to draw pixels after cursor.  
Hold `A` to erase pixels.  

### Bounce
Features a pixel that will move in random direction, bouncing off of walls.  

### Geometry
Allows for drawing hollow and filled rectangles and elipses.  
Use `UP`/`DOWN`/`LEFT`/`RIGHT` to move cursor.  
Press `B` to save starting point of next figure.  
Press `B` when ready to draw and shape will be blitted onto the screen.  
Press `X` to see current drawwing mode (first letter - Draw/Erase, second letter Rectangle/FilledRectange/Elipse/FilledElipse).  
Press `A` to change draw/erase mode.  
Press `Y` to change shape.  

### Raycaster
Features simple Wolfenstein3D like raycaster (without textures).  
Map can be ssen in left corner of the screen.  
Use `UP`/`DOWN`/`LEFT`/`RIGHT` to move player.  
