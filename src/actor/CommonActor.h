#pragma once

#include <memory>
#include <QObject>
#include <QKeyEvent>
#include <QBitArray>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "../gamestate/AnimationUser.h"
#include "../gamestate/TimerUser.h"
#include "../struct/Controls.h"
#include "../struct/CoordinatePair.h"
#include "../struct/Hitbox.h"
#include "../struct/Resources.h"
#include "../struct/Layers.h"

class ActorAPI;
class TileMap;
class GameView;
class Ammo;

// Common parameters that each actor type needs to pass down to the CommonActor constructor.
// TODO: Carry the wanted resource type within this struct (prerequisite for fixing a part of issue #46)
struct ActorInstantiationDetails {
    std::shared_ptr<ActorAPI> api;
    CoordinatePair coords;
    bool fromEventMap;

    ActorInstantiationDetails(std::shared_ptr<ActorAPI> api, CoordinatePair coords = { 0.0, 0.0 }, bool fromEventMap = false)
        : api(api), coords(coords), fromEventMap(fromEventMap) {

    }
};

class CommonActor : public std::enable_shared_from_this<CommonActor>, public AnimationUser {
public:
    CommonActor(const ActorInstantiationDetails& initData);
    ~CommonActor();
    virtual void drawUpdate(std::shared_ptr<GameView>& view);
    virtual void tickEvent();
    void decreaseHealth(unsigned amount = 1);
    virtual void setToViewCenter(std::shared_ptr<GameView> view);
    CoordinatePair getPosition();
    virtual void updateHitbox();
    virtual void updateHitbox(const uint& w, const uint& h);
    sf::Vector2f getSpeed();
    bool getIsCollidable();
    virtual bool perish();
    virtual bool deactivate(const TileCoordinatePair& tilePos, int tileDistance);
    virtual void handleCollision(std::shared_ptr<CommonActor> other);
    bool moveInstantly(CoordinatePair location, bool absolute, bool force = false);
    void deleteFromEventMap();
    void updateGraphicState();
    void setInvulnerability(uint frames = 210u, bool blink = false);
    void advanceActorAnimationTimers();
    const ActorGraphicState getGraphicState();
    void handleAmmoFrozenStateChange(std::shared_ptr<CommonActor> ammo);
    Hitbox getHitbox();
        
protected:
    bool setAnimation(AnimStateT state) override;
    virtual void onHitFloorHook();
    virtual void onHitCeilingHook();
    virtual void onHitWallHook();
    bool loadResources(const QString& classId);
    virtual void tryStandardMovement();

    template<typename... P>
    bool playSound(const QString& id, P... params);

    template<typename... P>
    bool playNonPositionalSound(const QString& id, P... params);

    std::shared_ptr<ActorAPI> api;
    unsigned maxHealth;
    unsigned health;

    CoordinatePair pos;
    sf::Vector2f speed;
    sf::Vector2f externalForce;
    sf::Vector2f internalForce;
    sf::Vector2i originTile;

    bool canJump;
    bool canBeFrozen;
    bool isFacingLeft;
    bool isGravityAffected;
    bool isClippingAffected;
    bool isInvulnerable;
    bool isBlinking;
    bool isCollidable;
    bool isInvisible;
    uint frozenFramesLeft;
    double elasticity;
    double friction;
    SuspendType suspendType;
    bool isCreatedFromEventMap;
    std::shared_ptr<ResourceSet> resources;
    ActorGraphicState currentGraphicState;
    Hitbox currentHitbox;

private:
    template<typename T, typename... P>
    bool callPlaySound(const QString& id, T coordOrBool, P... params);
    
    static const double COLLISION_CHECK_STEP;
};
