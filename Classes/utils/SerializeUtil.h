#ifndef __SERIALIZE_UTIL_H__
#define __SERIALIZE_UTIL_H__

#include <sstream>
#include <string>
#include <vector>

class SerializeUtil
{
public:
    static std::vector<std::string> split(const std::string& text, char delimiter);

    static std::string join(const std::vector<std::string>& parts, char delimiter);
};

#endif
