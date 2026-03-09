#include "SerializeUtil.h"

std::vector<std::string> SerializeUtil::split(const std::string &text, char delimiter)
{
        std::vector<std::string> parts;
        std::stringstream stream(text);
        std::string part;
        while (std::getline(stream, part, delimiter))
        {
            parts.push_back(part);
        }
        return parts;
}

std::string SerializeUtil::join(const std::vector<std::string> &parts, char delimiter)
{
        std::stringstream stream;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i != 0)
            {
                stream << delimiter;
            }
            stream << parts[i];
        }
        return stream.str();
}