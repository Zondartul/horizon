void OnProgramStart();
void paintRect(int x1, int y1, int x2, int y2);
void Render2D();
void Render3D();
void Render_go2D();
void Render_go3D();
void RenderTick(HDC hDC);
void keyThing(UINT umsg, WPARAM wParam, LPARAM lParam);

void ProgramTick(HWND hwnd, HDC hDC);
void ProcessMouseclick(int mb);
void ProcessKeyboard(int kb);
void CallDestructor();
