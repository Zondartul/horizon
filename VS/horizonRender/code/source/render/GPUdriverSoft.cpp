#include "render/GPUdriverSoft.h"

GPUdriverSoft::~GPUdriverSoft() {}

#define rs stateStack.state

void GPUdriverSoft::renderLowInit() {}

void GPUdriverSoft::setViewportSize(int width, int height) {
	rs.width = width;
	rs.height = height;
}

void GPUdriverSoft::parseQueue(renderQueue3* rqueue) {
	for (auto I = rqueue->begin(); I != rqueue->end(); I++) {
		renderCommand3& rcmd = **I;
		parseCommand(rcmd);

	}
}

void GPUdriverSoft::parseCommand(const renderCommand3& rcmd) {
    switch (rcmd.type) {
        case(LAYER):            command_layer(rcmd.layer);      break;
        case(COLORING):         command_coloring(rcmd.b);       break;
        case(UVCOLORING):       command_uvcoloring(rcmd.b);     break;
        case(NORMALCOLORING):   command_normalcoloring(rcmd.b); break;
        case(TRANSPARENCY):     command_transparency(rcmd.b);   break;
        case(DEPTHMASK):        command_depthmask(rcmd.b);      break;
        case(TEXTURING):        command_texturing(rcmd.b);      break;
        case(DEBUG):            command_debugging(rcmd.b);      break;
        case(PROJECTION):       command_projection(rcmd.cam);   break;
        case(POSITION):         command_position(rcmd.v3);      break;
        case(ROTATION):         command_rotation(rcmd.v3);      break;
        case(SCALE):            command_scale(rcmd.v3);         break;
        case(TEXTURE_UPLOAD):   command_texture_upload(rcmd.t); break;
        case(TEXTURE_SELECT):   command_texture_select(rcmd.t); break;
        case(TEXTURE_POS):      command_texture_pos(rcmd.v2);   break;
        case(TEXTURE_SCALE):    command_texture_scale(rcmd.v2); break;
        case(RMODEL_UPLOAD):    command_rmodel_upload(rcmd.rm); break;
        case(RMODEL_RENDER):    command_rmodel_render(rcmd.rm); break;
        case(RMODEL_DELETE):    command_rmodel_delete(rcmd.rm); break;
        case(CLEAR_SCREEN):     command_clear_screen();         break;
        case(SCISSORING):       command_scissoring(rcmd.b);     break;
        case(COLOR):            command_color(rcmd.v3);         break;
        case(ALPHA):            command_alpha(rcmd.f);          break;
        //case(FONT_SELECT):      command_font_select(rcmd.fnt);  break;
        case(MODE_SELECT):      command_mode_select(rcmd.i);    break;
        //case(TEXT_POS):         command_text_pos(rcmd.v2);      break;
        case(SCISSOR):          command_scissor(rcmd.r);        break;
        case(POINTSIZE):        command_pointsize(rcmd.f);      break;
        case(LINEWIDTH):        command_linewidth(rcmd.f);      break;
        case(PRINT_TEXT):       command_print_text(rcmd.s);     break;
        case(DEPTH_TEST):       command_depth_test(rcmd.b);     break;
        case(LIGHTING):         command_lighting(rcmd.b);       break;
        case(SUN_POS):          command_sun_pos(rcmd.v3);       break;
        case(SUN_LIGHT_COLOR):      command_sun_light_color(rcmd.v3); break;
        case(AMBIENT_LIGHT_COLOR):  command_ambient_light_color(rcmd.v3); break;
        case(READ_PIXELS):      command_read_pixels(rcmd.buff); break;
        case(VIEWPORT):         command_viewport(rcmd.r);       break;
        case(FACE_CULLING):     command_face_culling(rcmd.b);   break;
        case(FACE_CULL_CCW):    command_face_cull_ccw();        break;
        case(PUSH_OPTIONS):     command_push_options();         break;
        case(POP_OPTIONS):      command_pop_options(); break;
        case(RESET_OPTIONS):	command_reset_options(); break;
        case(PRINT_OPTIONS):	command_print_options(); break;
    }
}

void GPUdriverSoft::reproject() {}