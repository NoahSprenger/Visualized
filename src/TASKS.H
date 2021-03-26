#ifndef TASKS_H
#define TASKS_H

class Tasks
{
public:
	Tasks();
	virtual ~Tasks();
	int create_task_database();
	int initalize_database();
	int create_task(std::string task_holder);
	sf::Text create_task_text();
	static int callback(void* data __attribute__((unused)), int argc, char** argv, char** azColName __attribute__((unused)));
	sf::Text read_task();
	std::string add_task_text(std::string holde);
	static std::string hold;
	static int num_tasks;
	static std::vector<std::string> m_string_vec;
	static std::vector<sf::Text> m_text_vec;
	// task checkboxs
	static std::vector<sf::RectangleShape> tasks_rec;
	// tasks delete box
	static std::vector<sf::RectangleShape> delete_rec;
	// counts how many tasks have been completed
	static int tasks_completed;
	static std::vector<std::string> completed_vec;
	static std::vector<int> tasks_num_vec;
	static std::vector<std::string> delete_vec;
	int new_database();
	// for marking tasks as completed
	int complete_task(int id);
	// for deleting tasks
	int delete_task(int id);
	static int deleted_id;

protected:
private:
	sf::RectangleShape m_task_rec;
	sf::Text m_text;
	sf::Font arial2;
	std::string task_string;
	std::vector<std::string> tasks_strings;
	sf::Texture delete_tex;
};

#endif // TASKS_H
