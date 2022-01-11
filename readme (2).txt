//==============================================//
//  H   H  OOO  RRRR   III  ZZZZZ  OOO  N    N  //
//  H   H O   O R   R   I      Z  O   O NN   N  //
//  HHHHH O   O RRRR    I     Z   O   O N N  N  //
//  H   H O   O R  R    I    Z    O   O N   NN  //
//  H   H  OOO  R   R  III  ZZZZZ  OOO  N    N  //
//==============================================//

Project documentation

//Table of Contents
//1. Description
//1.1 Future goals
//2. How to compile
//2.1 Required software
//2.2 Folder structure
//3. How to use

//Section 1. Description

	Horizon is a game and a game engine created from scratch. It is meant to combine the best elements from various genre of games, by providing a wide range of technical possibilities that are usually too difficult for modern game engines.

	Horizon is a first-person game. It is an open-world game. It may be a sandbox game. It may be a space simulator. It may include crafting. It is polygon-based and not voxel-based.

//sub-section 1.1. Future goals

	Horizon has a series of goals, ranked by their importance. The top goals are considered to be core features and the identity of the project hinges on their existence. Other goals are less important but add to the fun of the game.
	
	- Movement by walking.
	- Movement by vehicle.
	- Movement by walking within or on top of a moving vehicle.
	- Unlimited amount of terrain to traverse.
	
	- Destructable terrain a-la Geomod.
	
	- Enemies to shoot and guns to shoot them with
	
	- Order units around and have them build stuff a-la Iron Strategy
	
	- Ability to travel to other planets.
	- Correct orbital mechanics.
	- The spaceship has to be crafted with understanding of physics
	
	- Crafting in general.
	- Automatic coalescelnce of complex contraptions into simple representations where group acts as a unit
	
	- Crafting using non-abstracted tools.
	- Reliance on objects in the environment more than on player-character superpowers e.g. inventory.

//Section 2. How to compile

	Program can be compiled by running bin/compile.bat, or by invoking make within the bin directory.

//sub-section 2.1. Required software

	Horizon is classically compiled with MinGW (port of GCC for Windows) version 3.20. This uses gcc 4.6.3.
	Alternatively, Horizon can be compiled with nmake (make for windows) version 3.82.
	Version control is currently done with TortoiseSVN version 1.8.5 64-bit.


//sub-section 2.2. Folder structure

	Because SVN was set up that way, the entire project is embedded into C:/MinGW32/horizon/horizonSVN/horizon/ folder.
	Folder "bin" contains utilities used to compile and debug the program.
	Folder "random" contains code that is useful but not currently integrated into the project.
	Folder "source" contains .cpp source files.
	Folder "build" contains temporary files created during compilation. These can be safely deleted, but compilation will take longer while they are re-generated.
	Folder "include" contains .h header files that usually correspond to files in the "source" folder.
	Folder ".svn" is a hidden folder used by SVN to keep track of file version info.

	Inside each of the folders "source", "build" and "include", there are the following sub-folders:

	"display" contains code to display stuff on screen.
	"experimental" contains not entirely tested high-level features.
	"game" contains core game engine code.
	"gui" contains the code for window/widget toolkit used to draw GUI elements.
	"input" contains code for receiving input from keyboard and mouse.
	"main" contains program entry point and top-level program control code.
	"math" contains functions for performing vector, matrix, and quaternion math.
	"resource" contains code to import graphics, models, and sounds into the game.
	"util" contains general-purpose helpful functions.

//Section 3. How to use

	Just be like, "engage".