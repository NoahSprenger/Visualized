#include "TASKS.H"
#include "sqlite3.h"

Tasks::Tasks()
{
	//ctor
}

Tasks::~Tasks()
{
	//dtor
}

int Tasks::create_task_database()
{
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open("tasks.db", &DB);
	if (exit)
	{
		std::cerr << "Error open DB" << sqlite3_errmsg(DB) << std::endl;
		return (-1);
	}
	else
	{
		sqlite3_close(DB);
		return (0);
	}
}

int Tasks::initalize_database()
{
	sqlite3* DB;
	std::string sql = "CREATE TABLE TASKS("
					  "ID INT PRIMARY KEY     NOT NULL, "
					  "TASK           TEXT    NOT NULL, "
					  "DON            TEXT    NOT NULL );";
	int exit = 0;
	exit = sqlite3_open("tasks.db", &DB);
	char* messaggeError;
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Create Table" << std::endl;
		sqlite3_free(messaggeError);
	}
	sqlite3_close(DB);
	return (0);
}

// ! Ugly variables ! //
std::string Tasks::hold;
int Tasks::num_tasks = 0;
int Tasks::tasks_completed = 0;
std::vector<std::string> Tasks::m_string_vec;
// vectors for sfml objects
std::vector<sf::RectangleShape> Tasks::delete_rec;
std::vector<sf::RectangleShape> Tasks::tasks_rec;
// vector that holds whether the task is completed
std::vector<std::string> Tasks::completed_vec;
std::vector<int> Tasks::tasks_num_vec;
std::vector<std::string> Tasks::delete_vec;
int Tasks::deleted_id;

int Tasks::callback(void* data __attribute__((unused)), int argc __attribute__((unused)), char** argv, char** azColName __attribute__((unused)))
{
	int i;
	std::string hold;
	for (i = 0; i < 1; i++)
	{
		Tasks::hold = argv[1];
		Tasks::m_string_vec.push_back(argv[1]);
		Tasks::completed_vec.push_back(argv[2]);
		int holder = std::atoi(argv[0]);
		if (i == 0)
		{
			Tasks::tasks_num_vec.push_back(1);
			if (holder > deleted_id)
			{
				int new_holder = holder - 1;
				std::string holding_string = "UPDATE TASKS SET ID = " + std::to_string(new_holder) + " WHERE ID = " + std::to_string(holder) + ";";
				delete_vec.push_back(holding_string);
			}
		}
		std::string completed = argv[2];
		if (completed == "COMPLETED")
		{
			tasks_completed++;
		}
	}
	return 0;
}

int Tasks::create_task(std::string task_holder)
{
	sqlite3* DB;
	char* messaggeError;
	int exit = sqlite3_open("tasks.db", &DB);
	std::string query = "SELECT * FROM TASKS;";
	int num = Tasks::tasks_num_vec.size();
	std::string sql("INSERT INTO TASKS VALUES(" + std::to_string(num) + ", '" + task_holder + "', 'INCOMPLETED');");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
	}
	Tasks::tasks_num_vec.clear();
	Tasks::m_string_vec.clear();
	Tasks::completed_vec.clear();
	Tasks::tasks_completed = 0;
	Tasks::m_text_vec.clear();
	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
	sqlite3_close(DB);
	return (0);
}

int Tasks::new_database()
{
	sqlite3* DB;
	sqlite3_open("tasks.db", &DB);
	std::string query = "SELECT * FROM TASKS;";
	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
	sqlite3_close(DB);
	return (0);
}

std::string Tasks::add_task_text(std::string holde)
{
	m_text.setString(holde);
	return holde;
}

std::vector<sf::Text> Tasks::m_text_vec;
sf::Text Tasks::create_task_text()
{
	// ! Working ! //
	int pos_x = 70;
	arial2.loadFromFile("Resources/Fonts/arial.ttf");
	delete_tex.loadFromFile("Resources/Textures/delete_button.png");
	unsigned int i;
	try
	{
		for (i = 0; i < Tasks::m_string_vec.size(); i++)
		{
			// for the text
			m_text_vec.push_back(m_text);
			m_text_vec[i].setCharacterSize(25.0);
			m_text_vec[i].setFillColor(sf::Color::Black);
			m_text_vec[i].setFont(arial2);
			m_text_vec[i].setPosition(60, pos_x += 40);
			m_text_vec[i].setString(Tasks::m_string_vec[i]);
			// for the buttons
			tasks_rec.push_back(m_task_rec);
			delete_rec.push_back(m_task_rec);
			if (completed_vec[i] == "COMPLETED")
			{
				tasks_rec[i].setFillColor(sf::Color::Green);
			}
			else
			{
				tasks_rec[i].setFillColor(sf::Color::Red);
			}
			delete_rec[i].setTexture(&delete_tex);
			delete_rec[i].setSize(sf::Vector2f(25, 25));
			delete_rec[i].setPosition(510, pos_x);
			tasks_rec[i].setSize(sf::Vector2f(25, 25));
			tasks_rec[i].setPosition(475, pos_x);
		}
		return m_text;
	}
	catch (std::runtime_error& e)
	{
	}
	return m_text;
}

sf::Text Tasks::read_task()
{
	arial2.loadFromFile("src/fonts/arial.ttf");
	m_text.setCharacterSize(30.0);
	m_text.setFillColor(sf::Color::Red);
	m_text.setPosition(60, 100);
	m_text.setFont(arial2);
	return m_text;
}

// for changing task from incomplete to complete
int Tasks::complete_task(int id)
{
	// open database
	// change incomplete to complete
	// close database
	sqlite3* DB;
	char* messaggeError;
	int exit = sqlite3_open("tasks.db", &DB);
	std::string query = "SELECT * FROM TASKS;";

	std::string update = "UPDATE TASKS SET DON = 'COMPLETED' WHERE ID = " + std::to_string(id) + ";";

	exit = sqlite3_exec(DB, update.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Update" << std::endl;
		sqlite3_free(messaggeError);
	}

	// acts as a reset
	Tasks::tasks_num_vec.clear();
	Tasks::m_string_vec.clear();
	Tasks::completed_vec.clear();
	Tasks::tasks_completed = 0;

	Tasks::m_text_vec.clear();

	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	sqlite3_close(DB);
	return (0);
}

// for deleting a task
int Tasks::delete_task(int id)
{
	// open database
	// delete task
	// close database
	deleted_id = id;
	sqlite3* DB;
	char* messaggeError;
	int exit = sqlite3_open("tasks.db", &DB);
	std::string query = "SELECT * FROM TASKS;";
	// delete task
	std::string delete_sql = "DELETE FROM TASKS WHERE ID = " + std::to_string(id);
	exit = sqlite3_exec(DB, delete_sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Delete" << std::endl;
		sqlite3_free(messaggeError);
	}

	// acts as a reset
	Tasks::delete_vec.clear();
	Tasks::tasks_num_vec.clear();
	Tasks::m_string_vec.clear();
	Tasks::completed_vec.clear();
	Tasks::tasks_completed = 0;
	Tasks::m_text_vec.clear();

	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	// re-update
	// if statement needed to avoid out of range error that results in a segmentation fault
	if (delete_vec.size() > 0)
	{
		for (unsigned int i = 0; i <= delete_vec.size(); i++)
		{
			exit = sqlite3_exec(DB, delete_vec[i].c_str(), NULL, 0, &messaggeError);
			if (exit != SQLITE_OK)
			{
				std::cerr << "Error update" << std::endl;
				sqlite3_free(messaggeError);
			}
		}
	}
	// acts as a reset
	Tasks::delete_vec.clear();
	Tasks::tasks_num_vec.clear();
	Tasks::m_string_vec.clear();
	Tasks::completed_vec.clear();
	Tasks::tasks_completed = 0;
	Tasks::m_text_vec.clear();
	Tasks::delete_rec.clear();
	Tasks::tasks_rec.clear();

	sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);

	sqlite3_close(DB);
	return (0);
}