#include < bangtal.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <string.h>

using namespace std;

SceneID scene1;
ObjectID background_Blank, puzzle[16],original[16],start;
TimerID timer1,timer_sub, timer_main;

const char* puzzle_image[16] = {"Images/0.png","Images/1.png", "Images/2.png", "Images/3.png", 
"Images/4.png", "Images/5.png", "Images/6.png", "Images/7.png", "Images/8.png", "Images/9.png",
"Images/10.png", "Images/11.png", "Images/12.png", "Images/13.png", "Images/14.png", "Images/15.png" };
const int puzzle_x[16] = {340, 490, 640, 790, 340, 490, 640, 790, 340, 490, 640, 790, 340, 490, 640, 790 };
const int puzzle_y[16] = {510, 510, 510, 510, 360, 360, 360, 360, 210, 210, 210, 210, 60, 60, 60, 60 };
int blank = 15, mixcount, gametype = 0, time_1;;

time_t time_start, time_end;
const char* time_f;




int puzzle_index(ObjectID object) {
	for (int i = 0; i < 16; i++) {
		if (puzzle[i] == object) return i;
	}
	return -1;
}

void puzzle_move(int index) {

	ObjectID t = puzzle[blank];
	puzzle[blank] = puzzle[index];
	puzzle[index] = t; 

	locateObject(puzzle[blank], scene1, puzzle_x[blank], puzzle_y[blank]);
	locateObject(puzzle[index], scene1, puzzle_x[index], puzzle_y[index]);

	blank = index;
}
bool move_possible(int index) {
	
		if (index < 0 || index> 15) return false;
		if (index == blank - 1 && blank % 4 != 0) return true;
		if (index == blank + 1 && blank % 4 != 3) return true;
		if (index == blank - 4 && blank / 4 != 0) return true;
		if (index == blank + 4 && blank / 4 != 3) return true;

		return false;
	
	
	
}
void puzzle_mix() {
	int index;
	
	do{ 
	switch (rand() % 4) {
	case 0: index = blank - 1; break;
	case 1: index = blank + 1; break;
	case 2: index = blank - 4; break;
	case 3: index = blank + 4; break;
	}

	} while (!move_possible(index));
	puzzle_move(index);
}
bool puzzle_check() {
	for (int i = 0; i < 16; i++) {
		if (original[i] != puzzle[i]) return false;
		
	}
	return true;
}
void reset() {
	for (int i = 0; i < 16; i++) {
		puzzle[i] = original[i];
		locateObject(puzzle[i], scene1, puzzle_x[i], puzzle_y[i]);
		showObject(puzzle[i]);
		blank = 15;
	}
	setTimer(timer_main, 360.0f);
}
	
void time_check() {
	time_1 = time_end - time_start;
	string time_2;
	time_2 = to_string(time_1);
	time_f = time_2.c_str();
	showMessage(time_f);
}

void mouse_callback(ObjectID object, int x, int y, MouseAction action) {
	int index = puzzle_index(object);
	if (index == -1) {
		if (object == start)
			hideObject(start);
			mixcount = 300;
			startTimer(timer1);
			hideObject(puzzle[blank]);
	}
	else {
		if (gametype == 1 && move_possible(index)) {
			puzzle_move(index);
			if (puzzle_check()) {
				showObject(start);
				time_end = time(NULL);
				time_check();
				
				reset();
				stopTimer(timer_main);
				gametype = 0;
			}
		}
	}
}

void timer_callback(TimerID timer) {
	if (timer == timer1) {
		mixcount--;
		if (mixcount > 0) {
			puzzle_mix();
			setTimer(timer1, 0.05f);
			startTimer(timer1);
		}
		else {
			startTimer(timer_main);
			time_start = time(NULL);
			gametype = 1;
		}
	}
	else if (timer == timer_main) {
		showMessage("Time Over");
		gametype = 0;
		reset();
		showObject(start);

	}
	
	
}



int main() {
	setMouseCallback(mouse_callback);
	setTimerCallback(timer_callback);

	srand(time(NULL));
	scene1 = createScene("back", "Images/back.png");
	timer1 = createTimer(0.5f);
	timer_main = createTimer(360.f);
	showTimer(timer_main);
	background_Blank = createObject("Images/blank.png");
	locateObject(background_Blank, scene1, 340, 60);
	showObject(background_Blank);


	for (int i = 0; i < 16; i++) {
		puzzle[i] = createObject(puzzle_image[i]);
		locateObject(puzzle[i], scene1, puzzle_x[i], puzzle_y[i]);
		showObject(puzzle[i]);
		original[i] = puzzle[i];
	}
	

	start = createObject("Images/start.png");
	locateObject(start, scene1, 1000, 70);

	showObject(start);
	
	

	startGame(scene1);
	
}