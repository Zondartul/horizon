/*!
@mainpage Horizon: Main Page
This is the documentation for the Horizon game engine. The engine is mostly modular, with a low degree of coupling between different systems.
@section Systems
@subsection Display
This system handles on-screen rendering.
See @link paint.h@endlink and @link renderable.h@endlink.
@subsection Game
This super-system handles overal game logic. It depends on most other systems.
@subsection GUI
This is the GUI system that contains several GUI toolkits and user interfaces built out of them.
@subsection Input
This system receives interactive input from the outside world and translates it into the format understood by the game engine.
@subsection Main
This super-system handles overal engine logic. It depends on most other systems.
@subsection Resource
This system reads game files and performs acquisition and memory management for other data-heavy resources.
@subsection SDL
The SDL library is a cross-platform multimedia library. It allows access to keyboard, mouse, window creation, and an OpenGL context in an OS-independent manner.
@subsection Messaging
This system allows for asynchronous communication between parts of the engine. Messages can range from simple events to complex non-POD objects.
@subsection Debug
The debug system has lots of features, but for some reason  printf is used everywhere instead.
@section Libraries
There are several collections that provide utility functions but have no responsibilities of their own, e.g. globals.h
@subsection Geometry
There are 2 and 3 dimensional vectors (pairs/triplets of floats or integers) as well as a "rect" class for advanced screen area calculations.
@subsection Strings
There are several convienience functions for string manipulation and converting data to strings.
@subsection Colors
Color3i and Color4i are RGB and RGBA representations used by stuff.
@subsection Math
Math functions contain operations on quaternions, 3d vectors, and polygon meshes.
*/