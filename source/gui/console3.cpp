#include "console3.h"

//hacks
//call this like:
//string str; vfstring(&str);

string vfstring(const char *fmt, va_list args){
	size_t num = vsnprintf(0, 0, fmt, args);
	char *buff = new char[num+1];
	vsnprintf(buff, num+1, fmt, args)
	return string(buff);
}

// for reference:
// string fstring(const char* fmt, ...){
	// va_list args;
	// va_start(args, fmt);
	// size_t num = vsnprintf(0, 0, fmt, args);
	// char *buff = new char[num+1];
	// vsnprintf(buff, num+1, fmt, args);
	// va_end(args);
	// return string(buff);
// }
//print formatted output to cursor, advance cursor by amount printed
void console3::printf(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	string str = vfstring(fmt, args);
	va_end(args);
	print(str.c_str());
}
//print formatted output to specific coordinates
void console3::printfXY(int x, int y, const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	string str = vfstring(fmt, args);
	va_end(args);
	printXY(x,y, str.c_str());
}
//print a c string to cursor, advance cursor by amount printed
void print(const char *str){
	int I = 0;
	char C = str[I++];
	while(C != 0){
		printchar(C);
		C = str[I++];
	}
}
//print a c string to specific coordinates
void printXY(int x, int y, const char *str){
	if((x < 0) || (x >= width_chars)){printf("x = %d\n",x); assert(!"print coordinate x out of bounds");}
	if((y < 0) || (y >= height_chars)){printf("y = %d\n",y); assert(!"print coordinate y out of bounds");}
	int I = y*width_chars+x;
	int Imax = width_chars*height_chars;
	int J = 0;
	char C = str[J];
	while(C && (I < Imax)){
		consoleChar CC = {bgcolor, fgcolor, str[J++]};
		drawBuffer[I++] = CC;
	}
}
//print character to cursor, advance cursor by one
void printchar(char C){
	if(C == '\n'){
		int rem = cursorpos % width_chars;
		if(rem){
			cursorpos = cursorpos - rem + width_chars;
			checkCursor();
		}
	}else{
		consoleChar CC = {bgcolor, fgcolor, C};
		drawBuffer[cursorpos++] = CC;
		checkCursor();
	}
}

/* if(cursorpos >= history_height*width_chars){
	outputHistory.erase(outputHistory.begin(), outputHistory.begin()+width_chars);
	outputHistory.resize(width_chars*height_history);
} */

//handle the scroll logic
void checkCursor(){
	int x = cursorpos % width_chars;
	int y = (cursorpos - x ) / width_chars;
	if(y > height_chars){
		//scroll the console up
		scrollines++;
		if(scrollines >= height_history){
			//delete the first line in history
			outputHistory.erase(outputHistory.begin(), outputHistory.begin()+width_chars);
		}
		//move the first line in drawbuffer to the last line in history
		outputHistory.insert(outputHistory.end(), drawBuffer.begin(), drawBuffer.begin()+width_chars);
		//delete the first line in drawbuffer
		drawBuffer.erase(drawBuffer.begin(), drawBuffer.begin()+width_chars);
		drawBuffer.resize(width_chars*height_chars);
		y--;
	}
}

//print character to specific coordinates
void printcharXY(char C){
	if((x < 0) || (x >= width_chars)){printf("x = %d\n",x); assert(!"print coordinate x out of bounds");}
	if((y < 0) || (y >= height_chars)){printf("y = %d\n",y); assert(!"print coordinate y out of bounds");}
	consoleChar CC = {bgcolor, fgcolor, C};
	drawBuffer[y*width_chars+x] = CC;
}
//custom constructor
console3::console3(){
	cursorpos = 0;
	width_chars = 80;
	height_chars = 25;
	char_width = 8;
	char_height = 12;
	consoleChar C = {{0,0,0,255},{0,255,0,255},' '};
	for(int I = 0; I < height_chars; I++){
		for(int J = 0; J < width_chars; J++){
			drawBuffer.push_back(C);
		}
	}
}
//custom render function
void console3::render(){
	GUI2base::render();
	int x1;
	int y1;
	int x2;
	int y2;
	//1) draw the console output
	for(int I = 0; I < height_chars; I++){
		for(int J = 0; J < width_chars; J++){
			consoleChar C = drawBuffer[I*width_chars + J];
			x1 = pos.x + J*char_width;
			y1 = pos.y + I*char_height;
			x2 = x1 + char_width;
			y2 = y1 + char_height;
			setColor(C.bgcolor);
			paintRect(x1,y1,x2,y2);
			setColor(C.fgcolor);
			printw(x1,x2,-1,-1,"%c",C.C);
		}
	}
	//2) draw the command prompt
	//later...
}
//custom message receiver
void console3::receiveMessage(message *msg){
	if(msg->type == "key_down"){
		if(msg->name == "enter"){
			
		}else{
		char C = msg->name[0]; //I really need a "key-to-ascii" and "isprint" funcs
		}
	}
}










