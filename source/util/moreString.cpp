#include "util/globals.h"

std::string operator+(std::string const &a, int b){
  std::ostringstream oss;
  oss<<a<<b;
  return oss.str();
}

std::string operator+(std::string const &a, double b){
  std::ostringstream oss;
  oss<<a<<b;
  return oss.str();
}

string tolower(string A){
	string B = A;
	for(int I = 0; I<A.length();I++){
		B[I] = tolower(A[I]);
	}
	return B;
}
string toupper(string A){
	string B = A;
	for(int I = 0; I<A.length();I++){
		B[I] = toupper(A[I]);
	}
	return B;
}
string fstring(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	size_t num = vsnprintf(0, 0, fmt, args);
	char *buff = new char[num+1];
	vsnprintf(buff, num+1, fmt, args);
	va_end(args);
	return string(buff);
}


string translateEnum(uint32_t E, string type){
	if(type == "glGetError"){
		switch(E){
			case GL_NO_ERROR:return "GL_NO_ERROR";break;
			case GL_INVALID_ENUM:return "GL_INVALID_ENUM";break;
			case GL_INVALID_VALUE:return "GL_INVALID_VALUE";break;
			case GL_INVALID_OPERATION:return "GL_INVALID_OPERATION";break;
			//case GL_INVALID_FRAMEBUFFER_OPERATION:return "GL_INVALID_FRAMEBUFFER_OPERATION";break;
			case GL_OUT_OF_MEMORY:return "GL_OUT_OF_MEMORY";break;
			default:return "unknown "+type+" enum";
		}
	}else if(type == "SDL_WINDOWEVENT"){
		switch(E){
			case SDL_WINDOWEVENT_NONE: return "SDL_WINDOWEVENT_NONE"; break;
			case SDL_WINDOWEVENT_SHOWN: return "SDL_WINDOWEVENT_SHOWN"; break;
			case SDL_WINDOWEVENT_HIDDEN: return "SDL_WINDOWEVENT_HIDDEN"; break;
			case SDL_WINDOWEVENT_EXPOSED: return "SDL_WINDOWEVENT_EXPOSED"; break;
			case SDL_WINDOWEVENT_MOVED: return "SDL_WINDOWEVENT_MOVED"; break;
			case SDL_WINDOWEVENT_RESIZED: return "SDL_WINDOWEVENT_RESIZED"; break;
			case SDL_WINDOWEVENT_SIZE_CHANGED: return "SDL_WINDOWEVENT_SIZE_CHANGED"; break;
			case SDL_WINDOWEVENT_MINIMIZED: return "SDL_WINDOWEVENT_MINIMIZED"; break;
			case SDL_WINDOWEVENT_MAXIMIZED: return "SDL_WINDOWEVENT_MAXIMIZED"; break;
			case SDL_WINDOWEVENT_RESTORED: return "SDL_WINDOWEVENT_RESTORED"; break;
			case SDL_WINDOWEVENT_ENTER: return "SDL_WINDOWEVENT_ENTER"; break;
			case SDL_WINDOWEVENT_LEAVE: return "SDL_WINDOWEVENT_LEAVE"; break;
			case SDL_WINDOWEVENT_FOCUS_GAINED: return "SDL_WINDOWEVENT_FOCUS_GAINED"; break;
			case SDL_WINDOWEVENT_FOCUS_LOST: return "SDL_WINDOWEVENT_FOCUS_LOST"; break;
			case SDL_WINDOWEVENT_CLOSE: return "SDL_WINDOWEVENT_CLOSE"; break;
		}
	}
	return "(unknown enum type)";
}