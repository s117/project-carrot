#pragma once
#include "Collectible.h"

class ActorAPI;
class Player;

enum GemType {
    GEM_RED    = 0,
    GEM_GREEN  = 1,
    GEM_BLUE   = 2,
    GEM_PURPLE = 3
};

class GemCollectible : public Collectible {
public:
    GemCollectible(const ActorInstantiationDetails& initData, GemType type);
    virtual void collect(std::shared_ptr<Player> player) override;

private:
    const GemType gemType;
    static const uint gemValues[4];
    static const sf::Vector3i gemColors[4];
};
