#include "util/globals.h"

color3i operator + (color3i A, color3i B)
{
	return {(uint8_t)clamp(A.r+B.r,0,255),(uint8_t)clamp(A.g+B.g,0,255),(uint8_t)clamp(A.b+B.b,0,255)};
}
color3i operator * (color3i A, int B){
	return {(uint8_t)clamp(A.r*B,0,255),(uint8_t)clamp(A.g*B,0,255),(uint8_t)clamp(A.b*B,0,255)};
}
color3i operator / (color3i A, int B){
	return {(uint8_t)clamp(A.r/B,0,255),(uint8_t)clamp(A.g/B,0,255),(uint8_t)clamp(A.b/B,0,255)};
}
color3i operator * (color3i A, double B){
	return {(uint8_t)clamp(A.r*B,0,255),(uint8_t)clamp(A.g*B,0,255),(uint8_t)clamp(A.b*B,0,255)};
}
color3i operator / (color3i A, double B){
	return {(uint8_t)clamp(A.r/B,0,255),(uint8_t)clamp(A.g/B,0,255),(uint8_t)clamp(A.b/B,0,255)};
}
