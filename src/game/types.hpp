#pragma once

#include <map>
#include <vector>
#include <string> // for some idiotic reason removing this break the whole file

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
    ARMS,
    FINGER
};

const std::map<std::string, EquipSlot> EQUIP_SLOT_MAP = {
    {"HEAD", EquipSlot::HEAD},
    {"TORSO", EquipSlot::TORSO},
    {"LEGS", EquipSlot::LEGS},
    {"ARM", EquipSlot::ARM},
    {"ARMS", EquipSlot::ARMS},
    {"FINGER", EquipSlot::FINGER}
};

std::string toString(EquipSlot slot);

enum PlayerSlot {
    P_HEAD,
    P_TORSO,
    P_LEGS,
    P_ARM1,
    P_ARM2,
    P_ARMS,
    P_FINGER1,
    P_FINGER2
};

const std::map<std::string, PlayerSlot> PLAYER_SLOT_MAP = {
    {"HEAD", PlayerSlot::P_HEAD},
    {"TORSO", PlayerSlot::P_TORSO},
    {"LEGS", PlayerSlot::P_LEGS},
    {"ARM1", PlayerSlot::P_ARM1},
    {"ARM2", PlayerSlot::P_ARM2},
    {"ARMS", PlayerSlot::P_ARMS},
    {"FINGER1", PlayerSlot::P_FINGER1},
    {"FINGER2", PlayerSlot::P_FINGER2},
};

std::string toString(PlayerSlot slot);

const std::map<EquipSlot, std::vector<PlayerSlot>> SLOT_CORRELATION_MAP = {
    {EquipSlot::ARM, {PlayerSlot::P_ARM1, PlayerSlot::P_ARM2}},
    {EquipSlot::ARMS, {PlayerSlot::P_ARMS}},
    {EquipSlot::HEAD, {PlayerSlot::P_HEAD}},
    {EquipSlot::TORSO, {PlayerSlot::P_TORSO}},
    {EquipSlot::LEGS, {PlayerSlot::P_LEGS}},
    {EquipSlot::FINGER, {PlayerSlot::P_FINGER1, PlayerSlot::P_FINGER2}}
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
