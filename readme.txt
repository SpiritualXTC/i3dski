/*
 * Name: Chris Snell      
 * Student ID: s3435406
 *
 */


NOTES:
+ Tested & Compiled, on Fedora 64-bit VM [29/5/14] (@ 5 FPS! lol)
+ Compiler Warning in regarding string constants & char* ?

Note 1:
Awesome Tree Image Generator! 
http://arnaud.ile.nc/cantree/generator.php

Note 2:
Some messy code due to rushing things a little :'(

Note 3: (SP/MP)
+ Single Player & SplitScreen can be changed by setting the 'splitScreen' local variable in the GAME::GAME() constructor

Note 4: Billboard Sorting (Fixing later)
If the camera ends up upside down, sorting will be in the wrong direction <,<
+ Could Stop camera for going upside down
+ Or could multiply the Billboard Distance by the sign, of sin(?) of camera->phi

Note 5: Collada Format / Blender / Open Asset Importer Library (assimp)
+ Collada File for Importing Heirachial 3D Model
+ Only if Time
	+ "Time, Dr. Freeman? Is it really that time again?"

Requirements:

Trees			
	+ Billboarding 						[x]
	+ Collision Detection				[x]
	+ Billboard Sorting (Back-Front)	[x]
		+ Frustrum Distance

Terrain:
	+ Snow Texture						[x]
	+ Infinite Terrain					[x]
	
Multiplayer
	+ Split Screen						[x]
	
Particles
	+ Gravity							[x]
	+ Particle Engine					[x]
		(Meh Implementation)
	+ Don't generate when no contact	[x]
	
Skier
	+ Physics							[x]
	+ Heirarchical Model				[ ]
	
Skybox
	+ Displays							[x]
	+ Moves	with Camera					[x]
	+ Super Awesome HD Texture			[x]

Heirarhcial Model						[ ]
	+ Design in Blender					[ ]
	+ assimp libraries linking			[ ]
	+ Render Mesh in OpenGL Mesh		[ ]