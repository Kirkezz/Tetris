#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
#include <vector>

class Tetris {
private:
	Clock clock;
	struct polyomino {
		int x, y, color, type;
	};
	polyomino figures[7][4] = {
		{{4, 0}, {4, 1}, {4, 2}, {4, 3}}, // I
		{{4, 0}, {4, 1}, {5, 0}, {5, 1}}, // O
		{{4, 0}, {4, 1}, {5, 0}, {3, 1}}, // S
		{{4, 0}, {4, 1}, {3, 0}, {5, 1}}, // Z
		{{3, 0}, {3, 1}, {3, 2}, {4, 2}}, // L
		{{4, 0}, {4, 1}, {4, 2}, {3, 2}}, // J
		{{4, 0}, {5, 0}, {6, 0}, {5, 1}}, // T
	};
	std::vector<polyomino> selectedFigure, nextFigure, holdedFigure;
	RenderWindow& window;
	int field[40][20] = { 0 };
	double squareSize = 0;
	bool canHoldThisFrame = 1;
	Color getColor(int color) {
		switch (color) {
		case 0:
			return Color(0, 0, 0);
		case 1:
			return Color(255, 255, 0);
		case 2:
			return Color(128, 0, 128);
		case 3:
			return Color(0, 255, 0);
		case 4:
			return Color(255, 0, 0);
		case 5:
			return Color(0, 0, 255);
		case 6:
			return Color(255, 127, 0);
		case 7:
			return Color(0, 255, 255);
		}
	}
	void draw() {
		RectangleShape temp;
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 20; ++j) {
				temp.setPosition(i * squareSize, j * squareSize);
				temp.setSize(Vector2f(squareSize - 1, squareSize - 1));
				temp.setFillColor(getColor(field[i][j]));
				if (temp.getFillColor() == Color(0, 0, 0)) {
					temp.setSize(Vector2f(squareSize, squareSize));
				}
				window.draw(temp);
			}
		}
		for (int i = 0; i < selectedFigure.size(); ++i) {
			temp.setPosition(selectedFigure[i].x * squareSize, selectedFigure[i].y * squareSize);
			temp.setSize(Vector2f(squareSize - 1, squareSize - 1));
			temp.setFillColor(getColor(selectedFigure[i].color));
			window.draw(temp);
		}
		for (int i = 0; i < nextFigure.size(); ++i) {
			temp.setPosition(nextFigure[i].x * squareSize + 275, nextFigure[i].y * squareSize + 50);
			temp.setSize(Vector2f(squareSize - 1, squareSize - 1));
			temp.setFillColor(getColor(nextFigure[i].color));
			window.draw(temp);
		}
		for (int i = 0; i < holdedFigure.size(); ++i) {
			temp.setPosition(holdedFigure[i].x * squareSize + 275, holdedFigure[i].y * squareSize + 350);
			temp.setSize(Vector2f(squareSize - 1, squareSize - 1));
			temp.setFillColor(getColor(holdedFigure[i].color));
			window.draw(temp);
		}
	}
	void selectNewFigure() {
		selectedFigure = nextFigure;
		nextFigure.clear();
		for (int i = 0, figure = rand() % 7, color = rand() % 7 + 1; i < 4; ++i) {
			nextFigure.push_back(figures[figure][i]);
			nextFigure[nextFigure.size() - 1].color = color;
			nextFigure[nextFigure.size() - 1].type = figure;
		}
	}
	void dropFigure() {
		if (speed == 1)
			speed = 300;
		for (int i = 0; i < selectedFigure.size(); ++i) {
			if (field[selectedFigure[i].x][selectedFigure[i].y]) {
				std::cout << "You lose!";
				return;
			}
		}
		canHoldThisFrame = 1;
		for (int i = 0; i < selectedFigure.size(); ++i) {
			field[selectedFigure[i].x][selectedFigure[i].y] = selectedFigure[i].color;
		}
		selectedFigure.clear();
	}
public:
	int speed = 300;
	Tetris(RenderWindow& window) : window(window) {
		squareSize = window.getSize().y / 20.0;
		for (int i = 0, figure = rand() % 7, color = rand() % 7 + 1; i < 4; ++i) {
			nextFigure.push_back(figures[figure][i]);
			nextFigure[nextFigure.size() - 1].color = color;
			nextFigure[nextFigure.size() - 1].type = figure;
		}
	}
	void update() {
		if (clock.getElapsedTime().asMilliseconds() > speed) {
			down();
			clock.restart();
		}
		if (selectedFigure.empty()) {
			selectNewFigure();
		}
		for (int y = 0; y < 20; ++y) {
			bool empty = 0;
			for (int x = 0; x < 10; ++x) {
				if (!field[x][y])
					empty = 1;
			}
			if (!empty) {
				for (int y1 = y; y1 > 0; --y1) {
					for (int x1 = 0; x1 < 10; ++x1) {
						field[x1][y1] = field[x1][y1 - 1];
					}
				}
			}
		}
		draw();
	}
	void left() {
		for (int i = 0; i < selectedFigure.size(); ++i) {
			if (selectedFigure[i].x == 0 || field[selectedFigure[i].x - 1][selectedFigure[i].y])
				return;
		}
		for (int i = 0; i < selectedFigure.size(); ++i) {
			--selectedFigure[i].x;
		}
	}
	void right() {
		for (int i = 0; i < selectedFigure.size(); ++i) {
			if (selectedFigure[i].x == 9 || field[selectedFigure[i].x + 1][selectedFigure[i].y])
				return;
		}
		for (int i = 0; i < selectedFigure.size(); ++i) {
			++selectedFigure[i].x;
		}
	}
	void down() {
		for (int i = 0; i < selectedFigure.size(); ++i) {
			if (selectedFigure[i].y == 19 || field[selectedFigure[i].x][selectedFigure[i].y + 1]) {
				dropFigure();
				return;
			}
		}
		for (int i = 0; i < selectedFigure.size(); ++i) {
			++selectedFigure[i].y;
		}
	}
	void rotate() {
		if (selectedFigure[0].type == 1) {
			return;
		}
		polyomino p = selectedFigure[1];
		for (int i = 0; i < 4; ++i) {
			int x = selectedFigure[i].y - p.y, y = selectedFigure[i].x - p.x;
			if (field[p.x - x][p.y + y] != 0 || p.x - x < 0 || p.x - x > 9 || p.y - y < 0 || p.y - y > 19) {
				return;
			}
		}
		for (int i = 0; i < 4; ++i) {
			int x = selectedFigure[i].y - p.y, y = selectedFigure[i].x - p.x;
			selectedFigure[i].x = p.x - x;
			selectedFigure[i].y = p.y + y;
		}
	}
	void hold() {
		if (!canHoldThisFrame)
			return;
		std::vector<polyomino> a = holdedFigure, b = selectedFigure;
		holdedFigure.clear();
		selectedFigure.clear();
		for (int i = 0; i < b.size(); ++i) {
			holdedFigure.push_back(figures[b[i].type][i]);
			holdedFigure[holdedFigure.size() - 1].color = b[i].color;
			holdedFigure[holdedFigure.size() - 1].type = b[i].type;
		}
		for (int i = 0; i < a.size(); ++i) {
			selectedFigure.push_back(figures[a[i].type][i]);
			selectedFigure[selectedFigure.size() - 1].color = a[i].color;
			selectedFigure[selectedFigure.size() - 1].type = a[i].type;
		}
		if (selectedFigure.size() == 0) {
			selectNewFigure();
		}
		canHoldThisFrame = 0;
	}
};