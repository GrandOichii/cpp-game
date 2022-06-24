#pragma once

namespace game {
namespace entities {
    
class Entity {
private:
public:
    Entity() {

    }

    virtual int getMaxHealth() = 0;
    virtual int getCurrentHealth() = 0;
};

}
}
