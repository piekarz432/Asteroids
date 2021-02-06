#pragma once
#include"pch.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include<fstream>

#define MAX_NUMBER_OF_ITEMS 2

using namespace sf;
using namespace std;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;



class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
	std::vector<IntRect> frames;

	Animation() {}

	Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
		Frame = 0;
		speed = Speed;

		for (int i = 0; i < count; i++)
			frames.push_back(IntRect(x + i * w, y, w, h));

		sprite.setTexture(t);
		sprite.setOrigin(w / 2, h / 2);
		sprite.setTextureRect(frames[0]);
	}


	void update()
	{
		Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n > 0) sprite.setTextureRect(frames[int(Frame)]);
	}

	bool isEnd()
	{
		return Frame + speed >= frames.size();
	}

};


class Entity
{
public:
	float x, y, dx, dy, R, angle;
	bool life;
	int health;
	int punkty;
	std::string name;
	Animation anim;
	Sprite tab[3];
	int i = 0;

	Entity()
	{
		life = 1;
	}

	void settings(Animation &a, int X, int Y, float Angle = 0, int radius = 1)
	{
		anim = a;
		x = X; y = Y;
		angle = Angle;
		R = radius;
	}

	virtual void update() {};

	void draw(RenderWindow &app)
	{
		anim.sprite.setPosition(x, y);
		anim.sprite.setRotation(angle + 90);
		app.draw(anim.sprite);

		CircleShape circle(R);
		circle.setFillColor(Color(255, 0, 0, 170));
		circle.setPosition(x, y);
		circle.setOrigin(R, R);
		//app.draw(circle);
	}

	virtual ~Entity() {};

	void show_health(RenderWindow &window)
	{
		
		Texture texture;
		texture.loadFromFile("images/ship.png");

		tab[0].setTexture(texture);
		tab[0].setPosition(10, 10);

		tab[1].setTexture(texture);
		tab[1].setPosition(60, 10);

		tab[2].setTexture(texture);
		tab[2].setPosition(110, 10);

		for (int i = 0; i < 3; i++)
		{
			window.draw(tab[i]);
		}
	}
};


class asteroid : public Entity
{
public:
	asteroid()
	{
		dx = rand() % 8 - 4;
		dy = rand() % 8 - 4;
		name = "asteroid";
		punkty = rand() % 10;
	}

	void update()
	{
		x += dx;
		y += dy;

		if (x > W) x = 0;  if (x < 0) x = W;
		if (y > H) y = 0;  if (y < 0) y = H;


	}
};


class bullet : public Entity
{
public:
	bullet()
	{
		name = "bullet";
	}

	void update()
	{
		dx = cos(angle*DEGTORAD) * 6;
		dy = sin(angle*DEGTORAD) * 6;
		 angle+=rand()%7-3;  /*try this*/
		x += dx;
		y += dy;

		if (x > W || x<0 || y>H || y < 0) life = 0;
	}

};


class player : public Entity
{
public:
	
	bool thrust;

	player()
	{
		name = "player";
		health = 3;
	}

	void update()
	{
		std::cout << "GRACZ!!!!!!!!" << endl;
		if (thrust)
		{
			dx += cos(angle*DEGTORAD)*0.2;
			dy += sin(angle*DEGTORAD)*0.2;
		}
		else
		{
			dx *= 0.99;
			dy *= 0.99;
		}

		int maxSpeed = 15;
		float speed = sqrt(dx*dx + dy * dy);
		if (speed > maxSpeed)
		{
			dx *= maxSpeed / speed;
			dy *= maxSpeed / speed;
		}

		x += dx;
		y += dy;

		if (x > W) x = 0; if (x < 0) x = W;
		if (y > H) y = 0; if (y < 0) y = H;
	}

};


class Menu
{
public:
	Menu(float width, float height)
	{
		if (!font.loadFromFile("Fonts/Kenney_Future_Narrow.ttf"))
		{
			std::cout << "B³ad dodawania do klasy Menu"<<endl;
		}
		if (!texture.loadFromFile("PIA17016_hires.jpg"))
		{
			//error
		}


		menu[0].setFont(font);
		menu[0].setFillColor(sf::Color::Blue);
		menu[0].setString("Play");
		menu[0].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

		menu[1].setFont(font);
		menu[1].setFillColor(sf::Color::White);
		menu[1].setString("Exit");
		menu[1].setPosition(sf::Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

		selectedItemIndex = 0;

		background.setTexture(texture);
		background.setScale(sf::Vector2f(1.0f, 1.2f));
	}
	~Menu()
	{

	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(background);
		for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
		{
			window.draw(menu[i]);
		}
	}
	void MoveUp()
	{
		if (selectedItemIndex - 1 >= 0)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			menu[selectedItemIndex].setFillColor(sf::Color::Blue);
		}
	}
	void MoveDown()
	{
		if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			menu[selectedItemIndex].setFillColor(sf::Color::Blue);
		}
	}
	int GetPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Texture texture;
	sf::Sprite background;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];

};


class Save_Load
{
public:
	string linia;
	Save_Load(float width, float height)
	{
		if (!font.loadFromFile("Fonts/Kenney Future Narrow.ttf"))
		{
			std::cout << "B³ad dodawania do klasy Save_Load";
		}
		for (int i = 0; i < 10; i++)
		{
			tab[i].setFont(font);
			tab[i].setFont(font);
			tab[i].setFillColor(sf::Color::Red);
			tab[i].setPosition(sf::Vector2f(width / 2, height / 10));
		}

	}

	void Save(int punkty)
	{
		std::fstream plik;
		plik.open("punkty.txt", std::ios::in | std::ios::out);
		if (plik.good() == true)
		{
			std::cout << "Uzyskano dostep do pliku!" << std::endl;
			plik<<punkty << endl;
			plik.close();
		}
		else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;
	}

	string Load()
	{
		fstream plik;
		string linia;

		plik.open("punkty.txt", ios::in);
		do
		{
			getline(plik, linia); //pobierz linijkê
			return linia;
			//cout << linia; //wypisz na ekranie
		} while (linia != "");

		plik.close();

	}

private:
	Text tab[10];
	Text text;
	Font font;

};


bool isCollide(Entity *a, Entity *b)
{
	return (b->x - a->x)*(b->x - a->x) +
		(b->y - a->y)*(b->y - a->y) <
		(a->R + b->R)*(a->R + b->R);
}





