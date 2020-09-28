#include <iostream>
#include <fstream>
#include <sstream>

std::string getStringFromFile(std::string fileName)
{
        std::ifstream ifile(fileName);
        std::string strDataFromFile;
        if(ifile)
        {
                std::ostringstream ss;
                ss << ifile.rdbuf();
                strDataFromFile = ss.str();
        }
        ifile.close();
        return strDataFromFile;
}
