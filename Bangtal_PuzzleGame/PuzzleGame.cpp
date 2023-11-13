#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES	1

#include <bangtal.h>
#include <string>
#include <string.h>
#include <random>
#include <time.h>

using namespace std;

SceneID scene_back;
ObjectID start, back_blank, initObjects[16], gameObjects[16], timer_block;
TimerID timerMixing, timer_end;

time_t time_start, time_end;

char message[30];
int blank = 15, rd_num, mixing = 0, time_1;
const int mixCount = 300;
const Second animationTime = 0.01f;
const char* time_f;

bool gametype = false;

struct POS {
	int x, y;
};
POS positions[16] = {
	{340, 510}, {490, 510}, {640, 510}, {790, 510},
	{340, 360}, {490, 360}, {640, 360}, {790, 360},
	{340, 210}, {490, 210}, {640, 210}, {790, 210},
	{340, 60}, {490, 60}, {640, 60}, {790, 60},
};

ObjectID create_Object(const char* image, SceneID scene, int x, int y, bool shown) {

	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}
	return object;
}

void random_number(int start, int end) {
	random_device rd;
	mt19937_64 rdn(rd());
	uniform_int_distribution<int> dis(start, end);
	rd_num = dis(rdn);
}

void set_backgroud() {
	switch (rd_num)
	{
	case 0:
		scene_back = createScene("background", "Images1/back.png");

		break;
	case 1:
		scene_back = createScene("background", "Images2/back.png");
		break;

	case 2:
		scene_back = createScene("background", "Images3/back.png");
		break;
	}
}

void set_image() {
	char image[30];
	switch (rd_num)
	{
	case 0:
		for (int i = 0; i < 16; i++) {
			sprintf(image, "Images1/Puzzle_%d.png", i + 1);
			initObjects[i] = create_Object(image, scene_back, positions[i].x, positions[i].y, true);
			gameObjects[i] = initObjects[i];

		}
		break;
	case 1:
		for (int i = 0; i < 16; i++) {
			sprintf(image, "Images2/Puzzle_%d.png", i + 1);
			initObjects[i] = create_Object(image, scene_back, positions[i].x, positions[i].y, true);
			gameObjects[i] = initObjects[i];

		}
		break;
	case 2:
		for (int i = 0; i < 16; i++) {
			sprintf(image, "Images3/Puzzle_%d.png", i + 1);
			initObjects[i] = create_Object(image, scene_back, positions[i].x, positions[i].y, true);
			gameObjects[i] = initObjects[i];

		}
		break;
	
	}
}

void game_init()
{
	
	random_number(0,2);
	set_backgroud();
	back_blank = create_Object("Images/blank.png", scene_back, 340, 60, true);
	set_image();
	timer_block = create_Object("Images/block.png", scene_back, 610, 670, true);
	start = create_Object("Images/start.png", scene_back, 1000, 20, true);

	blank = 15;

	gametype = false;

	timerMixing = createTimer();
}


int click_return(ObjectID objectid) {
	for (int i = 0; i < 16; i++) {
		if (gameObjects[i] == objectid) return i;
	}

	return -1;
}


bool puzzle_move(int index)
{
	if (index < 0) return false;
	if (index > 15) return false;

	if (index == (blank - 4) || index == (blank + 4) ||
		(index == (blank - 1) && (blank % 4) != 0) ||
		(index == (blank + 1) && (blank % 4) != 3)) {

		locateObject(gameObjects[index], scene_back, positions[blank].x, positions[blank].y);
		locateObject(gameObjects[blank], scene_back, positions[index].x, positions[index].y);

		ObjectID temp = gameObjects[blank];
		gameObjects[blank] = gameObjects[index];
		gameObjects[index] = temp;

		blank = index;

		return true;
	}

	return false;
}

void puzzle_random_move()
{
	bool possible = false;
	int next = -1;
	while (!possible)
	{
		random_number(0, 3);
		switch (rd_num) {
		case 0:
			next = blank - 4;
			break;
		case 1:
			next = blank + 4;
			break;
		case 2:
			next = (blank % 4 != 0) ? (blank - 1) : -1;
			break;
		case 3:
			next = (blank % 4 != 3) ? (blank + 1) : -1;
			break;
		}

		possible = puzzle_move(next);
	}
}

void game_prepare()
{
	hideObject(gameObjects[blank]);

	mixing = mixCount;

	setTimer(timerMixing, animationTime);
	startTimer(timerMixing);

}

bool game_check()
{
	for (int i = 0; i < 16; i++) {
		if (gameObjects[i] != initObjects[i]) return false;
	}

	return true;
}
void time_check() {
	time_1 = time_end - time_start;
	string time_2;
	time_2 = to_string(time_1);
	time_f = time_2.c_str();
	showMessage(time_f);
}
void set_timer_end() {
	timer_end = createTimer(360.0f);
	startTimer(timer_end);
	showTimer(timer_end);
}
void game_end()
{
	gametype = false;

	showObject(gameObjects[blank]);
	setObjectImage(start, "Images/restart.png");
	showObject(start);

	time_end = time(NULL);
	time_check();

}



void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (gametype) {
		puzzle_move(click_return(object));

		if (game_check()) {
			game_end();
			time_check();
			stopTimer(timer_end);
		}
	}
	else if (mixing == 0 && object == start) {
		game_prepare();

	}

}

void timerCallback(TimerID timer)
{
	if (timer == timerMixing) {
		puzzle_random_move();

		if (mixing > 0) {
			--mixing;

			setTimer(timerMixing, animationTime);
			startTimer(timerMixing);
			set_timer_end();
		}
		else {

			hideObject(start);
			time_start = time(NULL);
			showMessage("start, 완성시 걸린시간(초)이 표시됩니다.");
			gametype = true;
		}

	}
	else if (timer == timer_end) {
		game_end();
		showMessage("time over!!");
	}
}

int main()
{
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, 0);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, 0);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, 0);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	game_init();
	startGame(scene_back);
}