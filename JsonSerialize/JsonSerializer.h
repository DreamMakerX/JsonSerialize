#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "rapidjson/document.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define SIMPLE_SERIALIZE(x) v.serialize(#x, x);

class JsonSerializer
{
public:
	JsonSerializer()
		:writer_(string_buffer_)
	{
	}

	JsonSerializer(std::string path)
		:file_path_(path)
		,writer_(string_buffer_)
	{
	}

	template<typename T>
	std::string Serialize(T& value, bool format = true)
	{
		writer_.StartObject();
		value.serialize(*this);
		writer_.EndObject();

		std::string result = string_buffer_.GetString();
		if (format)
		{
			result = FormatJson(result);
		}

		if (!file_path_.empty())
		{
			std::ofstream outfile(file_path_, std::ios::trunc);
			outfile << result;
		}

		return result;
	}

	template<typename T>
	void serialize(const char* name, T& value)
	{
		writer_.Key(name);
		serialize(value);
	}

	template<typename T>
	void serialize(const char* name, std::vector<T>& value)
	{
		writer_.Key(name);

		writer_.StartArray();
		for (int i = 0; i < value.size(); ++i)
		{
			serialize(value[i]);
		}
		writer_.EndArray();
	}

private:
	void serialize(bool& value)
	{
		writer_.Bool(value);
	}

	void serialize(signed char& value)
	{
		writer_.Int(value);
	}

	void serialize(short& value)
	{
		writer_.Int(value);
	}

	void serialize(int& value)
	{
		writer_.Int(value);
	}

	void serialize(long& value)
	{
		writer_.Int64(value);
	}

	void serialize(long long& value)
	{
		writer_.Int64(value);
	}

	void serialize(unsigned char& value)
	{
		writer_.Uint(value);
	}

	void serialize(unsigned short& value)
	{
		writer_.Uint(value);
	}

	void serialize(unsigned int& value)
	{
		writer_.Uint(value);
	}

	void serialize(unsigned long& value)
	{
		writer_.Uint64(value);
	}

	void serialize(unsigned long long& value)
	{
		writer_.Uint64(value);
	}

	void serialize(float& value)
	{
		writer_.Double(value);
	}

	void serialize(double& value)
	{
		writer_.Double(value);
	}

	void serialize(std::string& value)
	{
		writer_.String(value.c_str());
	}


	template<typename T>
	void serialize(T& value)
	{
		writer_.StartObject();
		value.serialize(*this);
		writer_.EndObject();
	}

	template<typename T>
	void serialize(std::vector<T>& value)
	{
		writer_.StartArray();
		for (int i = 0; i < value.size(); ++i)
		{
			serialize(value[i]);
		}
		writer_.EndArray();
	}

private:
	std::string file_path_;

	rapidjson::StringBuffer string_buffer_;

	rapidjson::Writer<rapidjson::StringBuffer> writer_;

	void AddString(char** result, size_t& size, size_t& pos, const std::string& addString);
	std::string FormatJson(const std::string& json);
	std::string GetLevelString(int level);
};