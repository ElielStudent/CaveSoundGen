#include <iostream>
#include <SFML/Graphics.hpp>
#define WIDTH 800
#define HEIGHT 800
#define CAVEWIDTH (WIDTH/10)
#define CAVEHEIGHT (HEIGHT/10)
#define WALLCHANCE 45

using namespace sf;
using namespace std;

int cave[CAVEWIDTH][CAVEHEIGHT] = { {0} };
int lastCave[CAVEWIDTH][CAVEHEIGHT] = { {0} };

void copyCave() {
	for (int y = 0; y < CAVEHEIGHT; y++) {
		for (int x = 0; x < CAVEWIDTH; x++) {
			lastCave[y][x] = cave[y][x];
		}
	}
}

int getNeighbors(int x, int y) {
	bool hasBelow = y < CAVEHEIGHT -2;
	bool hasAbove = y > 1;
	bool hasLeft = x > 1;
	bool hasRight = x < CAVEWIDTH -2;

	return (!hasAbove || lastCave[y - 1][x]) +
		(!hasAbove || !hasLeft || lastCave[y - 1][x - 1]) +
		(!hasAbove || !hasRight || lastCave[y - 1][x + 1]) +
		(!hasLeft || lastCave[y][x - 1]) +
		(!hasRight || lastCave[y][x + 1]) +
		(!hasBelow || lastCave[y + 1][x]) +
		(!hasBelow || !hasLeft || lastCave[y + 1][x - 1]) +
		(!hasBelow || !hasRight || lastCave[y + 1][x + 1]);

}

void fillCave() {
	srand(time(NULL));
	for (int y = 0; y < CAVEHEIGHT; y++) {
		for (int x = 0; x < CAVEWIDTH; x++) {
			cave[y][x] = rand() % 101 < WALLCHANCE;
			lastCave[y][x] = cave[y][x];
		}
	}
}

void drawCave(RenderWindow& window) {
	RectangleShape rect;
	rect.setSize(Vector2f(WIDTH / CAVEWIDTH, HEIGHT / CAVEHEIGHT));
	rect.setFillColor(Color::Black);
	for (int y = 0; y < CAVEHEIGHT; y++) {
		for (int x = 0; x < CAVEWIDTH; x++) {
			if (cave[y][x]) {
				rect.setPosition(Vector2f(x * (WIDTH / CAVEWIDTH), y * (HEIGHT / CAVEHEIGHT)));
				window.draw(rect);
			}
		}
	}
}

void UpdateNeighbors() {
	int neighbors;
	for (int y = 0; y < CAVEHEIGHT; y++) {
		for (int x = 0; x < CAVEWIDTH; x++) {
			neighbors = getNeighbors(x, y);
			cave[y][x] = (neighbors == 4 && lastCave[y][x]) || neighbors >= 5;
		}
	}
	copyCave();
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML cave works!");
	fillCave();
	while (window.isOpen())
	{
		window.clear(Color::White);
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyReleased) {
				UpdateNeighbors();
				if (event.key.code == Keyboard::R)
					fillCave();
			}
		}
		drawCave(window);
		window.display();
	}

	return 0;
}