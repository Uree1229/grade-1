#include <bangtal.h>
#include <string>
#include <string.h>

using namespace std;

SceneID scene, title;
ObjectID bord, stone_ck, dot_bord[361], stone_black[181], stone_white[180], turn_white, turn_black, start;

TimerID timer_bl, timer_wh, timer_end, timer_end2;
int select_num = 0;
int stone_bord_x[361], stone_bord_y[361], dot, state_bord[361];
int check_x = 0, check_y = 0, num_bl = 0, num_wh = 1, turn = 0; // 0 =black, 1= white




ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {

	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}
	return object;
}
void timer_create() {

	timer_bl = createTimer(20.0f);
	timer_wh = createTimer(20.0f);
	timer_end = createTimer(5.0f);
	timer_end2 = createTimer(3.0f);
}

void object_create() {

	start = createObject("Images/start.png", title, 590, 60, true);
	bord = createObject("Images/bord.png", scene, 340, 60, true);
	turn_black = createObject("Images/turn_black.png", scene, 106, 60, true);
	turn_white = createObject("Images/turn_white.png", scene, 1056, 60, false);

	
}

void bord_create() {
	for (int i = 0; i < 361; i++) {
		stone_bord_x[i] = 340 - 4 + (30 * (i % 19 + 1));
		stone_bord_y[i] = 60 - 4 + (30 * (19 - i / 19));
		dot_bord[i] = createObject("Images/dot_bord.png", scene, stone_bord_x[i], stone_bord_y[i], true);
		state_bord[i] = 0;
	}
	stone_ck = createObject("Images/stone_check.png", scene, check_x, check_y, false);
}

void game_setting() {

	title = createScene("title", "Images/title.png");
	scene = createScene("background", "Images/background.png");

	object_create();
	timer_create();
	bord_create();


}

int cordinate_click(ObjectID object) {
	for (int i = 0; i < 361; i++) {
		if (dot_bord[i] == object) {
			return i;
		}
	}
	if (object == stone_ck) return 361;
	return -1;
}

void turn_check() {
	if (turn == 0) {
		showObject(turn_black);
		hideObject(turn_white);
	}
	else if (turn == 1) {
		showObject(turn_white);
		hideObject(turn_black);
	}
}


void stone_check() {
	locateObject(stone_ck, scene, check_x, check_y);
	showObject(stone_ck);
}

void end_game() {
	hideTimer();
	startTimer(timer_end2);
	showTimer(timer_end);
	turn = 2;
}

void success_check() {
	for (int i = 0; i < 361; i++) {
		if (turn == 1) {
			if (i <=356 && (i % 19 <= 14) && state_bord[i] + state_bord[i + 1] + state_bord[i + 2] + state_bord[i + 3] + state_bord[i + 4] == 5)  {
				showMessage("Black Win!!");
				end_game();
			}
			else if (i <= 284  && state_bord[i] + state_bord[i + 19] + state_bord[i + 38] + state_bord[i + 57] + state_bord[i + 76] == 5) {
				showMessage("Black Win!!");
				end_game();
			}
			else if (i <= 284 && (i % 19 >= 4)&& state_bord[i] + state_bord[i + 18] + state_bord[i + 36] + state_bord[i + 54] + state_bord[i + 72] == 5) {
				showMessage("Black Win!!");
				end_game();
			}
			else if (i <= 280 && (i % 19 <= 14) && state_bord[i] + state_bord[i + 20] + state_bord[i + 40] + state_bord[i + 60] + state_bord[i + 80] == 5) {
				showMessage("Black Win!!");
				end_game();
			}
		}
		else {
			if (i <= 356 && (i % 19 <= 14) && (state_bord[i] + state_bord[i + 1] + state_bord[i + 2] + state_bord[i + 3] + state_bord[i + 4]) == -5) {
				showMessage("White Win!!");
				end_game();
			}
			else if (i <= 284 && state_bord[i] + state_bord[i + 19] + state_bord[i + 38] + state_bord[i + 57] + state_bord[i + 76] == -5) {
				showMessage("White Win!!");
				end_game();
			}
			else if (i <= 284 && (i % 19 >= 4) && state_bord[i] + state_bord[i + 18] + state_bord[i + 36] + state_bord[i + 54] + state_bord[i + 72] == -5) {
				showMessage("White Win!!");
				end_game();
			}
			else if (i <= 280 && (i % 19 <= 14) && state_bord[i] + state_bord[i + 20] + state_bord[i + 40] + state_bord[i + 60] + state_bord[i + 80] == -5) {
				showMessage("White Win!!");
				end_game();
			}
		}

	}
}

void timer_setting(TimerID timer, int type) {
	hideTimer();
	if (type == 0) {
		stopTimer(timer);
		setTimer(timer, 20.0f);
	}
	else if (type == 1) {
		showTimer(timer);
		startTimer(timer);
	}

}
void timerEvent(bool color) {
	if (color == true) { // black
		timer_setting(timer_bl, 0);
		timer_setting(timer_wh, 1);
	}
	else {
		timer_setting(timer_wh, 0);
		timer_setting(timer_bl, 1);
	}
}
void create_stone(bool color, int select) {
	turn_check();
	if (color == true) { //black
		stone_black[num_bl] = createObject("Images/stone_black.png", scene, stone_bord_x[select] + 4 - 10, stone_bord_y[select] + 4 - 10, true);
		state_bord[select] = 1;
		num_bl += 1;
		turn = 1;
	}
	else {
		stone_white[num_wh] = createObject("Images/stone_white.png", scene, stone_bord_x[select] + 4 - 10, stone_bord_y[select] + 4 - 10, true);
		state_bord[select] = -1;
		num_wh += 1;
		turn = 0;
	}
	timerEvent(color);
}

void Mouse_Callback(ObjectID object, int x, int y, MouseAction action) {

	int select = cordinate_click(object); // select<0 : 버튼 / select=0~360 : 격자점 / select= 361 : 확인용 돌

	if (select < 0) {
		if (object == start) {
			enterScene(scene);
			startTimer(timer_bl);
			showTimer(timer_bl);
		}
	}
	else {
		if (select >= 0 && select < 361) {
		
				select_num = select;
				check_x = stone_bord_x[select] + 4 - 10;
				check_y = stone_bord_y[select] + 4 - 10;
				stone_check();
				
			
			
		}
		else if (select == 361) {

			locateObject(stone_ck, scene, 0, 0);
			hideObject(stone_ck);

			if (turn == 0) {
				create_stone(true, select_num);
				success_check();
			}
			else if (turn == 1) {
				create_stone(false, select_num);
				success_check();			
			}
		}
	}
}

void Timer_callback(TimerID timer) {
	if (timer == timer_bl) {
		showMessage("White Win!!");
		end_game();
	}
	else if (timer == timer_wh) {
		showMessage("Black Win!!");
		end_game();
	}
	else if (timer == timer_end) {
		endGame();
	}
	else if (timer == timer_end2) {
		showMessage("5초후 게임이 종료됩니다!");
		startTimer(timer_end);

	}
}

int main()
{

	setMouseCallback(Mouse_Callback);
	setTimerCallback(Timer_callback);

	game_setting();

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, 0);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, 0);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, 0);

	startGame(title);

}
