#include <iostream>
#include <cstring>
#include "ownlib.h"
#include "cjson.h"

const char *help = "--help";
int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Few arguments!" << std::endl
			<< "./cfg-json --help" << std::endl;
		return 1;
	}
	if (!strcmp(argv[1], help)) {
		std::cout << "./cfg-json [file.cfg]" << std::endl;
		return 0;
	}

	std::string *file_data = new std::string;
	*file_data = getStringFromFile(argv[1]);

	cjson cfg_json;
	char new_str_key = '\n';
	int qua_str = cfg_json.nchar(file_data, new_str_key);
	cfg_json.stsa(file_data, qua_str, new_str_key);
	cfg_json.rfl_space_all();
	
	std::cout << cfg_json.rjson_str();
	delete file_data;
	return 0;
}
