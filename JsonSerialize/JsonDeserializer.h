#pragma once

#include <string>
#include <vector>
#include <stack>
#include <list>
#include <fstream>
#include <iostream>
#include <windows.h>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#define SIMPLE_DESERIALIZE(x) v.deserialize(#x, x);
#define SIMPLE_DESERIALIZE_OPTIONAL(x) v.deserializeOptional(#x, x);

struct Object
{
	Object(){
		type = TYPE_ERR;
	}

	enum ObjectValueType
	{
		TYPE_ERR = 0,
		TYPE_INT,
		TYPE_STRING,
		TYPE_VECTOR,
		TYPE_NULL
	};

	ObjectValueType type;
	int int_val;
	std::string string_val;
	std::vector<Object> vector_val;
};

class JsonDeserializer
{
public:
	JsonDeserializer(const char* path);

	JsonDeserializer(const std::string& jsonString);

	~JsonDeserializer()
	{}

	template<typename T>
	void Deserialize(T& value)
	{
		value.deserialize(*this);
	}

	template<typename T>
	void deserialize(std::string name, T& value)
	{
		info_.push_back(name);

		const rapidjson::Value& json_parent = *value_stack_.top();

		if (!json_parent.HasMember(name.c_str()))
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] doesn't exist!";
			throw err_info;
		}

		const rapidjson::Value& json_value = json_parent[name.c_str()];
		value_stack_.push(&json_value);

		deserialize(value);

		value_stack_.pop();

		info_.pop_back();
	}

	template<typename T>
	void deserializeOptional(std::string name, T& value)
	{
		info_.push_back(name);

		const rapidjson::Value& json_parent = *value_stack_.top();

		if (!json_parent.HasMember(name.c_str()))
		{
			info_.pop_back();
			return;
		}

		const rapidjson::Value& json_value = json_parent[name.c_str()];
		value_stack_.push(&json_value);

		deserialize(value);

		value_stack_.pop();

		info_.pop_back();
	}

	template<typename T>
	void deserialize(std::string name, std::vector<T>& values)
	{
		info_.push_back(name);

		const rapidjson::Value& json_parent = *value_stack_.top();

		if (!json_parent.HasMember(name.c_str()))
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] doesn't exist!";
			throw err_info;
		}

		const rapidjson::Value& json_array = json_parent[name.c_str()];

		if (!json_array.IsArray())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not an Array!";
			throw err_info;
		}

		for (size_t i = 0; i < json_array.Size(); i++)
		{
			info_.push_back(std::to_string((long long)i));

			const rapidjson::Value& json_value = json_array[i];

			value_stack_.push(&json_value);

			T value;
			deserialize(value);
			values.push_back(value);

			value_stack_.pop();

			info_.pop_back();
		}

		info_.pop_back();
	}

	template<typename T>
	void deserializeOptional(std::string name, std::vector<T>& values)
	{
		info_.push_back(name);

		const rapidjson::Value& json_parent = *value_stack_.top();

		if (!json_parent.HasMember(name.c_str()))
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] doesn't exist!";
			throw err_info;
		}

		const rapidjson::Value& json_array = json_parent[name.c_str()];

		if (!json_array.IsArray())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not an Array!";
			throw err_info;
		}

		for (size_t i = 0; i < json_array.Size(); i++)
		{
			info_.push_back(std::to_string((long long)i));

			const rapidjson::Value& json_value = json_array[i];

			value_stack_.push(&json_value);

			T value;
			deserialize(value);
			values.push_back(value);

			value_stack_.pop();

			info_.pop_back();
		}

		info_.pop_back();
	}

private:
	void deserialize(std::string& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsString())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a String value!";
			throw err_info;
		}

			value = Utf82Ansi(json_value.GetString());
	}

	void deserialize(int& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsInt())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Int value!";
			throw err_info;
		}

		value = json_value.GetInt();
	}

	void deserialize(long& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsInt64())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Int64 value!";
			throw err_info;
		}

		value = (long)json_value.GetInt64();
	}

	void deserialize(unsigned char& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsUint())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Uint value!";
			throw err_info;
		}

		value = json_value.GetUint();
	}

	void deserialize(unsigned short& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsUint())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Uint value!";
			throw err_info;
		}

		value = json_value.GetUint();
	}

	void deserialize(unsigned long& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsUint64())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Uint64 value!";
			throw err_info;
		}

		value = (unsigned long)json_value.GetUint64();
	}

	void deserialize(double& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsDouble())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Double value!";
			throw err_info;
		}

		value = json_value.GetDouble();
	}

	void deserialize(bool& value)
	{
		const rapidjson::Value& json_value = *value_stack_.top();

		if (!json_value.IsBool())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a bool value!";
			throw err_info;
		}

		value = json_value.GetBool();
	}

	void deserialize(Object& object)
	{
		const rapidjson::Value& json_obj = *value_stack_.top();

		if (json_obj.IsInt())
		{
			object.type = Object::TYPE_INT;
			object.int_val = json_obj.GetInt();
		}
		else if (json_obj.IsString())
		{
			object.type = Object::TYPE_STRING;
			object.string_val = json_obj.GetString();
		}
		else if (json_obj.IsArray())
		{
			object.type = Object::TYPE_VECTOR;

			for (size_t i = 0; i < json_obj.Size(); i++)
			{
				info_.push_back(std::to_string((long long)i));

				const rapidjson::Value& json_obj_item = json_obj[i];

				value_stack_.push(&json_obj_item);

				Object obj;
				deserialize(obj);
				object.vector_val.push_back(obj);

				value_stack_.pop();

				info_.pop_back();
			}
		}
		else if(json_obj.IsNull())
		{
			object.type = Object::TYPE_NULL;
		}
		else
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not a Int or String or Array or Null value!";
			throw err_info;
		}
	}

	template<typename T>
	void deserialize(T& value)
	{
		value.deserialize(*this);
	}
	
	template<typename T>
	void deserialize(std::vector<T>& values)
	{
		const rapidjson::Value& json_array = *value_stack_.top();

		if (!json_array.IsArray())
		{
			std::string err_info = "Load json file [" + file_path_ + "] error, [" + GetParsePath() + "] is not an Array!";
			throw err_info;
		}

		for (size_t i = 0; i < json_array.Size(); i++)
		{
			info_.push_back(std::to_string((long long)i));

			const rapidjson::Value& json_value = json_array[i];

			value_stack_.push(&json_value);

			T value;
			deserialize(value);
			values.push_back(value);

			value_stack_.pop();

			info_.pop_back();
		}
	}

	rapidjson::Document document_;

	std::stack<const rapidjson::Value*> value_stack_;

	std::list<std::string> info_;

	std::string file_path_;
	BYTE file_encoding;

private:
	template<class T>
	void MakeArray(T** ptr, const size_t& count)
	{
		if (count == 0)
		{
			*ptr = nullptr;
		}
		else
		{
			*ptr = new T[count];
			memset(*ptr, 0, count * sizeof(T));
		}
	}
	template<class T>
	void DeleteArray(T** ptr)
	{
		if (*ptr != nullptr)
		{
			delete[] * ptr;
			*ptr = nullptr;
		}
	}
	bool IsUtf8Encoding(const std::string& s);
	std::string GetParsePath();
	std::string Utf82Ansi(const std::string& s);
};