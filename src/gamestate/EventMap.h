#pragma once

#include <memory>
#include <QSettings>
#include <QBitArray>
#include <QVector>
#include <QMultiMap>
#include <QString>
#include <SFML/Graphics.hpp>

#include "../gamestate/TileMap.h"
#include "../struct/PCEvent.h"
#include "../struct/CoordinatePair.h"
#include "../struct/GameDifficulty.h"

class CarrotQt5;
class EventSpawner;

struct EventTile {
    PCEvent storedEvent;
    bool isEventActive;
    quint16 eventParams[8];
};

class EventMap {
public:
    EventMap(LevelManager* root, const EventSpawner* const spawner, const QString& eventsPath,
             const QSettings& configuration, unsigned int width, unsigned int height, GameDifficulty difficulty = DIFFICULTY_NORMAL);
    ~EventMap();
    void storeTileEvent(int x, int y, PCEvent e = PC_EMPTY, int flags = 0, const QVector<quint16>& params = QVector<quint16>());
    void activateEvents(const CoordinatePair& center, int tileDistance = 32);
    bool isPosHurting(double x, double y);
    unsigned short isPosPole(double x, double y);
    void deactivate(int x, int y);
    int getPositionWarp(double x, double y);
    PCEvent getPositionEvent(double x, double y);
    PCEvent getPositionEvent(int x, int y);
    void getPositionParams(double x, double y, quint16 (&params)[8]);
    void getPositionParams(int x, int y, quint16 (&params)[8]);
    void deactivateAll();
    void setTileParam(int x, int y, unsigned char idx, quint16 value);
    CoordinatePair getWarpTarget(unsigned id);
    QSet<QString> getResourceNameList();

private:
    void readEvents(const QString& filename, unsigned layoutVersion, GameDifficulty difficulty = DIFFICULTY_NORMAL);
    void addWarpTarget(unsigned id, unsigned x, unsigned y);
    bool positionHasEvent(int x, int y);
    LevelManager* root;
    QVector<QVector<std::shared_ptr<EventTile>>> eventLayout;
    QMultiMap<unsigned, CoordinatePair> warpTargets;
    const EventSpawner* const spawner;
    QSet<QString> resourceNames;
};
