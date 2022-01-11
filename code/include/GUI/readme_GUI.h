
//=========================================
// Horizon GUI widget toolkit
//
// Clients: include this file to get access
// to all the widgets.
//=========================================

//=========================================
// TODO: fix textEntry so that shift works,
// else can't enter +,(,),|,<,>,_,!,@,#,$,%
// etc.
//
// - make a "tick/think" func that is called
//   on every logic-tick (NOT RENDER). 
//	 This is for the
//	 "renderless GUI that really exists" idea
//
// - make a click-blocker thingy
// --- optionally block everything but mouseover checks
// 
// - keep a separate render order so elements
//	 appear on top of each other
//
// - add a "debug render" and "scissorless render"
//   options
//
// - make forms with elements selectable through 
//	 enter and tab, also record form answers etc.
//
// - for GUI_renderer, make a universal "color palette"
//   object, and have "color blend modes".
//
// - make a transform-chain - set of mutually pointing
//	 "transform" objects that form a "skeleton", so
//   coordinates can be automatically transformed from A-relative
//   to B-relative coordinates.
//
// - make an event-chain, so we can debug who observes or gates which events.
//
//=========================================

//=========================================
// GUI creation guide:
// 1. construct an instance of a GUI widget
//  	GUIwindow *win = new GUIwindow();
// 2. set it's properties
//		win->setTitle("hello");
//		win->setSize(200,200);
// 3. construct one or more controls to populate the widget
//		GUIbutton *btn = new GUIbutton();
//		btn->setText("push me");
//		btn->setSize(vec2(150,150));
//		btn->setPos(vec2(25,25));
//		btn->setFunction([=](){printf("hi\n");});
// 4. add the controls to the widget
//		win->addChild(btn);
// 5. add the widget to the root GUI object
//		GUI->addChild(win);
//=========================================

//=========================================
// Interactivity guide:
// 1. information and indication elements
//
//		GUIlabel - a simple string text
//
//		GUIimage - an image. Can use any .png or .jpg. The image can be
//	resized by making the widget bigger or smaller.
//	
//		(future) GUIrichText - a large block of formatted text.
//
//		(future) GUIplot - a plot, 2d-graph, or histogram that allows
//	multidimensional, unstructured numeric data to be displayed in a 
//	fancy fashion.
//
//		(future) GUIdiagram - a widget that allows structured data, such
//	as graph diagrams, trees, and block schematics to be displayed.
//
//		(future) GUIrender - an embedded window where OpenGL rendering
//	may take place.
//
// 2. controls
//
//		GUIbutton - a button that, when pressed, calls a function. Can
//	have text or an image assigned. Use setFunction(functor f) to set
//	the callback.
//
//		(future) GUIcheckbox - a checkbox that can be checked or not.
//
//		(future) radiobutton - a round checkbox where only one radiobutton
//	in a radiogroup may be checked at a time.
//
//	3. data entry
//
//		GUItextEntry - a rectangular area where text may be entered.
//	Set a function with setFunction(functor f). When one of conditions
//	is met (text edited, enter pressed, focus lost), and if that
//	condition is enabled, the function will be called.
//
//		(future) GUIcolorSelector - a rainbowy rectangle from which a color
//	may be picked.
//
//		(future) GUIdirectionSelector - a compass where 2D or 3D direction or
//	vector may be chosen.
//
//		(future) GUIdragSelectSpace - a space where widgets may be selected
//	by clicking them, or by drawing a selection rectangle, dragged, and
//	double-clicked to select them.
//
//		(future) GUIselectableItem - a widget that may be selected
//	in the GUIdragSelectSpace.
//
//	4. (future) forms
//
//		(future) GUIform - an invisible element that logically groups
//	data entry and display widgets, without affecting their layout.
//	allows entered data to be extracted in a single summary.
//
//=========================================

//=========================================
// Layout guide:
// 1. manual layout
//
//  	setSize(vec2) to set the elements size manually
//
//  	moveTo(vec2) to set the elements position manually
//
//  	invalidate() for the element to change it's internal
//	layout according to size and position.
//
// 2. semi-automatic layout
//
//  	sizeToContents() - for the element to shrink or expand until
//	it fits all the children (only happens once per call)
//
//		(future)sizeToFill() - for the element to expand until
//	it uses all the space offered by parent.
//
// 3. use basic widgets
//
//  	GUIbase - an invisible rectangular element that other elements
//	can be attached to. The base can then be moved to move other 
//	elements as a group. Only elements within the client area 
//	(inside the invisible rectangle) will be drawn. GUIbase is also 
//	the base class for all other gui widgets.
//
//		GUIframe - same as GUIbase but draws a rectangle with a border 
//	and background.
//
//  	GUIscrollbar - a special widget that is bigger on the inside:
//	the client area is bigger than external area and can be scrolled 
//	with the scroll-bars.
//
//  	(future)GUItabs - a widget that lets you select one of several
//	tabs. All tabs occupy the same space and only the selected tab is
//	drawn.
//
// 4. use layout widgets
//
//		GUIgrid - aligns the elements to a rectangular grid, where rows
//	and columns may have different sizes.
//
//		(future) GUIhorizontalLayout - aligns the items horizontally
//	(a specialization of GUIgrid for a single row)
//
//		(future) GUIverticalLayout - aligns the items vertically
//	(a specialization of GUIgrid for a single column)
//=========================================

//=========================================
// Internal mechanics guide:
// 1. Hierarchy:
// 1.1 A GUIbase object exists as the root widget of the GUI
//	  (a different object would be used for other screens)
//    it accepts input events from the eventChannel "inputChannel"
//
// 1.2 All widgets are either root widgets or child widgets.
//	  Child widgets have their area defined relative to the start
//	  (top-left corner)(0,0) of the client area of their parent widget.
//	  Client area itself is a rectangle at a slight offset from the
//	  overall widget origin. Parts of widgets that are outside the
//	  client area of their parent are not drawn and do not receive clicks.
//
// 2. Rendering: 	 
// 2.1 every renderTick, the renderLogic() of the root widget
//	   is called by user while the renderLayer "layer2D" is current.
//
// 2.2 renderLogic() draws every child, and then calls their renderLogic().
//     widgets that are owned (part of) current widget are drawn first, then
//	   all client widgets. renderLogic() also applies a scissoring effect,
//	   so that only parts of children that are inside the client area
//	   of the current widget are drawn.
//
// 2.3 widget::render() is called by renderLogic(), and the widget
//	   submits renderCommands to the current renderLayer.
//
// 2.4 the widget that is listed later in the child list is drawn last,
//	   and so appears on top of other widgets.
//
// 2.5 a widget may be hidden, in which case it will not render and will not
//	   accept input events.
//
// 3. Input:
// 3.1 when a user input event happens (mouse move, mouse click, keyboard press)
//     an event is generated by system and submitted to the inputChannel.
//
// 3.2 when a mouseover event is received, the widgets calculate whether the new
//	   mouse position is inside the widget and if so, set their "mouseover" flag.
//
// 3.3 a widget that receives events will typically process it, and then
//	   publish it to it's children. The event may be masked by a child.
//
// 4. Layout changes
// 4.1 a widget whose size changes raises an "invalidate" event, that causes
//	   the widget to recalculate the positions and sizes of it's parts
//	   and of it's client area. The invalidate event is also passed to all children.
//
// 4.2 widget area changes caused by the invalidation resolution do not
//	   initiate an invalidation on their own. This is controlled by the 
//	   "suppressInvalidate" flag. This distinguishes changes caused by the
//	   widget toolkit from changes caused by user.
//
// 5. Subsystems
// 5.1 area - the "area" and "clientArea" are rects. Area is defined relative
//	   to the parent's clientArea, and clientArea is defined relative to the
//	   the widget's origin. Every widget contains functions to convert rects
//	   between coordinate systems, including thisToWorld(), clientToWorld().
//	   It is possible to calculate the subset of area and clientArea that are
//	   visible (i.e. not clipped by the borders of the ancestors' clientAreas).
//
//	5.2 border - borders are rects defined dynamically from the widget's area.
//	    it is possible to test whether a position is within any of the borders.
//
//	5.3 propertyTable - a table (map<string,string>) containing properties of
//		the widget that allow the widget to be controlled through scripting.
//
//=========================================