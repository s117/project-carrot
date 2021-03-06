#include "AmmoToaster.h"
#include "../../gamestate/ActorAPI.h"
#include "../../gamestate/TileMap.h"

AmmoToaster::AmmoToaster(const ActorInstantiationDetails& initData, std::weak_ptr<Player> firedBy,
    double initSpeed, bool firedLeft, bool firedUp, bool poweredUp)
    : Ammo(initData, firedBy, firedLeft, firedUp, 63, poweredUp) {
    isGravityAffected = false;
    loadResources("Weapon/Toaster");
    if (firedUp) {
        speed.x = (qrand() % 100 - 50.0) / 100.0;
        speed.y = (1.0 + qrand() % 100 * 0.001) * -3;
    } else {
        speed.y = (qrand() % 100 - 50.0) / 100.0;
        speed.x = (1.0 + qrand() % 100 * 0.001) * (firedLeft ? -1 : 1) + initSpeed;
    }

    AnimationUser::setAnimation(poweredUp ? "WEAPON_TOASTER_POWERUP" : "WEAPON_TOASTER");
}


AmmoToaster::~AmmoToaster() {
}

void AmmoToaster::tickEvent() {
    Ammo::tickEvent();

    auto tiles = api->getGameTiles().lock();
    if (tiles == nullptr || tiles->isTileEmpty((pos + speed).tilePosition())) {
        moveInstantly(speed, false, true);
    } else {
        moveInstantly(speed, false, true);
        checkCollisions();
        moveInstantly(-speed, false, true);

        if (!poweredUp) {
            health = 0;
        }
    }
}

WeaponType AmmoToaster::getType() const {
    return WEAPON_TOASTER;
}

void AmmoToaster::ricochet() {
    // do nothing
}
