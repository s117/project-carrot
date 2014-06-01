#ifndef H_CP_BRIDGE
#define H_CP_BRIDGE

#include "CommonActor.h"
#include "SolidObject.h"
#include "CarrotQt5.h"

enum DynamicBridgeType {
    BRIDGE_ROPE         = 0,
    BRIDGE_ROCKS_GRAY   = 1,
    BRIDGE_VINE         = 2,
    BRIDGE_ROCKS_RED    = 3,
    BRIDGE_LOGS         = 4,
    BRIDGE_DIAMOND      = 5,
    BRIDGE_LABRAT       = 6
    // type 7 in JJ2 equals to 0
};

class DynamicBridgePiece : public SolidObject {
    public:
        DynamicBridgePiece(CarrotQt5* root, double x = 0.0, double y = 0.0, DynamicBridgeType type = BRIDGE_ROPE);
        ~DynamicBridgePiece();
        bool deactivate(int x, int y, int dist) override;
        Hitbox getCollHitbox();
};

class DynamicBridge : public CommonActor {
    public:
        DynamicBridge(CarrotQt5* root, double x = 0.0, double y = 0.0, unsigned int width = 1, DynamicBridgeType type = BRIDGE_ROPE, unsigned int toughness = 0);
        ~DynamicBridge();
        Hitbox getHitbox() override;
        void tickEvent() override;

    private:
        double original_y;
        unsigned short toughness;
        DynamicBridgeType bridge_type;
        unsigned int bridge_width;
        QList< DynamicBridgePiece* > bridge_objs;
};

#endif
