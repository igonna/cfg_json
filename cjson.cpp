#include "cjson.h"
#include <string>
#include <iostream>

cjson::cjson(void): arr(nullptr), section_list_size(16)
{
	section_list[0] = "log ";
	section_list[1] = "line ";
	section_list[2] = "e1_input ";
	section_list[3] = "network";
	section_list[4] = "bts 0";
	section_list[5] = "trx 0";
	section_list[6] = "timeslot 0 ";
	section_list[7] = "timeslot 1 ";
	section_list[8] = "timeslot 2 ";
	section_list[9] = "timeslot 3 ";
	section_list[10] = "timeslot 4 ";
	section_list[11] = "timeslot 5 ";
	section_list[12] = "timeslot 6 ";
	section_list[13] = "timeslot 7 ";
	section_list[14] = "mncc-int";
	section_list[15] = "nitb";
	psw = "password ";
}

cjson::~cjson(void)
{
	if (arr)
		delete[] arr;
}

int cjson::nchar(std::string *str, char ch)
{
	int n_ch = 0;
	int s_size = str->size();
	for (int step = 0; step < s_size; ++step)
		if ((*str)[step] == ch)
			++n_ch;
	return n_ch;
}

void cjson::stsa(std::string *str, int n, char key)
{
	arr = new std::string[n];
	int s_size = str->size();
	arr_size = n;

	int margin = 0;
	for (int n_str = 0; n_str < n; ++n_str) {
		for (int n_char = 0; n_char < s_size; ++n_char) {
			if ((*str)[n_char + margin] == key) {
				margin += n_char + 1;
				break;
			}
			arr[n_str] += (*str)[n_char + margin];
		}
	}
}

std::string *cjson::operator[](int n)const
{
	if (n >= 0 && n < arr_size)
		return arr + n;
	else
		return nullptr;
}

void cjson::rfl_space(std::string *str)
{
	while ((*str)[0] == ' ') {
		for (int step = 0; step < static_cast<int>(str->size()) - 1; ++step) {
			(*str)[step] = (*str)[step + 1];
		}
		str->resize(str->size() - 1);
	}
//	*str += '\n';
}

void cjson::rfl_space_all(void)
{
	for (int n = 0; n < arr_size; ++n)
		rfl_space(arr + n);
}

int cjson::count_words(std::string str, char key)const
{
	int qua = 0;
	for (int step = 0; step < static_cast<int>(str.size()) - 1; ++step)
		if (str[step] == key)
			++qua;
	return ++qua;
}

bool cjson::isnum(std::string str, int pos)const
{
//	int is_n_count = 0;
//	for (; pos < static_cast<int>(str.size()); ++pos) {
//		if (!(static_cast<int>(str[pos]) >= 48
//				&& static_cast<int>(str[pos]) <= 57))
//			if (is_n_count)
//				return false;
//			else
//				return true;
//		else
//			++is_n_count;
//	}
//	return true;
	if (static_cast<int>(str[pos]) >= 48 && static_cast<int>(str[pos]) <= 57)
		return true;
	else
		return false;
}

std::string cjson::kv_str(std::string str, bool is_last_kv)
{
	if (str[0] == '!') {
		for (int step = 0; step < static_cast<int>(str.size()); ++step)
			if (str[step] == '!')
				str[step] = ' ';
		rfl_space(&str);
		std::string tmp_str = "//";
		return tmp_str + str;
	}

	std::string jstr;
	jstr += "\"";

	bool is_key = true;
	bool is_num = false;

	for (int step = 0; step < static_cast<int>(str.size()); ++step)
		if (str[step] != ' ')
			jstr += str[step];
		else
			if (is_key) {
				if (isnum(str, step + 1)) {
					jstr += "\" : ";
					is_num = true;
				} else
					if (is_num) {
						jstr += " : \"";
						is_num = false;
					} else {
						jstr += "\" : \"";
						is_num = false;
					}
				is_key = false;
			} else {
				if (isnum(str, step + 1)) {
					jstr += "\", ";
					is_num = true;
				} else
					if (is_num) {
						jstr += ", \"";
						is_num = false;
					} else {
						jstr += "\", \"";
						is_num = false;
					}

				is_key = true;
			}
	if (is_num) {
		if (!is_last_kv)
			jstr += ",";
	}
	else
		if (!is_last_kv)
			jstr += "\",";
		else
			jstr += "\"";
	return jstr;
}

bool cjson::is_fw(std::string str_1, std::string str_2)
{
	for (int i = 0; i < static_cast<int>(str_1.size()); ++i)
		if (str_1[i] == str_2[i] &&
				(str_1[i] == ' ' || i == static_cast<int>(str_1.size() - 1)))
			return true;
		else if (str_1[i] != str_2[i])
			return false;
	return false;
}
bool cjson::is_section(std::string str)
{
	for (int step = 0; step < section_list_size; ++step)
		if (is_fw(str, section_list[step])) {
			return true;
		}
	return false;
}
bool cjson::is_last_kv(int pos)
{
	if (pos >= arr_size)
		return true;
	if (is_section(arr[pos + 1]))
		return true;
	if (arr[pos + 1][0] == '!' && is_section(arr[pos + 2]))
		return true;
	return false;
}
std::string cjson::rjson_str(void)
{
	std::string js_str;
	js_str += "{\n";
	
	bool is_firbrack = true;
	bool is_prev_section = false;
	bool was_psw = false;
	std::string tab = "";
	for (int n_str = 0; n_str < arr_size; ++n_str) {
		if ((kv_str(arr[n_str], false))[0] == '/')
			js_str = js_str + '\t' + kv_str(arr[n_str], false) + '\n';
		else {

			if (arr[n_str] == "timeslot 0") {
				tab = "\t";
			}
			if (is_psw(arr[n_str], &js_str) && !was_psw) {
				was_psw = true;
				continue;
			}
		if (is_section(arr[n_str])) {
			if (!is_firbrack) {
				js_str = js_str + tab + "\t}\n\n";
				is_firbrack = true;
			}
			js_str = js_str + tab + "\t" + "\"" + arr[n_str] + "\" : {" + '\n';
			is_prev_section = true;
						
			if (arr[n_str] == "trx 0") {
				js_str += "\n\t\t{\n";
				tab = "\t";
			}	
		}
		else {
			if (is_prev_section) {
				is_firbrack = false;
				is_prev_section = false;
//				js_str = js_str + "{\n";
			}
			if (n_str < arr_size - 1) {
				if (is_last_kv(n_str))
					js_str = js_str + tab + "\t\t" + kv_str(arr[n_str], true) + '\n';
				else
				if (is_section(arr[n_str + 1]))
					js_str = js_str + tab + "\t\t" + kv_str(arr[n_str], true) + '\n';
				else
					js_str = js_str + tab + "\t\t" + kv_str(arr[n_str], false) + '\n';
			} else
				if (n_str == arr_size - 1)
					js_str = js_str + tab + "\t\t" + kv_str(arr[n_str], true) + "\n\t\t}\n";
				else
					js_str = js_str + tab + "\t\t" + kv_str(arr[n_str], false) + '\n';


		}
		}
	}
	js_str += "\t}\n}\n";
	return js_str;
}

bool cjson::is_psw(std::string passwd, std::string *add)
{
	if (is_fw(passwd, psw)) {
		*add = *add + "\n\t{\n\t\t" + kv_str(passwd, true) + "\n\t}\n";
		return true;
	}
	return false;
}












