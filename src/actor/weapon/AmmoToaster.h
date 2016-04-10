#pragma once

#include <memory>

#include "Ammo.h"
#include "../Player.h"

class CarrotQt5;

class AmmoToaster : public Ammo {
public:
    AmmoToaster(std::shared_ptr<CarrotQt5> root, std::weak_ptr<Player> firedBy = std::weak_ptr<Player>(),
        double x = 0.0, double y = 0.0, bool firedLeft = false, bool firedUp = false);
    ~AmmoToaster();
    void tickEvent();

private:
    void ricochet() override;
};