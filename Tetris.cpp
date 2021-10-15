#include <SFML/Graphics.hpp>
#include "Game.h"
using namespace sf;

#include <iostream>
using namespace std;

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(555, 768), "Tetris!");
    Tetris* tetris = new Tetris(window);
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                case 0:
                    tetris->left();
                    break;
                case 18:
                    tetris->down();
                    break;
                case 3:
                    tetris->right();
                    break;
                case 22:
                    tetris->rotate();
                    break;
                case 38:
                    tetris->hold();
                    break;
                case 57:
                    tetris->speed = 1;
                    break;
                case 17:
                    tetris = new Tetris(window);
                    break;
                }
            }
        }

        window.clear(Color(0, 0, 0));
        tetris->update();



        window.display();
    }

    return 0;
}