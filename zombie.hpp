#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <deque>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <memory>

#ifdef DEBUG
#define INLINE
#else
#define INLINE inline
#endif

//#define forward_token (0)
//#define backward_token (1)
//#define increment_token (2)
//#define decrement_token (3)
//#define output_token (4)
//#define input_token (5)
//#define while_entry_token (6)
//#define while_exit_token (7)

typedef int code_type;
typedef std::map<std::string, code_type> code_map_type;
typedef std::set<char> token_type;

const code_type forward_token = (0);
const code_type backward_token = (1);
const code_type increment_token = (2);
const code_type decrement_token = (3);
const code_type output_token = (4);
const code_type input_token = (5);
const code_type while_entry_token = (6);
const code_type while_exit_token = (7);

class compiler_base
{
public:
	compiler_base() { ; }

	virtual bool check_token(char src) = 0;

	virtual bool is_match(char src) = 0;

	virtual code_type get_code(char src) = 0;
};

class binary : public compiler_base
{
private:
	token_type token;
	code_map_type map;
	std::string cache;
public:
	binary(char c0, char c1)
	{
		token.insert(c0);
		token.insert(c1);

		std::string temp = "";
		map[temp + c0 + c0 + c0] = forward_token;
		map[temp + c0 + c0 + c1] = backward_token;
		map[temp + c0 + c1 + c0] = increment_token;
		map[temp + c0 + c1 + c1] = decrement_token;
		map[temp + c1 + c0 + c0] = output_token;
		map[temp + c1 + c0 + c1] = input_token;
		map[temp + c1 + c1 + c0] = while_entry_token;
		map[temp + c1 + c1 + c1] = while_exit_token;
	}

	bool check_token(char src)
	{
		return token.end() != token.find(src);
	}

	bool is_match(char src)
	{
		bool result;
		cache += src;
		if (map.end() != map.find(cache))
		{
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}

	code_type get_code(char src)
	{
		code_type result;
		if (map.end() != map.find(cache))
		{
			result = map[cache];
			cache = "";
		}
		else
		{
			result = -1;
		}
		return result;
	}
};

class octonary : public compiler_base
{
private:
	token_type token;
	code_map_type map;
	std::string cache;
public:
	octonary(char c0, char c1, char c2, char c3, char c4, char c5, char c6, char c7)
	{
		token.insert(c0);
		token.insert(c1);
		token.insert(c2);
		token.insert(c3);
		token.insert(c4);
		token.insert(c5);
		token.insert(c6);
		token.insert(c7);

		std::string temp = "";
		map[temp + c0] = forward_token;
		map[temp + c1] = backward_token;
		map[temp + c2] = increment_token;
		map[temp + c3] = decrement_token;
		map[temp + c4] = output_token;
		map[temp + c5] = input_token;
		map[temp + c6] = while_entry_token;
		map[temp + c7] = while_exit_token;
	}

	bool check_token(char src)
	{
		return token.end() != token.find(src);
	}

	bool is_match(char src)
	{
		bool result;
		cache += src;
		if (map.end() != map.find(cache))
		{
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}

	code_type get_code(char src)
	{
		code_type result;
		if (map.end() != map.find(cache))
		{
			result = map[cache];
			cache = "";
		}
		else
		{
			result = -1;
		}
		return result;
	}
};


class brainfuck
{
private:

	typedef std::deque<char> data;
	data data_segment;
	data::iterator data_segment_it;

	typedef std::vector<code_type> buffer;
	buffer code_segment;
	buffer::iterator code_segment_it;

	typedef std::stack<buffer::iterator> stack;
	stack stack_segment;

	typedef std::shared_ptr<compiler_base> compiler_ptr;
	std::list<compiler_ptr> compiler_list;
public:

	brainfuck(compiler_ptr compiler)
	{
	    add_compiler(compiler);
	};

	void add_compiler(compiler_ptr compiler)
	{
	    compiler_list.push_back(compiler);
	}

    void parse(std::string script)
	{
		for (std::string::iterator it = script.begin();
			it != script.end();
			++it)
		{
			for (std::list<compiler_ptr>::iterator compiler_it = compiler_list.begin();
				compiler_it != compiler_list.end();
				++compiler_it)
			{
				if ((*compiler_it)->check_token(*it))
				{
					if ((*compiler_it)->is_match(*it))
					{
						code_segment.push_back((*compiler_it)->get_code(*it));
					}
					break;
				}
			}
		}
	}

	~brainfuck()
	{
		data_segment.clear();
		code_segment.clear();
		while (!stack_segment.empty())
			stack_segment.pop();
	}

public:

	void run()
	{
		data_segment.push_back(0);
		data_segment_it = data_segment.begin();
		for (code_segment_it = code_segment.begin();
			code_segment_it != code_segment.end();
			++code_segment_it)
		{
			function(*code_segment_it);
		}
	}

private:
	INLINE void function(code_type func_step)
	{
		switch (func_step)
		{
		case forward_token:
			forward();
			break;
		case backward_token:
			backward();
			break;
		case increment_token:
			increment();
			break;
		case decrement_token:
			decrement();
			break;
		case output_token:
			output();
			break;
		case input_token:
			input();
			break;
		case while_entry_token:
			while_entry();
			break;
		case while_exit_token:
			while_exit();
			break;
		default:
			break;
		}
	}


	INLINE void forward()
	{
		++data_segment_it;
		if (data_segment_it == data_segment.end())
		{
			data_segment.push_back(0);
			// It's not pretty, here must reset iterator
			data_segment_it = --data_segment.end();
		}
	}

	INLINE void backward()
	{
		if (data_segment_it == data_segment.begin())
		{
			data_segment.push_front(0);
		}
		--data_segment_it;
	}

	INLINE void increment()
	{
		++(*data_segment_it);
	}

	INLINE void decrement()
	{
		--(*data_segment_it);
	}

	INLINE void input()
	{
		*data_segment_it = getchar();
	}

	INLINE void output()
	{
		putchar(*data_segment_it);
	}

	INLINE void while_entry()
	{
		if (0 != *data_segment_it)
		{
			stack_segment.push(code_segment_it - 1);
		}
		else
		{
			int loop_level = 1;
			for (++code_segment_it;
				code_segment_it != code_segment.end() && loop_level /*> 0*/;
				++code_segment_it)
			{
				if (*code_segment_it == while_entry_token)
				{
					loop_level++;
				}
				else if (*code_segment_it == while_exit_token)
				{
					loop_level--;
				}
			}
		}
	}

	INLINE void while_exit()
	{
		if (0 != *data_segment_it)
		{
			code_segment_it = stack_segment.top();
			stack_segment.pop();
		}
	}
};
