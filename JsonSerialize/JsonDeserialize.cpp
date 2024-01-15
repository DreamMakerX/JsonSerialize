#include "JsonDeserializer.h"

JsonDeserializer::JsonDeserializer(const char* path)
	:file_path_(path)
{
	std::ifstream json_file(file_path_);

	if (!json_file.is_open())
	{
		std::string err_info = "Json file [" + file_path_ + "] doesn't exist!";
		throw err_info;
	}

	rapidjson::IStreamWrapper wrapper(json_file);

	if (document_.ParseStream(wrapper).HasParseError())
	{
		std::string err_info = "Parse json file [" + file_path_ + "] error!";
		throw err_info;
	}

	value_stack_.push(&document_);
}

JsonDeserializer::JsonDeserializer(const std::string& jsonString)
{
	if (document_.Parse(jsonString.c_str()).HasParseError())
	{
		std::string err_info = "Parse json string error!";
		throw err_info;
	}

	value_stack_.push(&document_);
}

bool JsonDeserializer::IsUtf8Encoding(const std::string& s)
{
	int count = 0;
	unsigned char single;
	bool allAscii = true;
	unsigned long len = s.size();

	//0x00-0x7F为ASCII码范围
	for (unsigned long uIndex = 0; uIndex < len; ++uIndex)
	{
		single = s[uIndex];

		if ((single & 0x80) != 0)
			allAscii = false;

		if (count == 0)
		{
			if (single >= 0x80)
			{
				if (single >= 0xFC && single <= 0xFD)
					count = 6;
				else if (single >= 0xF8)
					count = 5;
				else if (single >= 0xF0)
					count = 4;
				else if (single >= 0xE0)
					count = 3;
				else if (single >= 0xC0)
					count = 2;
				else
					return false;

				count--;
			}
		}
		else
		{
			if ((single & 0xC0) != 0x80)//在UTF-8中，以位模式10开始的所有字节是多字节序列的后续字节
				return false;

			count--;
		}
	}

	if (count > 0)
		return false;

	if (allAscii)
		return false;

	return true;
}

std::string JsonDeserializer::Utf82Ansi(const std::string& s)
{
	if (!IsUtf8Encoding(s))
	{
		return s;
	}

	int nwLen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = nullptr;
	MakeArray(&pwBuf, nwLen + 1);

	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = nullptr;
	MakeArray(&pBuf, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string result = pBuf;

	DeleteArray(&pBuf);
	DeleteArray(&pwBuf);

	return result;
}

std::string JsonDeserializer::GetParsePath()
{
	std::list<std::string>::iterator it = info_.begin();

	std::string path = *it++;

	for (; it != info_.end(); it++)
	{
		path += "/" + *it;
	}

	return path;
}