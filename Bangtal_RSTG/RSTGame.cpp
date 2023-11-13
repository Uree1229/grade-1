// RSTGame : Reaction Speed Test Game

#include <bangtal.h>
#include <random>
#include <string>
#include <string.h>

using namespace std;

SceneID scene_game, scene_title, scene_ending, scene_role;
ObjectID dot, Button_start, Button_end, Button_role, Button_back;
TimerID timer_dot, timer_game;

string second_score;

int dotX, dotY, score = 0, gametype = 0;  //gametype 0 = out game, 1 = in game
const char* final_score;


ObjectID createObject(const char*image, SceneID scene, int x, int y, bool shown, float scale) {
	
	ObjectID object = createObject(image);
	locateObject(object,scene,x,y);
	scaleObject(object, scale);
	if (shown) {
		showObject(object);
	}
	return object;
}
void titlesetting() {
	scene_title = createScene("title", "Images/title.png");
	Button_start = createObject("Images/start.png", scene_title, 390, 20, true, 1.0f);
	Button_end = createObject("Images/end.png", scene_title, 590, 20, true, 1.0f);
	Button_role = createObject("Images/howtoplay.png", scene_title, 790, 20, true, 1.0f);
}
void endingsetting() {
	scene_ending = createScene("ending", "Images/ending.png");
	setObjectImage(Button_start, "Images/restart.png");
	locateObject(Button_start, scene_ending, 590, 70);
	locateObject(Button_end, scene_ending, 590, 20);
	enterScene(scene_ending);
}
void rolesetting() {
	scene_role = createScene("role","Images/role.png");
	Button_back =createObject("Images/back.png", scene_role, 590, 20, true, 1.0f);
	
}
void createDot_random() {
	if (gametype == 1) {
		random_device rdx;
		random_device rdy;

		mt19937_64 rdX(rdx());
		mt19937_64 rdY(rdy());

		uniform_int_distribution<int> disX(0, 1080);
		uniform_int_distribution<int> disY(0, 640);
		dotX = disX(rdX);
		dotY = disY(rdY);
		locateObject(dot, scene_game, dotX, dotY);
	}
}
void startGame() {
	score = 0;
	gametype = 1;
	enterScene(scene_game);
	createDot_random();

	timer_dot = createTimer(0.5f);
	startTimer(timer_dot);

	timer_game = createTimer(30.0f);
	startTimer(timer_game);

	showTimer(timer_game);
	
}

void mouse_Callback(ObjectID object, int x, int y, MouseAction action) {

	if (object == dot) {
		hideObject(dot);
		showMessage("success, +1");
		score = score + 1;
	}
	else if (object == Button_start) {
		startGame();
	}
	else if (object == Button_end) {
		endGame();
	}
	else if (object == Button_role) {
		rolesetting();
		enterScene(scene_role);
		
	}
	else if (object == Button_back) {
		enterScene(scene_title);
	}

}
void timer_Callback(TimerID timer) {
	
	
	
	if (timer == timer_dot) {

		hideObject(dot);
		createDot_random();
		showObject(dot);
		setTimer(timer_dot, 0.5f);
		startTimer(timer_dot);
	}
	else if (timer == timer_game) {

		gametype = 0;
		second_score = to_string(score);
		final_score = second_score.c_str();
		endingsetting();
		showMessage(final_score);
		
	}

}



int main()
{
	
	setMouseCallback(mouse_Callback);
	setTimerCallback(timer_Callback);
	
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, 0);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, 0);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, 0);

	dot = createObject("Images/dot.png", scene_game, dotX, dotY, true, 1.0f);

	scene_game = createScene("background", "Images/background.png");
	titlesetting();
	startGame(scene_title);
}
