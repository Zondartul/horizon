#ifndef COLOR4I_GUARD
#define COLOR4I_GUARD
struct color4i{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	void setColor(color3i newColor);
};
color4i operator + (color4i A, color4i B);
color4i operator * (color4i A, int B);
color4i operator / (color4i A, int B);
color4i operator * (color4i A, double B);
color4i operator / (color4i A, double B);
#endif