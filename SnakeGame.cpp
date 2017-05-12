/*__ _(_) __ _  ___  ___ _   _  __| | __ _ _   _| |_ ___
 / _` | |/ _` |/ _ \/ __| | | |/ _` |/ _` | | | | __/ _ \
| (_| | | (_| | (_) \__ \ |_| | (_| | (_| | |_| | || (_) |
 \__, |_|\__,_|\___/|___/\__,_|\__,_|\__,_|\__,_|\__\___/
 |___/                                 Snake Game Basic */
///Notice: Please complile with -std=c++11

#define _WIN32_WINNT 0x0500

#include <iostream>
#include <queue>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <stdlib.h>
#include <time.h>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

void gotoxy(int line, int column)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}


enum DIRECTION {
	UP, DOWN, LEFT, RIGHT
};

class Point {
public:
	int x, y;
	Point() {};
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point moveTo(DIRECTION d) {
		switch (d)
		{
		case UP:
			return Point(x - 1, y);
			break;
		case DOWN:
			return Point(x + 1, y);
			break;
		case RIGHT:
			return Point(x , y + 1);
			break;
		case LEFT:
			return Point(x , y - 1);
			break;
		default:
		    return Point(x,y);
			break;
		}
	}
};



void gotoPoint(Point p) {
	gotoxy(p.x, p.y);
}

class Prey {
public:
    Point postition;
	Prey() {};
	Prey(int x, int y) {
        postition.x=x;
        postition.y=y;
	}
	void show()
	{
		gotoPoint(postition);
		cout << 'O';
	}
};

class SnakeGame {
private:
	queue<Point> body;
	Point head, tail;
	int speed, hight, width;
	DIRECTION direction;
	bool area[100][100];
	Prey prey;
public:
	SnakeGame() {
		hight = 20;
		width = 50;
		speed = 200;
		head = Point(3, 3);
		tail = Point(3, 3);
		body.push(head);
		direction = RIGHT;
		memset(area, false, sizeof area);
		area[3][3] = true;
		show();
		prey.postition=creatPrey();
		prey.show();
	}

	void showScore(){
        gotoxy(0,7);
        cout<<body.size()-1;
	}

	Point creatPrey(){
        srand(time(NULL));
		while(true){
            int x = rand() % hight + 2;
            int y = rand() % width + 1;
            if(area[x][y]==false) return Point(x,y);
		}
		return Point(0,0);
	}

	bool isSafe() {
		Point nextPos = head.moveTo(direction);

		if (nextPos.x <= 1 || nextPos.x >= 2 + hight) return false;
		if (nextPos.y <= 0 || nextPos.y >= 1 + width) return false;

		if (area[nextPos.x][nextPos.y]) return false;
		return true;
	}
	void tryEat() {
		Point nextPos = head.moveTo(direction);
		if (nextPos.x == prey.postition.x && nextPos.y == prey.postition.y)
			earn(nextPos);
	}
  
	void move()
	{
		if (!isSafe()) die();
		tryEat();
		tail = body.front();
		body.pop();
		gotoPoint(tail);
		cout << ' ';
		area[tail.x][tail.y] = false;

		head = head.moveTo(direction);
		body.push(head);
		gotoPoint(head);
		cout << 'x';
		area[head.x][head.y] = true;

		gotoxy(hight+2, width+2);
	}
  
	void changeDirection(DIRECTION d) {
		if (d == LEFT && direction == RIGHT) return;
		if (d == RIGHT && direction == LEFT) return;
		if (d == UP && direction == DOWN) return;
		if (d == DOWN && direction == UP) return;
		this->direction = d;
	}
  
	int getSpeed() {
		return speed;
	}
  
	Point getHead() {
		return head;

	}
	void earn(Point p) {
		head = p;
		body.push(head);
		gotoPoint(head);
		cout << 'x';
		area[head.x][head.y] = true;
		prey.postition=creatPrey();
		prey.show();
		showScore();
	}

	void die() {
		gotoxy(0, 15);
		cout << "You lose";
		speed = 99999999;
	}
	void show()
	{
	    gotoxy(0,0);
		cout << "Score: " << endl;
		cout << (char)201;
		for (int i = 1; i <= width; i++) cout << (char)205;
		cout << (char)187;
		cout << endl;
		for (int i = 1; i <= hight; i++)
		{
			cout << (char)186;
			for (int i = 1; i <= width; i++) cout << " ";
			cout << (char)186 << endl;
		}
		cout << (char)200;
		for (int i = 1; i <= width; i++) cout << (char)205;
		cout << (char)188;
	}
};

SnakeGame snake;

void SnakeMove() {
	while (true) {
		snake.move();
		Sleep(snake.getSpeed());
	}
}

void ListenKeyboard() {
	int c = 0;
	while (true)
	{
		c = 0;

		switch ((c = _getch())) {
		case KEY_UP:
			snake.changeDirection(UP);
			break;
		case KEY_DOWN:
			snake.changeDirection(DOWN);
			break;
		case KEY_LEFT:
			snake.changeDirection(LEFT);
			break;
		case KEY_RIGHT:
			snake.changeDirection(RIGHT);
			break;
		default:
			break;
		}
	}
}

int main()
{
	  HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions
    MoveWindow(console, r.left, r.top, 500, 450, TRUE);

	  thread moveSnake(SnakeMove);
	  thread listenKeyboard(ListenKeyboard);

	  moveSnake.join();
	  listenKeyboard.join();

  	return 0;
}

