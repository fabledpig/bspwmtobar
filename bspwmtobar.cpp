#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

typedef struct
{
	bool active;
	bool hasWindow;
	std::string name;
} Desktop;

typedef struct
{
	char pos;
	char prefix;
	std::string text;
} Element;

std::vector<std::string> splitStringIntoTokens(std::string &str, char c);
std::size_t countDesktops(std::vector<std::string> &tokens);
Desktop* getDesktops(std::vector<std::string> &tokens, std::size_t n);
void printElements(std::vector<Element>& elements);
void eraseStringTillCharacter(std::string &str, char c);

std::string colorActive = "#FFFF0000";
std::string colorHasWindow = "#FFFFF000";
std::string colorNormal = "#FFFFFFFF";

//syntax bspwmtobar -l x -c W -r c --color-active $color1 --color-haswindow $color2 --color-normal $color3
int main(int argc, char * argv[])
{
	char pos = 'l';
	std::vector<Element> elements;

	for(int i = 1; i < argc; i++)
	{
		std::string strargv = argv[i];
		if(strargv == "-l" || strargv == "-c" || strargv == "-r")
			pos = strargv[1];
		else if(strargv == "--color-active")
		{
			i++;

			if(i >= argc)
			{
				std::cerr << "Error! Not enough argument!";
				return -1;
			}

			colorActive = argv[i];
		}
		else if(strargv == "--color-haswindow")
		{
			i++;

			if(i >= argc)
			{
				std::cerr << "Error! Not enough argument!";
				return -1;
			}

			colorHasWindow = argv[i];
		}
		else if(strargv == "--color-normal")
		{
			i++;

			if(i >= argc)
			{
				std::cerr << "Error! Not enough argument!";
				return -1;
			}

			colorNormal = argv[i];
		}
		else
		{
			Element element;
			element.pos = pos;
			element.prefix = strargv[0];

			elements.push_back(element);
		}
	}

	std::ifstream fifo("/tmp/bspwm-fifo", std::ifstream::in);
	std::string buffer;

	while(std::getline(fifo, buffer))
	{
		std::stringstream textss;
		char prefix = buffer[0];

		if(prefix == 'W')
		{
			std::vector<std::string> tokens = splitStringIntoTokens(buffer, ':');

			std::size_t n = countDesktops(tokens);
			Desktop* desktops = getDesktops(tokens, n);

			//char layout = tokens[n + 1][1];

			for(unsigned int i = 0; i < n; i++)
			{
				std::string color;

				if(desktops[i].active)
					color = colorActive;
				else if(desktops[i].hasWindow)
					color = colorHasWindow;
				else
					color = colorNormal;

				textss << "%{F" << color << "}" << desktops[i].name << "%{F-} ";
			}
		}
		else
		{
			eraseStringTillCharacter(buffer, ':');

			textss << buffer;
		}

		for(Element &element : elements)
			if(element.prefix == prefix)
				element.text = textss.str();

		printElements(elements);
	}

	fifo.close();

	return 0;
}

void printElements(std::vector<Element>& elements)
{
	std::stringstream textss;
	textss << "%{l}";

	for(Element &element : elements)
		if(element.pos == 'l')
			textss << element.text << " ";

	textss << "%{c}";

	for(Element &element : elements)
		if(element.pos == 'c')
			textss << element.text << " ";

	textss << "%{r}";

	for(Element &element : elements)
		if(element.pos == 'r')
			textss << element.text << " ";

	std::string text = textss.str();

	if(text[text.size() - 1] == ' ')
		text.pop_back();

	std::cout << text << std::endl;
}

Desktop* getDesktops(std::vector<std::string> &tokens, std::size_t n)
{
	Desktop* desktops = new Desktop[n];

	for(unsigned int i = 0; i < n; i++)
	{
		switch(tokens[i + 1][0])
		{
			case 'o':
				desktops[i].active = false;
				desktops[i].hasWindow = true;
				break;
			case 'O':
				desktops[i].active = true;
				desktops[i].hasWindow = true;
				break;
			case 'f':
				desktops[i].active = false;
				desktops[i].hasWindow = false;
				break;
			case 'F':
				desktops[i].active = true;
				desktops[i].hasWindow = false;
				break;
		}

		desktops[i].name = tokens[i + 1].substr(1, tokens[i + 1].length() - 1);
	}

	return desktops;
}

std::size_t countDesktops(std::vector<std::string> &tokens)
{
	std::size_t n = 0;

	for(std::string &token : tokens)
	{
		switch(token[0])
		{
			case 'o':
			case 'O':
			case 'f':
			case 'F':
				n++;
				break;
		}
	}

	return n;
}

void eraseStringTillCharacter(std::string &str, char c)
{
	std::size_t pos;

	if((pos = str.find(c)) != std::string::npos)
		str.erase(0, pos + 1);
}

std::vector<std::string> splitStringIntoTokens(std::string &str, char c)
{
	std::vector<std::string> tokens;
	std::size_t pos = -1;
	std::size_t newPos;

	while((newPos = str.find(c, pos + 1)) != std::string::npos)
	{
		std::string token = str.substr(pos + 1, newPos - pos - 1);

		if(token.length() > 0)
			tokens.push_back(token);

		pos = newPos;
	}

	if(pos != str.length() - 1)
	{
		std::string token = str.substr(pos + 1, pos - str.length() - 1);

		tokens.push_back(token);
	}

	return tokens;
}
