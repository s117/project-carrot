#pragma once

#include <memory>
#include <QBitArray>
#include <QVector>
#include <QSettings>
#include <SFML/Graphics.hpp>

#include "../graphics/Tileset.h"
#include "../struct/WeaponTypes.h"
#include "../struct/PCEvent.h"
#include "../struct/Hitbox.h"
#include "../struct/AnimState.h"
#include "../struct/Resources.h"
#include "../struct/Layers.h"

class CarrotQt5;
class AnimatedTile;

class DestructibleDebris {
public:
    DestructibleDebris(std::shared_ptr<sf::Texture> texture, std::weak_ptr<sf::RenderTarget> window, 
        int x, int y, unsigned textureX, unsigned textureY, unsigned short quarter);
    ~DestructibleDebris();
    void tickUpdate();
    double getY();

private:
    std::unique_ptr<sf::Sprite> sprite;
    std::weak_ptr<sf::RenderTarget> window;
    double posX;
    double posY;
    double speedX;
    double speedY;
    static const int speedMultiplier[4];
};

class TileMap : public std::enable_shared_from_this<TileMap> {
public:
    TileMap(std::shared_ptr<CarrotQt5> gameRoot, const QString& tilesetFilename, const QString& maskFilename, const QString& sprLayerFilename);
    ~TileMap();
        
    // level related
    void readLevelConfiguration(const QString& filename);
    void readLayerConfiguration(enum LayerType type, const QString& filename, unsigned layerIdx = 0, QSettings& config = QSettings());
    void readAnimatedTiles(const QString& filename);
    void drawLowerLevels();
    void drawHigherLevels();
    unsigned getLevelWidth();
    unsigned getLevelHeight();
    void setTileEventFlag(int x, int y, PCEvent e = PC_EMPTY);
    bool isPosVine(double x, double y);
    QVector<QVector<std::shared_ptr<LayerTile>>> prepareSavePointLayer();
    void loadSavePointLayer(const QVector<QVector<std::shared_ptr<LayerTile>>>& layer);
    bool checkWeaponDestructible(double x, double y, WeaponType weapon = WEAPON_BLASTER);
    bool checkSpecialDestructible(double x, double y);
    void saveInitialSpriteLayer();
    void setTrigger(unsigned char triggerID, bool newState);
    bool getTrigger(unsigned char triggerID);
    void advanceAnimatedTileTimers();
    const std::shared_ptr<sf::Texture> getTilesetTexture();
    bool isTileEmpty(unsigned x, unsigned y);
    bool isTileEmpty(const Hitbox& hitbox, bool downwards = false);

private:
    std::shared_ptr<CarrotQt5> root;
    void drawLayer(TileMapLayer& layer, std::shared_ptr<sf::RenderWindow> target);
    double translateCoordinate(const double& coordinate, const double& speed, const double& offset, const bool& isY) const;
    void updateSprLayerIdx();
    void initializeBackgroundTexture(TileMapLayer& background);
    void drawTexturedBackground(TileMapLayer& layer, const double& x, const double& y, std::shared_ptr<sf::RenderWindow> target);
    std::shared_ptr<LayerTile> cloneDefaultLayerTile(int x, int y);
    std::unique_ptr<Tileset> levelTileset;
    QVector<TileMapLayer> levelLayout;
    unsigned sprLayerIdx;
    QVector<QVector<std::shared_ptr<LayerTile>>> spriteLayerAtLevelStart;
    QVector<std::shared_ptr<AnimatedTile>> animatedTiles;
    bool triggerState[256];
    std::unique_ptr<sf::RenderTexture> texturedBackgroundTexture;
    std::unique_ptr<sf::VertexArray> texturedBackgroundFadeArray;
    unsigned levelWidth;
    unsigned levelHeight;
    std::shared_ptr<ResourceSet> sceneryResources;
};