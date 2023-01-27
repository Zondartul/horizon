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
/// 
/// Bugs:
///	- [FIXED BUG 1] crash in horizonApp when clicking frame button in guieditor
///		-- thrown from horizonRender:GPUdriverKind::parseCommand (select texture but tex not uploaded)
///		-- only after mouse goes into the working area of the editor
///		--- fix: added a line to upload texture when tool_place is constructed
///		-- [related BUG] crash in horizonApp when clicking a textentry in opengui 3
///			--- texture not uploaded - maybe getTexture had used to upload them too?
/// - [FIXED BUG 2] resizing a window makes all UI derped
///		-- added an "event window_resized" but who should listen to it?
///		-- not sure what the problem is. Does the viewport match the window or is it clipped?
///			need some 3D stuff to check.
///		-- fix: turns out paint-to-rmcd function just forgot to handle viewport and some other commands,
///			so it didn't package their args.
/// - [FIXED BUG 3] horizonApp: guieditor: when a file is loaded, the gui is squoshed
///		-- this is due to fromString<rect> impl switching from scanf to operator>>,
///			which ignores spaces. Fixed by reducing the number of throwaway characters.
///			(related: bug 4)
/// - [FIXED BUG 4] horizonApp: guieditor: when a file is loaded, text doesn't display.
///		-- this is due to fromString<font*> impl parsing font name but not size.
///			(related: bug 3)
/// - [BUG 5] Crash in horizonApp due to memory leak (2.1 GB) probably due to text?
///		-- idea: make text only as renderable; forbid direct text drawing (printw)