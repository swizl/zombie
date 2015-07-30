
#include "zombie.hpp"

int main(int argc, char* argv[])
{
	const std::string file_flag = "-f"; // source file
	const std::string string_flag = "-s"; // source string
	auto bf = std::make_shared<brainfuck>(std::make_shared<octonary>('>', '<', '+', '-', '.', ',', '[', ']'));

	//bf->add_compiler(std::make_shared<octonary>('0', '1', '2', '3', '4', '5', '6', '7'));
	//bf->add_compiler(std::make_shared<octonary>('8', '9', 'A', 'B', 'C', 'D', 'E', 'F'));
	//bf->add_compiler(std::make_shared<binary>('*', '#'));
	//bf->add_compiler(std::make_shared<binary>('!', '?'));

	if (argc == 3)
	{
		std::string argv1 = argv[1];
		std::string argv2;
		if (0 == file_flag.compare(argv1))
		{
			std::string file_name = argv[2];
			std::ifstream source(file_name);
			if (!source)
				return -1;
			std::stringstream buffer;
			buffer << source.rdbuf();
			argv2 = buffer.str();
		}
		else if (0 == string_flag.compare(argv1))
		{
			argv2 = argv[2];
		}
		if (!argv2.empty())
			bf->parse(argv2);
	}

	if (bf)
	{
		bf->run();
	}
	//system("pause");
	return 0;
}
