#pragma once
#include "Collectible.h"

class ActorAPI;
class Player;

class CarrotCollectible : public Collectible {
public:
    CarrotCollectible(const ActorInstantiationDetails& initData, bool maxCarrot);
    virtual void collect(std::shared_ptr<Player> player) override;

private:
    bool maxCarrot;
};
