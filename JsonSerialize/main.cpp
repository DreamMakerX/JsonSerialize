#include "JsonSerializer.h"
#include "JsonDeserializer.h"

class Person
{
public:
	int							age;
	std::string					name;
	std::vector<std::string>	skills;

	template<typename T>
	void serialize(T& v)
	{
		SIMPLE_SERIALIZE(age);
		SIMPLE_SERIALIZE(name);
		SIMPLE_SERIALIZE(skills);
	}

	template<typename T>
	void deserialize(T& v)
	{
		SIMPLE_DESERIALIZE(age);
		SIMPLE_DESERIALIZE(name);
		SIMPLE_DESERIALIZE(skills);
	}
};
class Club 
{
public:
	std::string					name;
	std::string					location;
	std::vector<Person>			memberInfo;

	template<typename T>
	void serialize(T& v)
	{
		SIMPLE_SERIALIZE(name);
		SIMPLE_SERIALIZE(location);
		SIMPLE_SERIALIZE(memberInfo);
	}

	template<typename T>
	void deserialize(T& v)
	{
		SIMPLE_DESERIALIZE(name);
		SIMPLE_DESERIALIZE(location);
		SIMPLE_DESERIALIZE(memberInfo);
	}
};

std::string GetJsonString()
{
	return "{\"name\": \"Tennis Club\",\"location\" : \"xxx, Hedong District, Tianjin, China.\",\"memberInfo\" : [{\"name\": \"zcc\",\"age\" : 23,\"skills\" : [\"writing\",\"photography\",\"programming\"]},{\"name\": \"xyx\",\"age\" : 22,\"skills\" : [\"playing the piano\",\"dancing\"]}]}";
	/*JSON格式
{
	"name": "Tennis Club",
	"location": "xxx, Hedong District, Tianjin, China.",
	"memberInfo": [
		{
			"name": "zcc",
			"age": 23,
			"skills": [
				"writing",
				"photography",
				"programming"
			]
		},
		{
			"name": "xyx",
			"age": 22,
			"skills": [
				"playing the piano",
				"dancing"
			]
		}
	]
}
*/
}
void PrintClubObject(const Club& club)
{
	std::cout << "====================print club object start====================" << std::endl;

	std::cout << "name: " << club.name << std::endl;
	std::cout << "location: " << club.location << std::endl;
	std::cout << "memberInfo: " << std::endl;
	for (const auto& member : club.memberInfo)
	{
		std::cout << "\tname: " << member.name << std::endl;
		std::cout << "\tlocation: " << member.age << std::endl;
		std::cout << "\tskills: " << std::endl;
		for (const auto& skill : member.skills)
		{
			std::cout << "\t\t" << skill << std::endl;
		}
	}
	std::cout << "====================print club object end======================" << std::endl;
}
void PrintJsonString(const std::string& json)
{
	std::cout << "====================print json string start====================" << std::endl;
	std::cout << json << std::endl;
	std::cout << "====================print json string end======================" << std::endl;
}

void main()
{
	//创建反序列化对象
	JsonDeserializer jdl(GetJsonString());	//传入std::string类型,当作json string处理
	//JsonDeserializer jdl("club.txt");		//传入const char*类型，当作文件路径处理

	//执行反序列化
	Club club;
	jdl.Deserialize(club);

	//遍历输出Club对象
	PrintClubObject(club);

	//任意修改对象属性，以location为例
	club.location = "Beijing, China.";

	//创建序列化对象
	JsonSerializer js;					//空参数构造函数,则序列化时只返回json字符串
	//JsonSerializer js("club.txt");	//构造函数传入文件路径,则序列化时还会输出到文件

	//执行序列化
	std::string serialString = js.Serialize(club);

	//输出序列化结果
	PrintJsonString(serialString);

	std::cin.get();
}