#ifndef GAME_GUARD
#define GAME_GUARD

#ifdef USE_SDL
#include <Ext/SDL2/SDL.h>
#endif

#include "main/main.h"
#include "math/simplemath.h"
#include "experimental/editor2.h"
#include "gadgets/console.h"
#include "gadgets/picker.h"
#include "game/commands.h"
#include "game/GUIwindows.h"
#include "GUI/GUI.h"
#include "program/window.h"
#include "render/render.h"
#include "render/renderLayer.h"
#include "render/renderLow.h"
#include "render/camera.h"
#include "render/paint.h"
#include "resource/resource.h"
#include "loaders/fontLoader.h"
#include "generators/editmodel.h"
#include "input/mouse.h"
#include "input/input.h"
#include "input/keybinds.h"
#include "util/timer.h"
#include "util/util.h"
#endif
