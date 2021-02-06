#include"pch.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include"Game.h"
#include"TextBox.h"
#include<fstream>
#include<Windows.h>
#include<list>
using namespace std;

const int Width = 1200;
const int Height = 800;


void GameOver(sf::Vector2i Window_pos, int punkty)
{
	sf::RenderWindow window;
	Save_Load save(window.getSize().x, window.getSize().y);
	string points=save.Load();


	sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 445, (sf::VideoMode::getDesktopMode().height / 2) - 480);

	window.create(sf::VideoMode(400, 250), "Game Over", sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(Window_pos + centerWindow+sf::Vector2i(0, 250));

	window.setKeyRepeatEnabled(true);

	

	sf::Font font;
	if (!font.loadFromFile("Fonts/Kenney Future Narrow.ttf"))
	{
		std::cout << "Bład dodawania do GameOver";
	}

	sf::Text text;

	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(25);
	text.setString("Podaj nazwe gracza: ");
	text.setPosition(100,10);

	sf::Text text2;
	text2.setFont(font);
	text2.setFillColor(sf::Color::White);
	text2.setCharacterSize(25);
	text2.setString("Twój wynik: \n           " + to_string(punkty));
	text2.setPosition(20,110);

	sf::Text text3;
	text3.setFont(font);
	text3.setFillColor(sf::Color::White);
	text3.setCharacterSize(25);
	text3.setString("Najlepszy wynik: \n               " + points);
	text3.setPosition(230, 110);


	Textbox text1(20, sf::Color::White, true);
	text1.setPosition({100, 50 });
	text1.setLimit(true, 30);
	text1.setFont(font);

	Button btn1("Zapisz", {100, 35}, 25, sf::Color::Blue, sf::Color::White);
	btn1.setFont(font);
	btn1.setPosition({ 150, 200 });




	while (window.isOpen()) {

		sf::Event Event;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			text1.setSelected(true);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			text1.setSelected(false);
		}

		//Event Loop:
		while (window.pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::Closed:
				window.close();
			case sf::Event::TextEntered:
				text1.typedOn(Event);
				case sf::Event::MouseMoved:
					if (btn1.isMouseOver(window)) {
						btn1.setBackColor(sf::Color::Red);
					}
					else {
						btn1.setBackColor(sf::Color::Blue);
					}
					break;
				case sf::Event::MouseButtonPressed:
					if (btn1.isMouseOver(window)) {
						//std::cout << text1.getText() << " : " << punkty << endl;
						
						if (punkty>=stoi(points))
						{
							save.Save(punkty);
						}
						
						window.close();
					}
				
			}

		}
		window.clear();
		text1.drawTo(window);
		window.draw(text);
		window.draw(text2);
		window.draw(text3);
		btn1.drawTo(window);
		window.display();
	}
}

void Create_Game_Window()
{
	srand(time(0));

	RenderWindow app(VideoMode(W, H), "Asteroids!");
	app.setFramerateLimit(60);
	

	Texture t1, t2, t3, t4, t5, t6, t7;
	t1.loadFromFile("images/spaceship.png");
	t2.loadFromFile("images/background.jpg");
	t3.loadFromFile("images/explosions/type_C.png");
	t4.loadFromFile("images/rock.png");
	t5.loadFromFile("images/fire_blue.png");
	t6.loadFromFile("images/rock_small.png");
	t7.loadFromFile("images/explosions/type_B.png");

	t1.setSmooth(true);
	t2.setSmooth(true);


	Sprite background(t2);

	Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
	Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
	Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
	Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
	Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
	Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
	Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);




	std::list<Entity*> entities;


	
	for (int i = 0; i < 15; i++)
	{
		asteroid *a = new asteroid();
		a->settings(sRock, rand() % W, rand() % H, rand() % 360, 25);
		entities.push_back(a);
	}



	player *p = new player();
	p->settings(sPlayer, 200, 200, 0, 20);
	entities.push_back(p);


	int punkty = 0;

	/////main loop/////
	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
				{
					bullet *b = new bullet();
					b->settings(sBullet, p->x, p->y, p->angle, 10);
					entities.push_back(b);
				}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle += 3;
		if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle -= 3;
		if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust = true;
		else p->thrust = false;


		for (auto a : entities)
			for (auto b : entities)
			{
				if (a->name == "asteroid" && b->name == "bullet")
					if (isCollide(a, b))
					{
						a->life = false;
						b->life = false;
						cout <<"Zniszczono asteroide za :"<< a->punkty << endl;
						punkty += a->punkty;

						Entity *e = new Entity();
						e->settings(sExplosion, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);


						for (int i = 0; i < 2; i++)
						{
							if (a->R == 15) continue;
							Entity *e = new asteroid();
							e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
							entities.push_back(e);
						}

					}

				if (a->name == "player" && b->name == "asteroid")
					if (isCollide(a, b))
					{
						a->health--;
						a->tab[a->i++].setColor(Color::Red);
						cout << a->i << endl;
						b->life = false;
						

						Entity *e = new Entity();
						e->settings(sExplosion_ship, a->x, a->y);
						e->name = "explosion";
						entities.push_back(e);

						p->settings(sPlayer, W / 2, H / 2, 0, 20);
						p->dx = 0; p->dy = 0;
						if (a->health <= 0)
						{
							cout << "Statek zniszczony" << endl;
							GameOver(app.getPosition(),punkty);
							app.close();
						}
					}
			}


		if (p->thrust)  p->anim = sPlayer_go;
		else   p->anim = sPlayer;


		for (auto e : entities)
			if (e->name == "explosion")
				if (e->anim.isEnd()) e->life = 0;

		if (rand() % 150 == 0)
		{
			asteroid *a = new asteroid();
			a->settings(sRock, 0, rand() % H, rand() % 360, 25);
			entities.push_back(a);
		}

		for (auto i = entities.begin(); i != entities.end();)
		{
			Entity *e = *i;

			e->update();
			e->anim.update();

			if (e->life == false) { i = entities.erase(i); delete e; }
			else i++;
		}

		//////draw//////
		app.draw(background);
		for (auto i : entities) i->draw(app);
		p->show_health(app);
		app.display();
	}	
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "Main Menu");
	Menu menu(window.getSize().x, window.getSize().y);
	Save_Load save(window.getSize().x, window.getSize().y);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.MoveUp();
					break;

				case sf::Keyboard::Down:
					menu.MoveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.GetPressedItem())
					{
					case 0:
					{
						std::cout << "Play button has been pressed" << std::endl;
						Create_Game_Window();
					}
					break;
					case 1:
						window.close();
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				window.close();

				break;

			}
		}

		window.clear();
		menu.draw(window);
		//save.draw(window);
		window.display();
	}
}

