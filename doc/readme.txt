Name:     Crusher md2 Model Viewer

Filename: crush16b.zip
Version:  1.06 beta
Date:     February 9, 1999

Author:   Tom Conder
Email:    blitz@gazpacho.net

Website:  http://www.gazpacho.net/crusher/

CREDITS

	QUAKE 2

	Mike Abrash 
	Trey Harrison <trey@u.washington.edu>
	Jawed Karim <jawmd2@jawed.com>

	id Software <http://www.idsoftware.com/>

	UNREAL

	MaxSMoke <maxsmoke@quiknet.com>
	Dave Townsend <townsend@patriot.net>
	Andrew Scheidecker <andrew@planetunreal.com>
	Tim Sweeney <tim@epicgames.com>
	Chuck Durham <cdurham@xspot.com>

	Legend Entertainment <http://www.legendent.com/>
	Epic Games <http://unreal.epicgames.com/>

	TESTING AND SUGGESTIONS

	Tom Etavard <etavardt@apci.net>
	Shawn Crain <scrain@accessus.net>

	DIRECT X DEVELOPERS MAILING LIST
	(THESE POSTINGS HIT HOME)

	Tony Cox <tonycox@microsoft.com>
	Vinnie Falco <vfalco@mindspring.com>
	Gil Gribb <ggribb@ravensoft.com>
	Raymond Chen



DESCRIPTION

	This is a Quake 2 md2 model viewer.


INSTRUCTIONS

	Use the left and right arrows to step and back through the frames.

	Press and hold the left mouse to rotate the model.

	CTRL+W	- toggles wireframe mode

	C		- recenters the model

	<insert instructions on key remapping>

COMMAND LINE OPTIONS

	The following options are accepted as command line
	options:

	-model filename		loads the model file
	-skin filename		loads the skin file
	-xrot value			rotates about x-axis by value amount
	-yrot value			rotates about y-axis by value amount
	-zrot value			rotates about z-axis by value amount
	-wireframe			sets the viewer to wireframe mode
	-flatshade			set the viewer to flatshading mode
	-h, -help, -?		displays this screen
	-v, -V, -version	displays the program version number

EXPORTING TO UNREAL I FORMAT

** NOTE: To change the Unreal Project folder, modify line 113 in file 'defs.h'
** NOTE: Unreal has problems importing skins that are not powers of two

1. Make a new folder below the Unreal directory, for example 'Unreal\Hazard'

Unreal\
 +- Hazard
 +- Cache
 +- System
 +- ...

2. In this folder, create two news folder 'Classes' and 'Models'

Unreal\Hazard
 +- Classes
 +- Models

3. Use Crusher to export the model to Unreal format. This creates the needed *.3d and
   *.pcx files to the 'Models' folder. Also, a *.uc script is created in the 'Classes'
   folder.

4. Change to the Unreal\System directory. The next three steps will take place there.

5. Locate the Unreal.ini file. In the [Editor.EditorEngine] section add "Hazard" to
   the [EditPackages] list. Eg. EditPackages[8]=Hazard

6. In the Unreal\System directory delete the Hazard.u file if it exists

7. Type "Unreal -make" to build the Hazard.u file.

8. If Unreal completes the build without error, the model is in the game. Otherwise, check
   the Unreal.log file to investigate problems.


TODO

version
?.?
	? port to Linux/GTK
	? port to DirectX
	? compile as ActiveX control for VB

2.1
	add texture painter
	add 16 bit color texture support

2.0
	add palette browser

1.5
	add ability to save screenshot as pcx, bmp, gif, jpg, etc.
	add Half-life support

1.4
	load models from pak files
	add Quake 1 mdl support
	make a default model to be loaded (remove requirement on Quake 2 dir)

1.3
	add ability to show Unreal meshes and textures

1.2
	add depth-buffer; fix overlapping polys-on-model clipping problem
	allow keys to be remapped

1.1
	add frustum clipping and zooming


VERSIONS
	ver  1.06 beta
			added export to Unreal format
			fixed resize bug

	ver  1.05 beta
			fixed flat shading
			added Unreal package support

	ver  1.04 beta
			rewrote core objects as classes
			added key to recenter model
			model can be loaded from any directory
			skin can be loaded from any directory
			disabled textured mode if skin file is invalid
			removed ESC to exit
			added command line arguments
			
	ver  1.03 beta
			wireframe mode no longer shows subdividing triangles
				resulting from SH clip
			reduced demand on CPU 

	ver  1.02 beta
			improved resource management

	ver  1.01 beta
			the escape key exits program
			fixed line in wireframe mode
			fixed flicker on resize

	ver  1.00
			cleaned up the comments
			fixed initial dir
			right mouse button pans the model

	beta 0.05
			saves and loads the last loaded model and texture
			shows skin name in window title
			shows model name in window title
			loads a 'default' texture if first texture is invalid
			added clipping to window
			scales the model when window is resized
			adjusts background when window is resized
			loads first texture correctly

	beta 0.04
			added 256 color palette support
			model is now passed as parameter

	beta 0.03
			loads first texture found on model load
			made model and texture selection more obvious
			fixed loading of initial model and texture
			now actually uses registry settings (duh!)

	beta 0.02
			loads frame names
			loads/saves values in the registry
			added browse button to settings dialog box
			fixed the crakhor/female model problem
			checks choosen texture height and width

	beta 0.01
			fixed key accelerators
			fixed UI to load PCX texture map
			fixed UI to load md2 model
			use StretchBlt to adjust to resize? too slow!


COPYRIGHT/PERMISSIONS

Quake 2 is a registered trademark of id Software, Inc.

Crusher source code is Copyright (C) 1998 Tom Conder
<blitz@gazpacho.net>. All rights reserved

BY USING ANY PORTION OF THIS SOFTWARE, YOU AGREE TO THE FOLLOWING
TERMS AND CONDITIONS:

Tom Conder, "THE AUTHOR", grants you, "THE USER", a non-exclusive,
royalty free, license to use this software in source and binary code
form, provided the user does not utilize the software in a manner
which is disparaging to the author and the user acknowledges the
author in any derivative work.

This software is provided "AS IS," without a warranty of any kind. ALL
EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. THE AUTHOR SHALL NOT
BE LIABLE FOR ANY DAMAGES SUFFERED BY THE USER AS A RESULT OF USING,
MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT
WILL THE AUTHOR BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF THE
AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
