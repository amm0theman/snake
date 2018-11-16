#include "OpenGL.h"
#include "GL/glut.h"
#include "GL/GL.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>
#include <Windows.h>

using namespace std;
const int gridHeight = 50;
const int gridWidth = 60;
const int INITIAL_SIZE = 10;
const double PGenerateFruit = .01;

bool bw = false;
bool paused = false;
int headX = 25;
int headY = 25;
int grow = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;

char board[gridWidth][gridHeight];
char direction = 'E';
bool gameOver = true;

void DrawElement(double i, double j, char element) {
	double x = (i+.5) / gridWidth * 2.0 - 1.0;
	double y = (j+.5) / gridHeight * 2.0 - 1.0;
	double r = 1.0 / gridHeight;
	switch (element) {
	case 'S':
		glColor3f(0, bw*.5, !bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 's':
		glColor3f(0, !bw*.5, bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 'A':
		glColor3f(1.0, 0.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x+r*.15, y+r*.9, r*.4, 10);
		break;
	case 'C':
		glColor3f(0, 0.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 0, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case 'G':
		glColor3f(1, 1, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case ' ':
		break;
	default:
		break;
	}
}

bool calculateChance(double chanceOfHappening) {
	return rand() / (double)RAND_MAX < chanceOfHappening;
}

bool squareFilled(int x, int y, char type) {
	return board[x][y] != 'S' && board[x][y] != 's' && board[x][y] != type;
}

void movehead() {
	//Find out where the head will be...
	switch (direction) {
	case 'N': headY++; break;
	case 'E': headX++; break;
	case 'S': headY--; break;
	case 'W': headX--; break;
	}
}

void wraparound() {
	if (headX < 0) gameOver = true;
	if (headY < 0) gameOver = true;
	if (headX >= gridWidth)  gameOver = true;
	if (headY >= gridHeight) gameOver = true;
}

void movetail() {
	//Move the snake's tail..
	if (grow <= 0) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
	}
	else {
		grow--;
	}
}

void cherrybomb() {
	for (int i = 0; i < 6; i++) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
	}
}

void movesnake() {
	movehead();
	wraparound();
	movetail();
}

void generateItems() {
	//Generate Fruit...
	if (calculateChance(PGenerateFruit)) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;
		if (board[x][y] != 'S' && board[x][y] != 's' && board[x][y] != 'C') {
			board[x][y] = 'A';
		}
		if (calculateChance(PGenerateFruit * 10))
		{
			if (board[x][y] != 'S' && board[x][y] != 's')
			{
				board[x][y] = 'C';
			}
		}
		if (calculateChance(PGenerateFruit * 10)) 
		{
			if (board[x][y] != 'S' && board[x][y] != 's')
			{
				board[x][y] = 'G';
			}
		}
	}

}

void rungame() {
	bw = !bw;

	generateItems();
	movesnake();

	switch (board[headX][headY])
	{
	case 'A': grow += 4; break;
	case 'S': case 's': gameOver = true; break;
	case 'C': cherrybomb(); break;
	case 'G': grow += 8; break;
	}

	//Actually move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);
}

void update() {
	if (!paused) {
		rungame();
	}
}



	void draw() 
	{

	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			DrawElement(i, j, board[i][j]);
		}
	}
	Sleep(100);
	if (!gameOver)
		update();
}

void keyboard(int key) {
	switch (key) {
		case 'w': if (direction == 'E' || direction == 'W') direction = 'N'; break;
		case 'a': if (direction == 'N' || direction == 'S')	direction = 'W'; break;
		case 's': if (direction == 'E' || direction == 'W')	direction = 'S'; break;
		case 'd': if (direction == 'N' || direction == 'S') direction = 'E'; break;
		case 'p': paused = !paused; break;
		case ' ': 
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				headX = gridWidth / 2.0;
				headY = gridHeight / 2.0;
				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}
				grow = INITIAL_SIZE;
			}
			break;
	}
}

int main(int argc, char** argv) {
	RunOpenGL(argc, argv, draw, NULL, keyboard);
}