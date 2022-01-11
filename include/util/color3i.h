#ifndef COLOR3I_GUARD
#define COLOR3I_GUARD

struct color3i{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
color3i operator + (color3i A, color3i B);
color3i operator * (color3i A, int B);
color3i operator / (color3i A, int B);
color3i operator * (color3i A, double B);
color3i operator / (color3i A, double B);

#endif