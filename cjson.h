#include <string>
#include <iostream>
#include <cstring>
class cjson
{
	std::string *arr;
	int arr_size;
	std::string section_list[16];
	int section_list_size;
	std::string psw;
public:
	cjson(void);
	~cjson(void);
	//return count of ch (character) from str
	int nchar(std::string *str, char ch);
	//string to strings array
	void stsa(std::string *str, int n, char key);
	//return string
	std::string *operator[](int n)const;
	//remove first and last space
	void rfl_space(std::string *str);
	//remove first and last space of all arr
	void rfl_space_all(void);
	//count words in string
	int count_words(std::string str, char key)const;
	//return true if number
	bool isnum(std::string str, int pos)const;
	//return "key" : "value", string
	std::string kv_str(std::string str, bool is_last_kv);
	//is first word equal
	bool is_fw(std::string str_1, std::string str_2);
	//if it is section return true
	bool is_section(std::string str);
	//return string like json
	std::string rjson_str(void);
	//if this string its password return true
	bool is_psw(std::string passwd, std::string *add);
};
