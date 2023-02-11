/// 
/// Horizon
/// 21.01.2023: This is "gui_split" branch that got lost in HDD borking of summer 2022.
/// 
/// First split was in winter 2021-2022: 
///		horizon 
///			-> 
///		horizonOStools + horizonRender + horizon
/// 
/// Second split was in summer 2022:
///		horizonOStools + horizonRender + horizon
///			->
///		horizonUtil + horizonProgram + horizonResource ... horizonRender + horizonGUI ... horizonApp
/// Ok.
/// 
/// Todo:
/// 1. Find the old todo list(s)?
/// 3. idk just modernize everything 
/// 5. make a global input handler tree
/// 
/// Ideas:
/// - [IDEA 1] input handler tree 
/// - [IDEA 2] DLLs interchangeable (e.g. ProgramWin, ProgamLinux, ProgramSDL; RenderOpenGL, RenderSoft)
///		-- also a nexus that can be configured on which dll to load.
/// - [IDEA 3] render-command protocol with separate commands/data
/// - [IDEA 4] apps do not paint directly, but create render-objects. ro's make rcmds. rcmds are monotonic (no time travel).
///		-- this will help with debugging as we can make a horizonRender eat a stream immediately OR buffer it.
/// - [IDEA 5] have horizonRender run in a separate thread (render scene with ro's is the common ground)
/// - [IDEA 6] options window for the entire program, split by module.
///		-- with input/output dir selection
/// - [IDEA 7] have data_in be read-only
/// - [IDEA 8] Build/deploy step that copies everything needed to run the game into a single folder
///		-- with resource packager to make it a single file
/// - [IDEA 9] tree of nodes (ownership / parts / composition)
/// - [IDEA 10] task manager for threads / processes
/// - [IDEA 11] horizonRender for software rendering (no gpu, draw to BMP)
///     -- with a debug drawer showing screen, texture memory, depth map, etc.
///	- [IDEA 12] GUI: textEdit should make the cursor jump to where it is clicked.
/// - [IDEA 13] GUI: add layout elements (Hbox, Vbox)
/// - [IDEA 14] GUI: add radiobutton
/// - [IDEA 15] renderableText: replace missing characters with boxes
/// - [IDEA 16] replace printw with renderableText
/// - [IDEA 17] horizonRender: add reset_options
///		-- also have it reset on boot
/// - [IDEA 18] horizonRender: ability to mute some commands
/// - [IDEA 19] horizonRender: debug command to print and diff renderOptions
/// - [IDEA 20] general: arbitrary (standardized) object serialization via <<archive
/// - [IDEA 21] horizonRender: RenderState: replace VP, camPos with camprojection
/// - [IDEA 22] horizonRender: separate the state-changing part of a command from the "do stuff" part.
///		-- also deduplicate renderState and renderOptions
/// 
/// 
/// Bugs:
///	v [fixed bug 1] crash in horizonApp when clicking frame button in guieditor
///		-- thrown from horizonRender:GPUdriverKind::parseCommand (select texture but tex not uploaded)
///		-- only after mouse goes into the working area of the editor
///		--- fix: added a line to upload texture when tool_place is constructed
///		-- [related BUG] crash in horizonApp when clicking a textentry in opengui 3
///			--- texture not uploaded - maybe getTexture had used to upload them too?
/// v [fixed bug 2] resizing a window makes all UI derped
///		-- added an "event window_resized" but who should listen to it?
///		-- not sure what the problem is. Does the viewport match the window or is it clipped?
///			need some 3D stuff to check.
///		-- fix: turns out paint-to-rmcd function just forgot to handle viewport and some other commands,
///			so it didn't package their args.
/// v [fixed bug 3] horizonApp: guieditor: when a file is loaded, the gui is squoshed
///		-- this is due to fromString<rect> impl switching from scanf to operator>>,
///			which ignores spaces. Fixed by reducing the number of throwaway characters.
///			(related: bug 4)
/// v [fixed bug 4] horizonApp: guieditor: when a file is loaded, text doesn't display.
///		-- this is due to fromString<font*> impl parsing font name but not size.
///			(related: bug 3)
/// - [BUG 5] Crash in horizonApp due to memory leak (2.1 GB) probably due to text?
///		-- idea: make text only as renderable; forbid direct text drawing (printw)
/// - [BUG 6] horizonApp: guieditor: can't add a tab widget
/// v [fixed bug 7] horizonApp: opengui 2 and 3: crash
///		-- was because getTextCentering doesn't handle all enum values
/// - [fixed bug 9] horizonApp: opengui4: crash
///		-- renderText tried to render a glypth with no texture
///		--- glypth is not in charmap. Font has no newline '\n' character.
///		--- fix: don't print missing characters	(see idea 15)
/// - [BUG 10] horizonApp: opengui4: crash when clicking the close button
///		-- also see [bug 20]
/// v [fixed bug 11] horizonGUI: textures and text are glitched out and incomprehensible
///		-- when implementing (idea 16)
///		-- looks like merely enabling textures messes things up, also discovered (bug 13)
/// v [fixed bug 12] horizonGUI: crash when using PUSH_OPTIONS.
///		-- the option was simply not handled
/// - [BUG 13] horizonRender: PUSH/POP options doesn't restore all the options.
///		-- reopened: current texture and rmodel are not checked
/// - [BUG 14] horizonGUI: labels with renderableText have tiiiny text
///		-- BUT console still looks good, and it also uses renderableText
///			maybe something in GUI leaves an incompatible setting?
///		-- try turning on GUI push options
///		-- renderOptions appear the same... see if rmodel and texture are the same?
///     -- turns out this happens when font is "calibri 16", it works
///			fine with "cour 14". check if other fonts are crappy
///		-- calibri 12 and 16 fails, 14 and 18 are ok.
/// v [fixed bug 15] horizonRender: crash if using resetOptions right after init
///		-- probably was about an unhandled render command?
///	- [BUG 16] horizonRender: resetOptions at init causes buttons and pictures to not display.
/// - [BUG 17] horizonRender: text is wrong color
///		-- console was green and labels blue.
///		-- now console white and labels black.
/// - [BUG 18] horizonApp: opengui 0 crashes if window was closed (dangling pointer)
///		-- needs smart pointers?
/// - [fixed bug 19] all text bork depending on whether the console is shown.
///		-- seen on opengui 8 once opengui 4 was shown
///			--- removing renderlayer commands from opengui4 isolates the fuckery
///		-- seen on opengui 3 regardless of 4
///		-- seems to be caused by the "grid" element
///		-- fix: in the "drawRectOutline" change renderMode back from wireframe.
/// - [BUG 20] horizonApp: opengui 4 button 'clear' crash.
///		-- also see [bug 10]
/// -----------------------------------------------
///  LOST AND FOUND: if other 'todo' lines are found, move them here
/// -----------------------------------------------
/// 
/// 
/// 
///  