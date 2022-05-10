#ifndef MAP_EDITOR_GUARD
#define MAP_EDITOR_GUARD
#include "game.h"

enum map_editor_mode {
	ME_MODE_DEFAULT,
	ME_MODE_BLOCK_PLACEMENT,
	ME_MODE_TERRAIN,
    ME_MODE_SELECT,
    ME_MODE_PLACE,
    ME_MODE_PATHFIND,
	ME_MODE_PROBE,
	ME_MODE_PLANE
};

enum map_editor_submode{
    ME_SUBMODE_DEFAULT,
    ME_SUBMODE_MOVE,
    ME_SUBMODE_PLACE_FLAG,
	ME_SUBMODE_POINT,
	ME_SUBMODE_CUBE
};

class map_editor_kind:public eventListener{
	public:
	GUIbase *mainWindow = 0;
	map_editor_mode mode = ME_MODE_DEFAULT;
    map_editor_submode submode = ME_SUBMODE_DEFAULT;
	renderLayer *layerMap3D = 0;
    renderLayer *layerMap2D = 0;
	float gridsize = 0.5f;
	vec3 sel_pos1;
	vec3 sel_pos2;
	vec3 mouseover_pos;
	vec3 mouseover_square;
	bool has_mouseover = false;
	bool is_selecting = false;
	vector<vec3> points;
	int selectmode;
	
    
	map_editor_kind();
	void onEvent(eventKind event);

    struct selectDataKind{
        vec3 ptUp;
        vec3 ptDn;
        vec3 ptLeft;
        vec3 ptRight;
        vec3 ptFwd;
        vec3 ptBack;

        vec3 ptUp2D;
        vec3 ptDn2D;
        vec3 ptLeft2D;
        vec3 ptRight2D;
        vec3 ptFwd2D;
        vec3 ptBack2D;

        vec3 dirAwayUp;
        vec3 dirAwayDn;
        vec3 dirAwayLeft;
        vec3 dirAwayRight;
        vec3 dirAwayFwd;
        vec3 dirAwayBack;

        float rotUp;
        float rotDn;
        float rotLeft;
        float rotRight;
        float rotFwd;
        float rotBack;

        bool dragging = 0;
        enum dirKind{DIR_NONE, DIR_UP, DIR_DN, DIR_LEFT, DIR_RIGHT, DIR_FWD, DIR_BACK} dir = DIR_NONE;
        float initialOffset;
    }sdata;

};


struct gs_map_editorKind {
    map_editor_kind* g_mapeditor;
   
    renderLayer* g_templayer = 0;
};

void openMapEditor();

#endif
