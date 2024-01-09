#include "File.h"

namespace Util::File
{
    std::string Read(std::string path)
    {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}
