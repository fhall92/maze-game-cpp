
#include "pch.h"
#include "maze.cpp"
#include <string>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <sstream>

int main()
{
	float mazeX = 0;
	float mazeY = 0;
	int tileDimension = 16;
	int enemy1Direction = 0;
	int enemy2Direction = 0;
	int enemy3Direction = 0;
	int enemy4Direction = 0;


	int maze1[10][20] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
						{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
						{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
						{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
						{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1},
						{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };
	
	class Wall {
	public:
		sf::RectangleShape shape;


		Wall(){
			int tileDimension = 16;
			shape.setSize(sf::Vector2f(tileDimension,tileDimension));
			//shape.setFillColor(sf::Color::Blue);
			shape.setOutlineColor(sf::Color::Black);
			shape.setOutlineThickness(-1);

		}
	};

	class Floor {
	public:
		
		sf::RectangleShape shape;

		Floor() {
			
			int tileDimension = 16;
			shape.setSize(sf::Vector2f(tileDimension, tileDimension));
			//shape.setFillColor(sf::Color::Green);
		
		}

	};

	class Player {
	public:
		sf::Sprite sprite;

		Player() {
			sprite.setScale(1, 1);
		}
	};

	class Enemy {
		bool killedState = false;

	public:
		sf::Sprite sprite;

		Enemy() {
			sprite.scale(1, 1);
		}

		void kill()
		{
			killedState = true;
			sprite.setPosition(9, 0);			
		}

		bool getKilledState()
		{
			return killedState;
		}
	};
	
	class Explosion {
	public:

		sf::RectangleShape shape;
		

		Explosion() {

			int tileDimension = 16;
			shape.setSize(sf::Vector2f(tileDimension, tileDimension));
			//shape.setFillColor(sf::Color::Green);

		}


	};

	class Bomb {
	public:
		sf::RectangleShape shape;
		bool placedState = false;

		Bomb() {

			int tileDimension = 16;
			shape.setSize(sf::Vector2f(tileDimension, tileDimension));
		}
	};

	Wall wall = Wall();
	Floor floor = Floor();
	Player player = Player();
	Bomb bomb = Bomb();
	Enemy enemy1 = Enemy();
	Enemy enemy2 = Enemy();
	Enemy enemy3 = Enemy();
	Enemy enemy4 = Enemy();
	Explosion explosion1 = Explosion();
	Explosion explosion2 = Explosion();
	Explosion explosion3 = Explosion();
	Explosion explosion4 = Explosion();

	int playerLives = 3;
	int score = 0;
	
	sf::Font font;
	sf::Text lives;
	sf::Text scores;
	sf::Text endGame;

	
	lives.setScale(0.2, 0.2);
	lives.setFont(font);
	scores.setScale(0.2, 0.2);
	scores.setFont(font);
	endGame.setScale(1, 1);
	endGame.setFont(font);

	sf::Time threeSeconds = sf::seconds(3);
	sf::Time explosionSec = sf::seconds(4);
	sf::Time invulnerabilityDuration = sf::seconds(1.5);


	sf::Texture playerTexture;
	sf::Texture wallTexture;
	sf::Texture floorTexture;
	sf::Texture bombTexture;
	sf::Texture explosionTexture;
	sf::Texture enemyTexture;

	bool bombSet = false;
	bool playerHit = false;
	bool winCondition = false;
	bool loseCondition = false;
	

	std::vector<sf::FloatRect> walls;
	
	font.loadFromFile("font.ttf");
	wallTexture.loadFromFile("wallTexture.png");
	floorTexture.loadFromFile("floorTexture.png");
	playerTexture.loadFromFile("playerTexture.png");
	bombTexture.loadFromFile("bombTexture.png");
	enemyTexture.loadFromFile("enemyTexture.png");
	explosionTexture.loadFromFile("explosionTexture.png");

	

	//create Window
	sf::RenderWindow window(sf::VideoMode(320, 160), "BomberPerson");
	

	// dont change this code..START
	// set timePerFrame to 1 60th of a second. 60 frames per second

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	// the clock object keeps the time.
	sf::Clock clock;
	clock.restart();

	sf::Clock fuse;
	sf::Clock explosionTime;
	sf::Clock invulnerabilityTimer;
	


	lives.setPosition((wall.shape.getGlobalBounds().width), (wall.shape.getGlobalBounds().height));
	scores.setPosition((16 * wall.shape.getGlobalBounds().width), (1 * wall.shape.getGlobalBounds().height));

	enemy1.sprite.setPosition((4 * wall.shape.getGlobalBounds().width), (2 * wall.shape.getGlobalBounds().height));
	enemy2.sprite.setPosition((15 * wall.shape.getGlobalBounds().width), (3 * wall.shape.getGlobalBounds().height));
	enemy3.sprite.setPosition((4 * wall.shape.getGlobalBounds().width), (7 * wall.shape.getGlobalBounds().height));
	enemy4.sprite.setPosition((15 * wall.shape.getGlobalBounds().width), (6 * wall.shape.getGlobalBounds().height));
	player.sprite.setPosition((9 * wall.shape.getGlobalBounds().width), (5 * wall.shape.getGlobalBounds().height));
	
	while (window.isOpen()){


		if (winCondition == false && loseCondition == false) {
			std::stringstream livesStream;
			livesStream << "Lives " << playerLives;
			std::string livesString = livesStream.str();
			lives.setString(livesString);

			std::stringstream scoreStream;
			scoreStream << "Score " << score;
			std::string scoreString = scoreStream.str();
			scores.setString(scoreString);

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			//add to the time since last update and restart the clock
			timeSinceLastUpdate += clock.restart();



			if (timeSinceLastUpdate > timePerFrame) {

				//Win and Lose events
				window.clear();
				if(playerLives == 0){
					loseCondition = true;
				}

				if (score == 4) {
					winCondition = true;
				}


				//Draw in Maze
				for (int row = 0; row < 10; row++) {
					for (int col = 0; col < 20; col++) {
						mazeX = col * wall.shape.getGlobalBounds().width;
						mazeY = row * wall.shape.getGlobalBounds().height;

						if (maze1[row][col] == 1)
						{
							walls.push_back(wall.shape.getGlobalBounds());
							wall.shape.setPosition(mazeX, mazeY);
							wall.shape.setTexture(&wallTexture);
							window.draw(wall.shape);
						}
						else {
							floor.shape.setPosition(mazeX, mazeY);
							floor.shape.setTexture(&floorTexture);
							window.draw(floor.shape);

						}
					}
				}
				//Explosion tiles sent to arbitrary wall tile when inactive
				explosion1.shape.setPosition(0, -20);
				explosion2.shape.setPosition(0, -20);
				explosion3.shape.setPosition(0, -20);
				explosion4.shape.setPosition(0, -20);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					player.sprite.move(-1, 0);
					for (int i = 0; i <= 95; i++)
					{
						//General Collision Detect for player collisions
						if ((player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
							|| (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds())))
						{
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

								playerHit = false;
							}
						}

						if (player.sprite.getGlobalBounds().intersects(walls[i]))
						{
							player.sprite.move(1, 0);

						}

						if (player.sprite.getGlobalBounds().intersects(walls[i]))
						{
							player.sprite.move(1, 0);
						}

						if (player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds()))
						{
							player.sprite.move(1, 0);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
						{
							player.sprite.move(1, 0);

						}
						if (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds()))
						{
							player.sprite.move(1, 0);

						}
						if (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds()))
						{
							player.sprite.move(1, 0);

						}

					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{

					player.sprite.move(1, 0);

					for (int i = 0; i < 96; i++)
					{
						//General Collision Detect for player collisions
						if ((player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
							|| (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds())))
						{
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

								playerHit = false;
							}
						}

						if (player.sprite.getGlobalBounds().intersects(walls[i]))
						{
							player.sprite.move(-1, 0);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds()))
						{
							player.sprite.move(-1, 0);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
						{
							player.sprite.move(-1, 0);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds()))
						{
							player.sprite.move(-1, 0);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds()))
						{
							player.sprite.move(-1, 0);
						}
					}
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					player.sprite.move(0, -1);
					for (int i = 0; i <= 95; i++)
					{
						//General Collision Detect for player collisions
						if ((player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
							|| (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds())))
						{
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

								playerHit = false;
							}
						}

						if (player.sprite.getGlobalBounds().intersects(walls[i]))
						{
							player.sprite.move(0, 1);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, 1);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, 1);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, 1);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, 1);
						}



					}


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{


					player.sprite.move(0, 1);
					for (int i = 0; i <= 95; i++)
					{
						//General Collision Detect for player collisions
						if ((player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
							|| (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds())))
						{
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

							if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

								playerHit = false;
							}
						}

						if (player.sprite.getGlobalBounds().intersects(walls[i]))
						{
							player.sprite.move(0, -1);

						}
						if (player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, -1);

						}
						if (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, -1);
						}
						if (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, -1);

						}
						if (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds()))
						{
							player.sprite.move(0, -1);
						}

					}

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					//General Collision Detect for player collisions
					if ((player.sprite.getGlobalBounds().intersects(enemy1.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy2.sprite.getGlobalBounds()))
						|| (player.sprite.getGlobalBounds().intersects(enemy3.sprite.getGlobalBounds())) || (player.sprite.getGlobalBounds().intersects(enemy4.sprite.getGlobalBounds())))
					{
						if (playerHit == false) {
							invulnerabilityTimer.restart();
							playerLives--;
							playerHit = true;
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
					}

					fuse.restart();
					bombSet = true;

					bomb.shape.setTexture(&bombTexture);
					bomb.shape.setPosition(player.sprite.getPosition());
				}

				//If bomb is set and 3 seconds passed, bomb explodes
				if ((bombSet == true) && (fuse.getElapsedTime() > threeSeconds)) {

					if (fuse.getElapsedTime() < explosionSec) {
						explosion1.shape.setPosition(bomb.shape.getPosition());
						explosion2.shape.setPosition(bomb.shape.getPosition());
						explosion3.shape.setPosition(bomb.shape.getPosition());
						explosion4.shape.setPosition(bomb.shape.getPosition());

						explosion1.shape.move(16.f, 0.f);
						explosion2.shape.move(-16.f, 0.f);
						explosion3.shape.move(0.f, 16.f);
						explosion4.shape.move(0.f, -16.f);

						explosion1.shape.setTexture(&explosionTexture);
						explosion2.shape.setTexture(&explosionTexture);
						explosion3.shape.setTexture(&explosionTexture);
						explosion4.shape.setTexture(&explosionTexture);
						window.draw(explosion1.shape);
						window.draw(explosion2.shape);
						window.draw(explosion3.shape);
						window.draw(explosion4.shape);
					}
					if (fuse.getElapsedTime() > explosionSec) {
						bomb.shape.setPosition(0, 0);
						bombSet = false;

					}

				}




				//Enemy1 Movement
				if (enemy1Direction == 0) {
					enemy1.sprite.move(0, 0.2);
					for (int i = 0; i <= 95; i++) {
						if (enemy1.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy1Direction = 1;
						}

						if (enemy1.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {


							enemy1Direction = 1;

							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}

						if (enemy1.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy1Direction = 1;
						}


					}
				}

				if (enemy1Direction == 1) {
					enemy1.sprite.move(0, -0.2);
					for (int i = 0; i <= 95; i++) {
						if (enemy1.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy1Direction = 0;
						}
						if (enemy1.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
							enemy1Direction = 0;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
						if (enemy1.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy1Direction = 0;
						}


					}
				}

				if (enemy1.sprite.getGlobalBounds().intersects(explosion1.shape.getGlobalBounds()))
				{
					enemy1.kill();
					score++;
				}
				if (enemy1.sprite.getGlobalBounds().intersects(explosion2.shape.getGlobalBounds()))
				{
					enemy1.kill();
					score++;
				}
				if (enemy1.sprite.getGlobalBounds().intersects(explosion3.shape.getGlobalBounds()))
				{
					enemy1.kill();
					score++;
				}
				if (enemy1.sprite.getGlobalBounds().intersects(explosion4.shape.getGlobalBounds()))
				{
					enemy1.kill();
					score++;
				}



				//Enemy 2 Movement
				if (enemy2Direction == 0) {
					enemy2.sprite.move(0.2, 0);
					for (int i = 0; i <= 95; i++) {
						if (enemy2.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy2Direction = 1;
						}
						if (enemy2.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
						{
							enemy2Direction = 1;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}

						if (enemy2.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy2Direction = 1;
						}

					}
				}

				if (enemy2Direction == 1) {
					enemy2.sprite.move(-0.2, 0);
					for (int i = 0; i <= 95; i++) {
						if (enemy2.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy2Direction = 0;
						}
						if (enemy2.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
							enemy2Direction = 0;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
						if (enemy2.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy2Direction = 0;
						}
					}
				}

				if (enemy2.sprite.getGlobalBounds().intersects(explosion1.shape.getGlobalBounds()))
				{
					enemy2.kill();
					score++;
				}
				if (enemy2.sprite.getGlobalBounds().intersects(explosion2.shape.getGlobalBounds()))
				{
					enemy2.kill();
					score++;
				}
				if (enemy2.sprite.getGlobalBounds().intersects(explosion3.shape.getGlobalBounds()))
				{
					enemy2.kill();
					score++;
				}
				if (enemy2.sprite.getGlobalBounds().intersects(explosion4.shape.getGlobalBounds()))
				{
					enemy2.kill();
					score++;
				}

				//Enemy 3 Movement
				if (enemy3Direction == 0) {
					enemy3.sprite.move(0.2, 0);
					for (int i = 0; i <= 95; i++) {
						if (enemy3.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy3Direction = 1;
						}
						if (enemy3.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
						{
							enemy3Direction = 1;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
						if (enemy3.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy3Direction = 1;
						}
					}
				}

				if (enemy3Direction == 1) {
					enemy3.sprite.move(-0.2, 0);
					for (int i = 0; i <= 95; i++) {
						if (enemy3.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy3Direction = 0;
						}
						if (enemy3.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
							enemy3Direction = 0;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}

						if (enemy3.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy3Direction = 0;
						}
					}
				}

				if (enemy3.sprite.getGlobalBounds().intersects(explosion1.shape.getGlobalBounds()))
				{
					enemy3.kill();
					score++;
				}
				if (enemy3.sprite.getGlobalBounds().intersects(explosion2.shape.getGlobalBounds()))
				{
					enemy3.kill();
					score++;
				}
				if (enemy3.sprite.getGlobalBounds().intersects(explosion3.shape.getGlobalBounds()))
				{
					enemy3.kill();
					score++;
				}
				if (enemy3.sprite.getGlobalBounds().intersects(explosion4.shape.getGlobalBounds()))
				{
					enemy3.kill();
					score++;
				}

				//Enemy 4 Movement
				if (enemy4Direction == 0) {
					enemy4.sprite.move(0, 0.2);
					for (int i = 0; i <= 95; i++) {
						if (enemy4.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy4Direction = 1;
						}
						if (enemy4.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds()))
						{
							enemy4Direction = 1;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
						if (enemy4.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy4Direction = 1;
						}
					}
				}

				if (enemy4Direction == 1) {
					enemy4.sprite.move(0, -0.2);
					for (int i = 0; i <= 95; i++) {
						if (enemy4.sprite.getGlobalBounds().intersects(walls[i])) {
							enemy4Direction = 0;
						}
						if (enemy4.sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
							enemy4Direction = 0;
							if (playerHit == false) {
								invulnerabilityTimer.restart();
								playerLives--;
								playerHit = true;
							}
						}
						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() < invulnerabilityDuration)) {}

						if ((playerHit == true) && (invulnerabilityTimer.getElapsedTime() > invulnerabilityDuration)) {

							playerHit = false;
						}
						if (enemy4.sprite.getGlobalBounds().intersects(bomb.shape.getGlobalBounds())) {
							enemy4Direction = 0;
						}
					}
				}
				if (enemy4.sprite.getGlobalBounds().intersects(explosion1.shape.getGlobalBounds()))
				{
					enemy4.kill();
					score++;
				}
				if (enemy4.sprite.getGlobalBounds().intersects(explosion2.shape.getGlobalBounds()))
				{
					enemy4.kill();
					score++;
				}
				if (enemy4.sprite.getGlobalBounds().intersects(explosion3.shape.getGlobalBounds()))
				{
					enemy4.kill();
					score++;
				}
				if (enemy4.sprite.getGlobalBounds().intersects(explosion4.shape.getGlobalBounds()))
				{
					enemy4.kill();
					score++;
				}

				enemy1.sprite.setTexture(enemyTexture);
				enemy2.sprite.setTexture(enemyTexture);
				enemy3.sprite.setTexture(enemyTexture);
				enemy4.sprite.setTexture(enemyTexture);
				player.sprite.setTexture(playerTexture);

				if (bombSet == true) {
					window.draw(bomb.shape);
				}

				window.draw(player.sprite);
				if (enemy1.getKilledState() == false) {
					window.draw(enemy1.sprite);
				}
				if (enemy2.getKilledState() == false) {
					window.draw(enemy2.sprite);
				}
				if (enemy3.getKilledState() == false) {
					window.draw(enemy3.sprite);
				}
				if (enemy4.getKilledState() == false) {
					window.draw(enemy4.sprite);
				}
				window.draw(lives);
				window.draw(scores);

				window.display();
				timeSinceLastUpdate = sf::Time::Zero;

			}
		}
		
		if (winCondition == true) {
			window.clear();
			endGame.setString("You Win");
			window.draw(endGame);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				window.close();
			}
		}

		if (loseCondition == true) {
			window.clear();
			endGame.setString("You Lose");
			window.draw(endGame);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				window.close();
			}
		}

	}

	return 0;
}

