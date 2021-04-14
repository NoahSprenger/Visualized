#include "Platform/Platform.hpp"
#include "TASKS.CPP"
#include "TASKS.H"
#include "physics.h"
#include "sqlite3.h"

// ! Some ugly global variables ! //
// Box2d world for physics
b2World world(b2Vec2(0.0, 9.8));
// Title and font
sf::Text title;
sf::Font arial;
// list_base as a rectangle shape
sf::RectangleShape list_base_rec;
// strings to handle user input
std::string holding;
std::string holder;
// sf::Text to show what is being typed
sf::Text typing;
// create instance of Tasks
Tasks tasks;
// create instance of Block
Block circ;
std::vector<Block> circ_vec;
// for telling if the box needs to be filled when the program starts
bool first_time = true;

void init(void);
void init()
{
	// arial font
	arial.loadFromFile("Resources/Fonts/arial.ttf");
	// title
	title.setFont(arial);
	title.setFillColor(sf::Color::White);
	title.setCharacterSize(30.0);
	title.setString("(Tasks)");
	title.setPosition(250, 60);
	// base
	list_base_rec.setFillColor(sf::Color::White);
	list_base_rec.setPosition(50, 100);
	list_base_rec.setSize(sf::Vector2f(500, 500));
	// typing text
	typing.setCharacterSize(25.0);
	typing.setPosition(0, 0);
	typing.setFillColor(sf::Color::Magenta);
	typing.setFont(arial);
}

// decides how much the box is filled
int fill_box(void);
int fill_box()
{
	tasks.create_task_text();
	int box_area = 180000;
	double tasks_completed;
	tasks_completed = Tasks::tasks_completed;
	double tasks_total = Tasks::m_string_vec.size();
	double area_to_fill = (box_area) * (tasks_completed / tasks_total);
	int filled = area_to_fill / (13.5 * 13.5 * 3.14);
	for (int i = 0; i < filled; i++)
	{
		circ = physics::createCircle(world, 885, 500 - (i), 13.5);
		circ_vec.push_back(circ);
	}
	return 0;
}

void update_input(sf::Window& window);
void update_input(sf::Window& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Enter)
				{
					tasks.create_task(holder);
					holder = "";
					typing.setString("");
					if (first_time)
					{
						fill_box();
						first_time = false;
					}
					else
					{
						for (unsigned int i = 0; i < circ_vec.size(); i++)
						{
							physics::deleteBlock(world, circ_vec[i]);
						}
						circ_vec.clear();
						fill_box();
					}
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::TextEntered:
				try
				{
					if (event.text.unicode == '\b')
					{
						holder.erase(holder.size() - 1, 1);
					}
					else if (event.text.unicode < 128)
					{
						holder += static_cast<char>(event.text.unicode);
					}
					typing.setString(holder);
				}
				// catches the error when you try and backspace when there is not text
				catch (const std::out_of_range&)
				{
				}
			default:
				break;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// handles buttons for task rectangles
			for (unsigned int i = 0; i < Tasks::tasks_rec.size(); i++)
			{
				if (sf::Mouse::getPosition(window).x >= Tasks::tasks_rec[i].getPosition().x and sf::Mouse::getPosition(window).y >= Tasks::tasks_rec[i].getPosition().y and sf::Mouse::getPosition(window).x <= Tasks::tasks_rec[i].getPosition().x + Tasks::tasks_rec[i].getSize().x and sf::Mouse::getPosition(window).y <= Tasks::tasks_rec[i].getPosition().y + Tasks::tasks_rec[i].getSize().y)
				{
					Tasks::tasks_rec[i].setFillColor(sf::Color::Green);
					// make complete task function work
					tasks.complete_task(i);
					if (first_time)
					{
						fill_box();
						first_time = false;
					}
					else
					{
						// deletes the existing circles
						for (unsigned int i = 0; i < circ_vec.size(); i++)
						{
							physics::deleteBlock(world, circ_vec[i]);
						}
						circ_vec.clear();
						fill_box();
					}
				}
			}
			// handles buttons for delete rectangles
			for (unsigned int i = 0; i < Tasks::delete_rec.size(); i++)
			{
				if (sf::Mouse::getPosition(window).x >= Tasks::delete_rec[i].getPosition().x and sf::Mouse::getPosition(window).y >= Tasks::delete_rec[i].getPosition().y and sf::Mouse::getPosition(window).x <= Tasks::delete_rec[i].getPosition().x + Tasks::delete_rec[i].getSize().x and sf::Mouse::getPosition(window).y <= Tasks::delete_rec[i].getPosition().y + Tasks::delete_rec[i].getSize().y)
				{
					tasks.delete_task(i);
					if (first_time)
					{
						fill_box();
						first_time = false;
					}
					else
					{
						// deletes the existing circles
						for (unsigned int i = 0; i < circ_vec.size(); i++)
						{
							physics::deleteBlock(world, circ_vec[i]);
						}
						circ_vec.clear();
						fill_box();
					}
				}
			}
		}
	}
}

// for drawing sfml objects
void draw_sfml(sf::RenderWindow& window);
void draw_sfml(sf::RenderWindow& window)
{
	window.draw(title);
	window.draw(list_base_rec);
	window.draw(typing);
	// serves as a refrence for how to executing drawing the create task text vector of sf::Texts
	for (unsigned int i = 0; i < Tasks::m_text_vec.size(); i++)
	{
		window.draw(Tasks::m_text_vec[i]);
	}
	for (unsigned int i = 0; i < Tasks::tasks_rec.size(); i++)
	{
		window.draw(Tasks::tasks_rec[i]);
	}
	for (unsigned int i = 0; i < Tasks::delete_rec.size(); i++)
	{
		window.draw(Tasks::delete_rec[i]);
	}
	// fixes the bug that causes the task to multiply by all existing tasks
	Tasks::m_string_vec.clear();
}

int main()
{
	// SFML window
	sf::RenderWindow window(sf::VideoMode(1200, 675), "Visualized", sf::Style::Titlebar | sf::Style::Close);
	util::Platform platform;
	tasks.create_task_database();
	tasks.initalize_database();
	tasks.new_database();
	tasks.create_task_text();
	// fills the box on startup
	if (first_time)
	{
		fill_box();
		first_time = false;
	}
	else
	{
		// deletes the existing circles
		for (unsigned int i = 0; i < circ_vec.size(); i++)
		{
			physics::deleteBlock(world, circ_vec[i]);
		}
		circ_vec.clear();
		fill_box();
	}
	init();
	platform.setIcon(window.getSystemHandle());
	window.setFramerateLimit(60); //sets the game loop to run 60 times per second
	// box for box2d balls to fill
	physics::createBox(world, 650, 100, 20, 500, b2_staticBody);
	physics::createBox(world, 1100, 100, 20, 500, b2_staticBody);
	physics::createBox(world, 650, 580, 450, 20, b2_staticBody);

	while (window.isOpen() && !sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) //the main game loop, exits if someone closes the window
	{
		window.clear(sf::Color::Black); //clears the screen
		draw_sfml(window);
		update_input(window);
		physics::displayWorld(world, window);
		window.display(); //displays everything on the video card to the monitor
	}					  //ends the game loop
	return 0;
}
