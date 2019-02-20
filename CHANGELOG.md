# Changelog

## 1.06
- added export to Unreal format
- fixed resize bug

## 1.05
- fixed flat shading
- added Unreal package support

## 1.04
- rewrote core objects as classes
- added key to recenter model
- model can be loaded from any directory
- skin can be loaded from any directory
- disabled textured mode if skin file is invalid
- removed ESC to exit
- added command line arguments

## 1.03
- wireframe mode no longer shows subdividing triangles resulting from SH clip
- reduced demand on CPU 

## 1.02
- improved resource management

## 1.01
- the escape key exits program
- fixed line in wireframe mode
- fixed flicker on resize

## 1.00
- cleaned up the comments
- fixed initial dir
- right mouse button pans the model

## 0.05
- saves and loads the last loaded model and texture
- shows skin name in window title
- shows model name in window title
- loads a 'default' texture if first texture is invalid
- added clipping to window
- scales the model when window is resized
- adjusts background when window is resized
- loads first texture correctly

## 0.04
- added 256 color palette support
- model is now passed as parameter

## 0.03
- loads first texture found on model load
- made model and texture selection more obvious
- fixed loading of initial model and texture
- now actually uses registry settings (duh!)

## 0.02
- loads frame names
- loads/saves values in the registry
- added browse button to settings dialog box
- fixed the crakhor/female model problem
- checks choosen texture height and width

## 0.01
- fixed key accelerators
- fixed UI to load PCX texture map
- fixed UI to load md2 model
- use StretchBlt to adjust to resize? too slow!
