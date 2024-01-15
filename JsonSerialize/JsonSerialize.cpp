#include "JsonSerializer.h"

void JsonSerializer::AddString(char** result, size_t& size, size_t& pos, const std::string& addString)
{
	if (result)
	{
		if (*result)
		{
			if (size <= pos + addString.length())
			{
				char* p = (char*)realloc(*result, size * 2 + 1);
				assert(p);
				if (p)
				{
					*result = p;
					size *= 2;
				}
			}
			memcpy(&(*result)[pos], addString.c_str(), addString.length());
			pos += addString.length();
			(*result)[pos] = 0;
		}
	}
}

std::string JsonSerializer::FormatJson(const std::string& target)
{
	if (target.empty())
		return "";

	size_t pos = 0;
	size_t sizeOrigin = target.size();
	size_t size = sizeOrigin;
	char* result = (char*)malloc(size + 1);

	char currentChar;
	int level = 0, quote = 0;
	bool tailSwitchLine = ('\n' == target[sizeOrigin - 1]) ? true : false;
	for (std::string::size_type i = 0; i < sizeOrigin; i++)
	{
		currentChar = target[i];

		if (level > 0 && tailSwitchLine)
		{
			AddString(&result, size, pos, GetLevelString(level));
		}

		switch (currentChar)
		{
		case '{':
		case '[':
			if (quote & 0x01)
				AddString(&result, size, pos, std::string(1, currentChar));
			else
			{
				std::string addString = std::string(1, currentChar) + "\n";
				AddString(&result, size, pos, addString);
				level++;
				AddString(&result, size, pos, GetLevelString(level));
			}
			break;
		case ',':
			if (quote & 0x01)
				AddString(&result, size, pos, std::string(1, currentChar));
			else
			{
				std::string addString = std::string(1, currentChar) + "\n";
				AddString(&result, size, pos, addString);
				AddString(&result, size, pos, GetLevelString(level));
			}
			break;
		case '}':
		case ']':
			if (quote & 0x01)
				AddString(&result, size, pos, std::string(1, currentChar));
			else {
				std::string addString = "\n";
				AddString(&result, size, pos, addString);
				level--;
				AddString(&result, size, pos, GetLevelString(level));
				AddString(&result, size, pos, std::string(1, currentChar));
			}
			break;
		case '\"':
			quote++;
		default:
			AddString(&result, size, pos, std::string(1, currentChar));
			break;
		}

	}
	std::string json = result;

	free(result);

	return json;
}

std::string JsonSerializer::GetLevelString(int level)
{
	std::string levelString = "";
	for (int i = 0; i < level; i++)
	{
		levelString += "\t"; //这里可以\t换成你所需要缩进的空格数
	}
	return levelString;

}