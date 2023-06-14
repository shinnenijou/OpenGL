#include "fileSystem.h"
#include <string>
#include <iostream>
#include <fstream>

const std::string read_text(const std::string &filename)
{
    std::string buffer;

    std::ifstream istrm(filename.c_str());
    if (!istrm.is_open())
    {
        std::cerr << "Failed to open " << filename << std::endl;
    }
    else
    {
        char c;
        while (istrm.get(c))
        {
            buffer.push_back(c);
        }
    }

    return buffer;
}
