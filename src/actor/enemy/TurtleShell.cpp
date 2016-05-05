#include "TurtleShell.h"
#include "Enemy.h"
#include "../collectible/Collectible.h"
#include "../weapon/Ammo.h"
#include "../../gamestate/TileMap.h"

#include "../../CarrotQt5.h"

TurtleShell::TurtleShell(std::shared_ptr<CarrotQt5> root, double x, double y, double initSpeedX,
    double initSpeedY, bool fromEventMap) : CommonActor(root, x, y, fromEventMap) {
    loadResources("Enemy/TurtleShell");
    setAnimation(AnimState::IDLE);

    speedX = initSpeedX;
    speedY = initSpeedY;
    friction = root->gravity / 100;
    elasticity = 0.5;
    // TODO: test the actual number
    maxHealth = health = 8;

    playSound("ENEMY_TURTLE_SHELL_FLY");
}

TurtleShell::~TurtleShell() {
}

void TurtleShell::tickEvent() {
    speedX = std::max(std::abs(speedX) - friction, 0.0) * (speedX > 1e-6 ? 1 : -1);

    double posYBefore = posY;
    CommonActor::tickEvent();
    if (posYBefore - posY > 0.5 && std::abs(speedY) < 1) {
        speedX = std::max(std::abs(speedX) - 10 * friction, 0.0) * (speedX > 1e-6 ? 1 : -1);
    }

    auto collisions = root->findCollisionActors(shared_from_this());
    Hitbox hitbox = getHitbox();
    for (auto collider : collisions) {
        if (collider.expired()) {
            continue;
        }

        auto colliderPtr = collider.lock();

        {
            auto specializedPtr = std::dynamic_pointer_cast<Ammo>(colliderPtr);
            if (specializedPtr != nullptr) {
                playSound("ENEMY_TURTLE_SHELL_FLY");
                continue;
            }
        }

        {
            auto specializedPtr = std::dynamic_pointer_cast<Enemy>(colliderPtr);
            if (specializedPtr != nullptr) {
                specializedPtr->decreaseHealth(1);
                impact(-speedX * 3);
                continue;
            }
        }

        {
            auto specializedPtr = std::dynamic_pointer_cast<Collectible>(colliderPtr);
            if (specializedPtr != nullptr) {
                specializedPtr->impact();
                continue;
            }
        }

        {
            auto specializedPtr = std::dynamic_pointer_cast<TurtleShell>(colliderPtr);
            if (specializedPtr != nullptr) {
                if (speedY - specializedPtr->speedY > 1 && speedY > 0) {
                    specializedPtr->decreaseHealth(10);
                    continue;
                }

                if (std::abs(speedX) > std::abs(specializedPtr->speedX)) {
                    // Handle this only in the faster of the two.
                    speedX *= -1;
                    posX = specializedPtr->posX + (speedX > 0 ? 1 : -1) * (hitbox.right - hitbox.left + 1);
                    specializedPtr->decreaseHealth(1);
                    playSound("ENEMY_TURTLE_SHELL_IMPACT_SHELL");
                }
                continue;
            }
        }
    }

    auto tiles = root->getGameTiles().lock();
    if (tiles != nullptr) {
        tiles->checkSpecialDestructible(hitbox);
        tiles->checkCollapseDestructible(hitbox);
        tiles->checkWeaponDestructible(posX, posY, WEAPON_BLASTER);
    }
}

void TurtleShell::impact(double speed) {
    speedX = speed / 2;
}

void TurtleShell::onHitFloorHook() {
    if (std::abs(speedY) > 1) {
        playSound("ENEMY_TURTLE_SHELL_IMPACT_GROUND");
    }
}
