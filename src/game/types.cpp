#include "types.hpp"

#include <string>

namespace game {

std::string attributeToString(Attribute a) {
    for (auto it = ATTRIBUTE_MAP.begin(); it != ATTRIBUTE_MAP.end(); it++)
        if (it->second == a)
            return it->first;
    throw std::runtime_error("bad attribute");
}

std::string toString(PlayerSlot slot) {
    for (auto it = PLAYER_SLOT_MAP.begin(); it != PLAYER_SLOT_MAP.end(); it++)
        if (it->second == slot)
            return it->first;
    throw std::runtime_error("bad player slot");
}

std::string toString(EquipSlot slot) {
    for (auto it = EQUIP_SLOT_MAP.begin(); it != EQUIP_SLOT_MAP.end(); it++)
        if (it->second == slot)
            return it->first;
    throw std::runtime_error("bad equip slot");
}

}
