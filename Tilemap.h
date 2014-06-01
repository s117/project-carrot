#ifndef H_CP_TILEMAP
#define H_CP_TILEMAP

#include <SFML/Graphics.hpp>
#include "CarrotQt5.h"
#include "WeaponTypes.h"
#include <QSettings>
#include <QBitArray>
#include <QList>

class AnimatedTile;

enum LayerType {
    LAYER_SKY_LAYER = 0,
    LAYER_BACKGROUND_LAYER = 1,
    LAYER_SPRITE_LAYER = 2,
    LAYER_FOREGROUND_LAYER = 3
};

enum TileDestructType {
    DESTRUCT_NONE,
    DESTRUCT_WEAPON,
    DESTRUCT_SPEED, // not implemented yet
    DESTRUCT_COLLAPSE, // not implemented yet
    DESTRUCT_SPECIAL, // buttstomp, sidekick, uppercut
    DESTRUCT_TRIGGER
};

struct LayerTile {
    unsigned long tile_id;
    sf::Sprite sprite;
    bool flipped_x;
    bool flipped_y;
    bool animated;
    // collision affecting modifiers
    bool oneway;
    bool vine;
    TileDestructType dtype;
    unsigned long d_animation; // animation index for a destructible tile that uses an animation but doesn't animate normally
    int scenery_frame_idx; // denotes the specific frame from the above animation that is currently active
    // Collapsible: delay ("wait" parameter); trigger: trigger id
    unsigned extra_byte;
};

struct Tileset {
    QString unique_id;
    QString name;
    unsigned long tile_amount;
    unsigned tiles_col; // number of tiles next to each other
    sf::Texture* tiles;
    QList< QBitArray > masks;
    QList< bool > mask_empty; // to speed up collision checking so that not every tile needs to be pixel perfect checked
    QList< bool > mask_full;  // same with this one
};

struct TileMapLayer {
    enum LayerType type;
    unsigned idx;
    QList< QList< LayerTile > > tile_layout;
    double xspeed;
    double yspeed;
    double auto_xspeed;
    double auto_yspeed;
    bool xrepeat;
    bool yrepeat;
    double xoffset;
    double yoffset;
    // JJ2's "limit visible area" flag
    bool inherent_offset;

    // textured background, only useful for sky layer
    bool textured;
    bool textured_stars;
    sf::Color textured_color;

    bool TileMapLayer::operator< (TileMapLayer layer) {
        if (type != layer.type) {
            return (type < layer.type);
        }
        return (idx < layer.idx);
    };
};

class DestructibleDebris {
    public:
        DestructibleDebris(const sf::Texture* tex, sf::RenderTarget* win, int x, int y, unsigned tx, unsigned ty, unsigned short quarter);
        ~DestructibleDebris();
        void TickUpdate();
        double GetY();
    private:
        sf::Sprite* spr;
        sf::RenderTarget* wint;
        double pos_x;
        double pos_y;
        double h_speed;
        double v_speed;
};

class TileMap {
    public:
        TileMap(CarrotQt5* game_root, const QString& tileset_file, const QString& mask_file, const QString& spr_layer_file);
        ~TileMap();
        
        // level related
        void readLevelConfiguration(const QString& filename);
        void readLayerConfiguration(enum LayerType type, const QString& filename, unsigned layer_idx = 0, QSettings* config = nullptr);
        void readAnimatedTiles(const QString& filename);
        void drawLowerLevels();
        void drawHigherLevels();
        unsigned getLayerWidth(TileMapLayer* layer);
        unsigned getLayerHeight(TileMapLayer* layer);
        unsigned getLevelWidth();
        unsigned getLevelHeight();
        void setTileEventFlag(int x, int y, PCEvent e = PC_EMPTY);
        bool isPosVine(double x, double y);
        QList< QList< LayerTile > > prepareSavePointLayer();
        void loadSavePointLayer(const QList< QList< LayerTile > >& layer);
        bool checkWeaponDestructible(double x, double y, WeaponType weapon = WEAPON_BLASTER);
        bool checkSpecialDestructible(double x, double y);
        void saveInitialSpriteLayer();
        void setTrigger(unsigned char trigger_id,bool new_state);
        bool getTrigger(unsigned char trigger_id);
        void advanceAnimatedTileTimers();

        // assigned tileset related
        const sf::Texture* getTilesetTexture();
        void readTileset(const QString& file_tiles, const QString& file_mask);
        bool isTileEmpty(unsigned x, unsigned y);
        bool isTileEmpty(const Hitbox& hbox, bool downwards = false);
    private:
        CarrotQt5* root;
        void drawLayer(TileMapLayer* layer);
        double translateCoordinate(const double& coordinate, const double& speed, const double& offset, const bool& is_y) const;
        void updateSprLayerIdx();
        void initializeBackgroundTexture(TileMapLayer* background);
        void drawTexturedBackground(TileMapLayer* layer, const double& x, const double& y);
        Tileset level_tileset;
        QList< TileMapLayer > level_layout;
        unsigned spr_layer;
        QList< QList< LayerTile > > initial_spr_layer_copy;
        QList< AnimatedTile* > animated_tiles;
        bool trigger_state[256];
        sf::RenderTexture* tex_back;
        sf::VertexArray tex_fade;
        unsigned level_width;
        unsigned level_height;
};

#endif
