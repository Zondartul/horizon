#include "stdio.h"
#include "modules/os/os.h"
#include "modules/display/display.h"
#include "colors.h"


#include "modules/display/renderqueue.h"
void boop(){
	auto R = new r_rect2D_outline();
	R->area = rect(100,80).moveTo({50,50});
	R->color = white;
	renderQueue.push_back(R);
	auto R2 = new r_line2D();
	R2->A = {100,100};
	R2->B = {200,200};
	R2->cA = red;
	R2->cB = blue;
	renderQueue.push_back(R2);
}

int main(){
	printf("Hello world\n");
	openWindow();
	initDisplay();
	while(1){
		osCheckMessages();
		boop();
		render();
		sleep();
	}
}