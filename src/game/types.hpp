#pragma once

#include <map>
#include <string>

namespace game {

constexpr int CLOSE_OPERATION = 1;
constexpr int EQUIP_OPERATION = 2;
constexpr int USE_OPERATION = 4;

enum DamageType {
    PHYSICAL,
    FIRE,
    ICE
};

// const std::map<std::string, DamageType> DAMAGE_TYPE_MAP = {};
const std::map<std::string, DamageType> DAMAGE_TYPE_MAP = {
    {"PHYSICAL", DamageType::PHYSICAL},
    {"FIRE", DamageType::FIRE},
    {"ICE", DamageType::ICE}
};

enum AmmoType {
    ARROW,
    BOLT
};

const std::map<std::string, AmmoType> AMMO_TYPE_MAP = {
    {"ARROW", AmmoType::ARROW},
    {"BOLT", AmmoType::BOLT}
};

enum EquipSlot {
    HEAD,
    TORSO,
    LEGS,
    ARM,
    ARMS
};

const std::map<std::string, EquipSlot> EQUIP_SLOT_MAP = {
    {"HEAD", EquipSlot::HEAD},
    {"TORSO", EquipSlot::TORSO},
    {"LEGS", EquipSlot::LEGS},
    {"ARM", EquipSlot::ARM},
    {"ARMS", EquipSlot::ARMS}
};

enum Attribute {
    VIT,
    STR,
    AGI,
    INT,
    WIS,
    LUC
};

const std::map<std::string, Attribute> ATTRIBUTE_MAP = {
    {"VIT", Attribute::VIT},
    {"STR", Attribute::STR},
    {"AGI", Attribute::AGI},
    {"INT", Attribute::INT},
    {"WIS", Attribute::WIS},
    {"LUC", Attribute::LUC}
};

std::string attributeToString(Attribute a);

}
