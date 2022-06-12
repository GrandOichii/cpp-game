#include "types.hpp"

#include <string>

namespace game {

std::string attributeToString(Attribute a) {
    for (auto it = ATTRIBUTE_MAP.begin(); it != ATTRIBUTE_MAP.end(); it++)
        if (it->second == a)
            return it->first;
    throw std::runtime_error("attribute map not full");
}

}
