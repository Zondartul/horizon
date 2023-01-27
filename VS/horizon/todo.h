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
/// - [IDEA] input handler tree 
/// - [IDEA] DLLs interchangeable (e.g. ProgramWin, ProgamLinux, ProgramSDL; RenderOpenGL, RenderSoft)
///		-- also a nexus that can be configured on which dll to load.
/// - [IDEA] render-command protocol with separate commands/data
/// - [IDEA] apps do not paint directly, but create render-objects. ro's make rcmds. rcmds are monotonic (no time travel).
///		-- this will help with debugging as we can make a horizonRender eat a stream immediately OR buffer it.
/// - [IDEA] have horizonRender run in a separate thread (render scene with ro's is the common ground)
/// - [IDEA] options window for the entire program, split by module.
///		-- with input/output dir selection
/// - [IDEA] have data_in be read-only
/// - [IDEA] Build/deploy step that copies everything needed to run the game into a single folder
///		-- with resource packager to make it a single file
/// - [IDEA] tree of nodes (ownership / parts / composition)
/// - [IDEA] task manager for threads / processes
/// - [IDEA] horizonRender for software rendering (no gpu, draw to BMP)
///     -- with a debug drawer showing screen, texture memory, depth map, etc.
///	- [IDEA] GUI: textEdit should make the cursor jump to where it is clicked.
/// - [IDEA] GUI: add layout elements (Hbox, Vbox)
/// - [IDEA] GUI: add radiobutton
/// 
/// Bugs:
///	- [FIXED BUG] crash in horizonApp when clicking frame button in guieditor
///		-- thrown from horizonRender:GPUdriverKind::parseCommand (select texture but tex not uploaded)
///		-- only after mouse goes into the working area of the editor
///		--- fix: added a line to upload texture when tool_place is constructed
///		-- [related BUG] crash in horizonApp when clicking a textentry in opengui 3
///			--- texture not uploaded - maybe getTexture had used to upload them too?
/// - [FIXED BUG] resizing a window makes all UI derped
///		-- added an "event window_resized" but who should listen to it?
///		-- not sure what the problem is. Does the viewport match the window or is it clipped?
///			need some 3D stuff to check.
///		-- fix: turns out paint-to-rmcd function just forgot to handle viewport and some other commands,
///			so it didn't package their args.
/// - [BUG] horizonApp: guieditor: when a file is loaded, the gui is squoshed
/// - [BUG] Crash in horizonApp due to memory leak (2.1 GB) probably due to text?
///		-- idea: make text only as renderable; forbid direct text drawing (printw)