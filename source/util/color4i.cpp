#include "util/globals.h"

color4i operator + (color4i A, color4i B){
	return {(uint8_t)clamp(A.r+B.r,0,255),(uint8_t)clamp(A.g+B.g,0,255),(uint8_t)clamp(A.b+B.b,0,255),(uint8_t)clamp(A.a+B.a,0,255)};
}
color4i operator * (color4i A, int B){
	return {(uint8_t)clamp(A.r*B,0,255),(uint8_t)clamp(A.g*B,0,255),(uint8_t)clamp(A.b*B,0,255),A.a}; //multiplicative ops only affect color component
}
color4i operator / (color4i A, int B){
	return {(uint8_t)clamp(A.r/B,0,255),(uint8_t)clamp(A.g/B,0,255),(uint8_t)clamp(A.b/B,0,255),A.a};
}
color4i operator * (color4i A, double B){
	return {(uint8_t)clamp(A.r*B,0,255),(uint8_t)clamp(A.g*B,0,255),(uint8_t)clamp(A.b*B,0,255),A.a};
}
color4i operator / (color4i A, double B){
	return {(uint8_t)clamp(A.r/B,0,255),(uint8_t)clamp(A.g/B,0,255),(uint8_t)clamp(A.b/B,0,255),A.a};
}