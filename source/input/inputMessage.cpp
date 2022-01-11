#include "input/inputMessage.h"

//messages of type: key_down, key_still_down, key_up
message_key::message_key(){type = "key_something";}

//messages of type: lmb_down, lmb_up, rmb_down, rmb_up
message_mouse::message_mouse(){newPos = {0,0}; type = "mouse_something";}

//messages of type: mouse_move
message_mouse_move::message_mouse_move(){deltaPos = {0,0}; type = "mouse_move";}

//messages of type: mouse_wheel
message_mouse_wheel::message_mouse_wheel(){deltaWheel = 0; type = "mouse_wheel";}