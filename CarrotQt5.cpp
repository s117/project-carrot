#include "CarrotQt5.h"
#include "QSFMLCanvas.h"
#include "CarrotCanvas.h"
#include "CommonActor.h"
#include "Player.h"
#include "SolidObject.h"
#include "TileMap.h"
#include "EventMap.h"
#include "JJ2Format.h"
#include "MenuScreen.h"

#include "ui_AboutCarrotDialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QTime>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDir>
#include <QSettings>
#include <QFile>
#include <QDebug>
#include <bass.h>

CarrotQt5::CarrotQt5(QWidget *parent) : QMainWindow(parent), paused(false), levelName(""), frame(0),
    gravity(0.3), dbgShowMasked(false), lightingLevel(80), isMenu(false), mod_current(0), menu_object(nullptr), fps(0) {
#ifndef CARROT_DEBUG
    // Set application location as the working directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());
#endif

    // Apply the UI file, set window flags and connect menu items to class slots
    ui.setupUi(this);
    //setWindowFlags( (windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint);

    connect(ui.action_About_Project_Carrot,SIGNAL(triggered()),this,SLOT(openAboutCarrot()));
    connect(ui.action_About_Qt_5,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    connect(ui.action_Home_Page,SIGNAL(triggered()),this,SLOT(openHomePage()));
    connect(ui.debug_music,SIGNAL(triggered()),this,SLOT(debugLoadMusic()));
    connect(ui.debug_gravity,SIGNAL(triggered()),this,SLOT(debugSetGravity()));
    connect(ui.debug_lighting,SIGNAL(triggered()),this,SLOT(debugSetLighting()));
    connect(ui.debug_masks,SIGNAL(triggered(bool)),this,SLOT(debugShowMasks(bool)));

    // Initialize the paint surface
    window = new CarrotCanvas(ui.mainFrame, QPoint(0, 0), QSize(800, 600));
    window->show();
    
    // Fill the player pointer table with zeroes
    std::fill_n(players, 32, nullptr);

    // Read the main font
    mainFont = new BitmapFont("Data/Assets/jj2_font.png",29,31,14,16,32,256);
    
    // Define the game view which we'll use for following the player
    game_view = new sf::View(sf::FloatRect(0.0,0.0,800.0,600.0));
    ui_view = new sf::View(sf::FloatRect(0.0,0.0,800.0,600.0));

    // Attempt to start up the sound system, using the default audio
    // device
	if (!BASS_Init(-1,44100,0,0,NULL)) {
        //std::cout << "Failed to initialize the BASS sound system!\r\n";
    }
    BASS_SetVolume(1.0);
    sfxsys = new SFXSystem();
    sfxsys->addSFX(SFX_BLASTER_SHOOT_JAZZ,"weapon/bullet_blaster_jazz_4.wav");
    sfxsys->addSFX(SFX_COLLECT_AMMO,"pickup/ammo.wav");
    sfxsys->addSFX(SFX_COLLECT_GEM,"pickup/gem.wav"); // TODO: direct pitch modification
    sfxsys->addSFX(SFX_COLLECT_COIN,"pickup/coin.wav");
    sfxsys->addSFX(SFX_JUMP,"common/char_jump.wav");
    sfxsys->addSFX(SFX_LAND,"common/char_land.wav");
    sfxsys->addSFX(SFX_SWITCH_AMMO,"weapon_change.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_1.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_2.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_3.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_4.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_5.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_6.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_7.wav");
    sfxsys->addSFX(SFX_JAZZ_HURT,"jazz/hurt_8.wav");
    sfxsys->addSFX(SFX_AMMO_HIT_WALL,"common/wall_poof.wav");
    sfxsys->addSFX(SFX_SAVE_POINT,"object/savepoint_open.wav");
    sfxsys->addSFX(SFX_JAZZ_EOL,"jazz/level_complete.wav");
    sfxsys->addSFX(SFX_TOASTER_SHOOT,"weapon/toaster.wav");
    sfxsys->addSFX(SFX_WARP_IN,"common/warp_in.wav");
    sfxsys->addSFX(SFX_WARP_OUT,"common/warp_out.wav");
    sfxsys->addSFX(SFX_LIZARD_SPONTANEOUS,"lizard/noise_4.wav");
    sfxsys->addSFX(SFX_BLOCK_DESTRUCT,"common/scenery_destruct.wav");

    installEventFilter(this);

    // Define pause screen resources
    winTex = new sf::Texture();
    winTex->create(800,600);
    winTex->update(*window);

    pauseCap = new sf::Sprite();
    pauseCap->setTexture(*winTex);

    // Define the pause text and add vertical bounce animation to it
    pausedText = new BitmapString(mainFont,"Pause",FONT_ALIGN_CENTER);
    pausedText->setAnimation(true,0.0,6.0,0.015,1.25);

    // Create the light overlay texture
    lightTex = new sf::RenderTexture();
    lightTex->create(1600,1200);

    // Fill the light overlay texture with black
    sf::RectangleShape orect(sf::Vector2f(1600,1200));
    orect.setFillColor(sf::Color::Black);
    lightTex->draw(orect);

    // Create a hole in the middle
    sf::CircleShape ocirc(96);
    ocirc.setFillColor(sf::Color(0,0,0,0));
    ocirc.setOrigin(96,96);
    ocirc.setPosition(800, 600);
    lightTex->draw(ocirc, sf::RenderStates(sf::BlendNone));

    // TODO: create a feather effect
    // check the shader stuff and apply them to the texture here
    
    for (int i = 0; i < 32; ++i) {
        mod_name[i] = "(UNSET)";
        mod_temp[i] = 0;
    }
    mod_name[0] = "PerspCurve";      mod_temp[0] = 20;
    mod_name[1] = "PerspMultipNear"; mod_temp[1] = 1;
    mod_name[2] = "PerspMultipFar";  mod_temp[2] = 3;
    mod_name[3] = "PerspSpeed";      mod_temp[3] = 4;
    mod_name[4] = "SkyDepth";        mod_temp[4] = 1;

    last_timestamp = QTime::currentTime();
}

CarrotQt5::~CarrotQt5() {
    if (!isMenu) {
        cleanUpLevel();
        clearTextureCache();
    }
    delete sfxsys;
    delete window;
    delete game_view;
    delete ui_view;
}

void CarrotQt5::parseCommandLine() {
    QStringList param = QCoreApplication::arguments();
    QString level = "";
    if (param.size() > 1) {
        QDir d(QDir::currentPath());
        if (d.exists(param.at(param.size() - 1))) {
            level = param.at(param.size() - 1);
        }
    }

    if (level != "") {
        startGame(level);
    } else {
        startMainMenu();
    }
}

void CarrotQt5::cleanUpLevel() {
    delete game_tiles;
    delete game_events;
    for (int i = actors.size() - 1; i >= 0; --i) {
        if (i >= actors.size()) {
            continue;
        }
        delete actors.at(i);
    }
    for (int i = 0; i < 32; i++) {
        players[i] = nullptr;
    }
    clearTextureCache();
    
    window->setView(*ui_view);
}

void CarrotQt5::startGame(QVariant filename) {
    // Parameter is expected to be a string
    if (filename.canConvert<QString>()) {
        if (loadLevel(filename.toString())) {
            myTimer.setInterval(1000 / 70);
            myTimer.disconnect(this,SLOT(mainMenuTick()));
            connect(&myTimer, SIGNAL(timeout()), this, SLOT(gameTick()));

            if (menu_object != nullptr) {
                delete menu_object;
            }
            menu_object = nullptr;

            myTimer.start();
            isMenu = false;
        }
    } else {
        // invalid call
    }
}

void CarrotQt5::startMainMenu() {
    myTimer.setInterval(1000 / 70);
    myTimer.disconnect(this,SLOT(gameTick()));
    connect(&myTimer, SIGNAL(timeout()), this, SLOT(mainMenuTick()));
    myTimer.start();
    isMenu = true;

    setWindowTitle("Project Carrot");
    setMusic("Music/Menu.it");

    menu_object = new MenuScreen(this);
}

void CarrotQt5::openAboutCarrot() {
    QDialog* dialog = new QDialog(this,0);
    Ui_AboutCarrot ui;
    ui.setupUi(dialog);

    dialog->show();
}

void CarrotQt5::openHomePage() {
    QDesktopServices::openUrl(QUrl("http://soul-weaver.tk/index.php?i=carrot"));
}

/*void CarrotQt5::openHelp() {
    QDesktopServices::openUrl(QUrl("http://www.google.com/"));
}*/

void CarrotQt5::closeEvent(QCloseEvent *event) {
    QMessageBox msg;
    msg.setWindowTitle("Quit Project Carrot?");
    msg.setText("Are you sure you want to quit Project Carrot?");
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setIcon(QMessageBox::Question);

    if (msg.exec() == QMessageBox::No) {
        event->ignore();
    }
}

bool CarrotQt5::eventFilter(QObject *watched, QEvent *e) {
    // Catch focus events to mute the music when the window doesn't have it
    if (e->type() == QEvent::WindowActivate) {
        BASS_ChannelSlideAttribute(currentMusic,BASS_ATTRIB_MUSIC_VOL_GLOBAL,128,1000);
        paused = false;
    } else if (e->type() == QEvent::WindowDeactivate) {
        BASS_ChannelSlideAttribute(currentMusic,BASS_ATTRIB_MUSIC_VOL_GLOBAL,0,1000);
        paused = true;
        window->setView(*ui_view);
        winTex->update(*window);
    } else if (e->type() == QEvent::Resize) {
        int w = ui.centralWidget->size().width();
        int h = ui.centralWidget->size().height();

        game_view->setSize(w,h);
        ui_view->setSize(w,h);
        ui_view->setCenter(w/2.0,h/2.0);
        ui.mainFrame->resize(ui.centralWidget->size());
        window->setSize(sf::Vector2u(w,h));
        window->setView(*ui_view);
        winTex->create(w,h);
    }
    return FALSE;  // dispatch normally
}

void CarrotQt5::keyPressEvent(QKeyEvent* event) {
    if (!isMenu) {
        if (event->isAutoRepeat()) {
            return;
        }
        for(unsigned i = 0; i < actors.size(); i++) {
            actors.at(i)->keyPressEvent(event);
        }
        if (event->key() == Qt::Key::Key_Escape) {
            cleanUpLevel();
            startMainMenu();
        }
        
        if (event->key() == Qt::Key::Key_Insert) {
            mod_temp[mod_current] += 1;
        }
        if (event->key() == Qt::Key::Key_Delete) {
            mod_temp[mod_current] -= 1;
        }
        if (event->key() == Qt::Key::Key_PageUp) {
            mod_current = (mod_current + 1) % 32;
        }
        if (event->key() == Qt::Key::Key_PageDown) {
            mod_current = (mod_current + 31) % 32;
        }
    } else {
        if (menu_object != nullptr) {
            menu_object->keyPressEvent(event);
        }
    }
}

void CarrotQt5::keyReleaseEvent(QKeyEvent* event) {
    if (!isMenu) {
        if (event->isAutoRepeat()) {
            return;
        }
        for(unsigned i = 0; i < actors.size(); i++) {
            actors.at(i)->keyReleaseEvent(event);
        }
    }
}

void CarrotQt5::mainMenuTick() {
    frame++;
    
    // Clear the drawing surface
    window->clear();
    
    if (menu_object != nullptr) {
        menu_object->tickEvent();
    }
    
    // Update the drawn surface to the screen
    window->updateContents();
}

void CarrotQt5::gameTick() {
    frame++;
    if (frame % 20 == 0) {
        QTime now = QTime::currentTime();
        fps = (1000.0 / (last_timestamp.msecsTo(now) / 20.0));
        last_timestamp = now;
    }

    if (paused) {
        // Set up a partially translucent black overlay
        sf::RectangleShape overlay(sf::Vector2f(800.0,600.0));
        overlay.setFillColor(sf::Color(0,0,0,120));
        
        window->draw(*pauseCap);
        window->draw(overlay);

        // Draw the pause string
        pausedText->drawString(window,400,280);

        // Update the display
        //window->display();
        window->updateContents();
        return;
    }

    // Clear the drawing surface; we don't want to do this if we emulate the JJ2 behavior
    window->clear();

    // Set player to the center of the view
    players[0]->setToViewCenter(game_view);
    window->setView(*game_view);

    // Deactivate far away instances, create near instances
    int view_x = static_cast<unsigned>(window->getView().getCenter().x) / 32;
    int view_y = static_cast<unsigned>(window->getView().getCenter().y) / 32;
    for(unsigned i = 0; i < actors.size(); i++) {
        if (actors.at(i)->deactivate(view_x,view_y,32)) {
            --i;
        }
    }
    game_events->activateEvents(window->getView());

    // Run animated tiles' timers
    game_tiles->advanceAnimatedTileTimers();
    // Run all actors' timers
    for(unsigned i = 0; i < actors.size(); i++) {
        actors.at(i)->advanceTimers();
    }

    // Run all actors' tick events
    for(unsigned i = 0; i < actors.size(); i++) {
        actors.at(i)->tickEvent();
        if (actors.at(i)->perish()) {
            --i;
        }
    }


    // Draw the layers: first lower (background and sprite) levels...
    game_tiles->drawLowerLevels();
    // ...then draw all the actors...
    for(unsigned i = 0; i < actors.size(); i++) {
        actors.at(i)->DrawUpdate();
    }
    // ...then all the debris elements...
    for (unsigned i = 0; i < debris.size(); i++) {
        debris.at(i)->TickUpdate();
        if (debris.at(i)->GetY() - window->getView().getCenter().y > 400) {
            delete debris.at(i);
            debris.erase(debris.begin() + i);
            --i;
        }
    }
    // ...and finally the higher (foreground) levels
    game_tiles->drawHigherLevels();

    // Draw the lighting overlay
    sf::Sprite s(lightTex->getTexture());
    s.setColor(sf::Color(255,255,255,(255 * (100 - lightingLevel) / 100)));
    s.setOrigin(800,600);
    s.setPosition(players[0]->getPosition().x,players[0]->getPosition().y - 15); // middle of the sprite vertically
    window->draw(s);

    // Draw the UI
    window->setView(*ui_view);
    
    // Draw the character icon; managed by the player object
    players[0]->drawUIOverlay();
    
    //BitmapString::drawString(window,mainFont,"Frame: " + QString::number(frame),6,56);
    BitmapString::drawString(window,mainFont,"Actors: " + QString::number(actors.size()),6,176);
    BitmapString::drawString(window,mainFont,"FPS: " + QString::number(fps,'f',2) + " at " + QString::number(1000 / fps) + "ms/f",6,56);
    BitmapString::drawString(window,mainFont,"Mod-" + QString::number(mod_current) + " " + mod_name[mod_current] + ": " + QString::number(mod_temp[mod_current]),6,540);

    // Update the drawn surface to the screen and set the view back to the player
    //window->display();
    window->updateContents();
    window->setView(*game_view);

}

bool CarrotQt5::setMusic(const QString& filename) {
    // Stop the current music track and free its resources
    if (BASS_ChannelIsActive(currentMusic)) {
        BASS_ChannelStop(currentMusic);
        BASS_MusicFree(currentMusic);
    }

    // Load the new track and start playing it
    currentMusic = BASS_MusicLoad(false,filename.toUtf8().data(),0,0,BASS_SAMPLE_LOOP,1);
    BASS_ChannelPlay(currentMusic,true);

    return true;
}

unsigned CarrotQt5::getLevelWidth() {
    // todo: phase out
    return game_tiles->getLevelWidth();
}
unsigned CarrotQt5::getLevelHeight() {
    // todo: phase out
    return game_tiles->getLevelHeight();
}

bool CarrotQt5::addActor(CommonActor* actor) {
    actors.push_back(actor);
    return true;
}

bool CarrotQt5::addPlayer(Player* actor, short player_id) {
    // If player ID is defined and is between 0 and 31 (inclusive),
    // try to add the player to the player array
    if ((player_id > -1) && (player_id < 32)) {
        if (players[player_id] != nullptr) {
            // A player with that number already existed
            return false;
        }
        // all OK, add to the player 
        players[player_id] = actor;
    }
    actors.push_back(actor);
    return true;
}

bool CarrotQt5::loadLevel(const QString& name) {
    QDir level_dir(name);
    if (level_dir.exists()) {
        QStringList level_files = level_dir.entryList(QStringList("*.layer") << "config.ini");
        if (level_files.contains("spr.layer") && level_files.contains("config.ini")) {
            // Required files found

            QSettings level_config(level_dir.absoluteFilePath("config.ini"),QSettings::IniFormat);

            if (level_config.value("Version/LayerFormat",1).toUInt() > LAYERFORMATVERSION) {
                QMessageBox msg;
                msg.setWindowTitle("Error loading level");
                msg.setText("The level is using a too recent layer format. You might need to update to a newer game version.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.setIcon(QMessageBox::Critical);
                msg.exec();
                return false;
            }
            
            setLevelName(level_config.value("Level/FormalName","Unnamed level").toString());

            // Clear the window contents
            window->clear();

            // Show loading screen
            sf::Texture t;
            t.loadFromFile("Data/Assets/screen_loading.png");
            sf::Sprite s(t);
            s.setPosition(80,60);
            window->draw(s);
            BitmapString::drawString(window,mainFont,levelName,400,360,FONT_ALIGN_CENTER);
            window->updateContents();
            
            QString tileset = level_config.value("Level/Tileset","").toString();
            
            nextLevel = level_config.value("Level/Next","").toString();
                
            
            QDir tileset_dir(QDir::currentPath() + QString::fromStdString("/Tilesets/") + tileset);
            if (tileset_dir.exists()) {
                // Read the tileset and the sprite layer
                game_tiles = new TileMap(this,tileset_dir.absoluteFilePath("tiles.png"),
                                              tileset_dir.absoluteFilePath("mask.png"),
                                              level_dir.absoluteFilePath("spr.layer"));
            
                // Read the sky layer if it exists
                if (level_files.contains("sky.layer")) {
                    game_tiles->readLayerConfiguration(LAYER_SKY_LAYER,level_dir.absoluteFilePath("sky.layer"),0, &level_config);
                }
            
                // Read the background layers
                QStringList bglayers = level_files.filter(".bg.layer");
                for (unsigned i = 0; i < bglayers.size(); ++i) {
                    game_tiles->readLayerConfiguration(LAYER_BACKGROUND_LAYER,level_dir.absoluteFilePath(bglayers.at(i)),i, &level_config);
                }

                // Read the foreground layers
                QStringList fglayers = level_files.filter(".fg.layer");
                for (unsigned i = 0; i < fglayers.size(); ++i) {
                    game_tiles->readLayerConfiguration(LAYER_FOREGROUND_LAYER,level_dir.absoluteFilePath(fglayers.at(i)),i, &level_config);
                }

                if (level_dir.entryList().contains("animtiles.dat")) {
                    game_tiles->readAnimatedTiles(level_dir.absoluteFilePath("animtiles.dat"));
                }
                
                game_events = new EventMap(this,game_tiles,getLevelWidth(),getLevelHeight());
                if (level_files.contains("event.layer")) {
                    game_events->readEvents(level_dir.absoluteFilePath("event.layer"), level_config.value("Version/LayerFormat",1).toUInt());
                }

                game_tiles->saveInitialSpriteLayer();
                
                if (players[0] == nullptr) {
                    Player* defaultplayer = new Player(this,320.0,32.0);
                    addPlayer(defaultplayer,0);
                }
                
                setMusic(("Music/" + level_config.value("Level/MusicDefault","").toString().toUtf8()).data());
                setLighting(level_config.value("Level/LightInit",100).toInt(),true);
                
                connect(ui.debug_health,SIGNAL(triggered()),players[0],SLOT(debugHealth()));
                connect(ui.debug_ammo,SIGNAL(triggered()),players[0],SLOT(debugAmmo()));

                setSavePoint();
            } else {
                QMessageBox msg;
                msg.setWindowTitle("Error loading level");
                msg.setText("Unknown tileset " + tileset);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.setIcon(QMessageBox::Critical);
                msg.exec();
                return false;
            }
        } else {
            QMessageBox msg;
            msg.setWindowTitle("Error loading level");
            msg.setText("Sprite layer file (spr.layer) or configuration file (config.dat) missing!");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
            return false;
        }
    } else {
        QMessageBox msg;
        msg.setWindowTitle("Error loading level");
        msg.setText("Level folder " + name + " doesn't exist!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
        return false;
    }
    return true;
}

void CarrotQt5::debugLoadMusic() {
    QString filename = QFileDialog::getOpenFileName(this, "Load which file?", qApp->applicationDirPath(), "Module files (*.it *.s3m *.xm *.mod *.mo3 *.mtm *.umx);;All files (*.*)");
    if (filename.endsWith(".j2b")) {
        filename = JJ2Format::convertJ2B(filename);
    }
    if (filename != "") {
        setMusic(filename);
    }
}

void CarrotQt5::debugShowMasks(bool show) {
    dbgShowMasked = show;
}

void CarrotQt5::setLevelName(const QString& name) {
    levelName = name;
    setWindowTitle("Project Carrot - " + levelName);
}

void CarrotQt5::removeActor(CommonActor* actor) {
    for (int i = 0; i < actors.size(); ++i) {
        if (actors.at(i) == actor) {
            actors.erase(actors.begin() + i);
            return;
        }
    }
}

QList< CommonActor* > CarrotQt5::findCollisionActors(CoordinatePair pos, CommonActor* me) {
    QList< CommonActor* > res;
    for (int i = 0; i < actors.size(); ++i) {
        CoordinatePair pos2 = actors.at(i)->getPosition();
        if ((std::abs(pos.x - pos2.x) < 10) && (std::abs(pos.y - pos2.y) < 10)) {
            res << actors.at(i);
        }
    }
    return res;
}

QList< CommonActor* > CarrotQt5::findCollisionActors(Hitbox hbox, CommonActor* me) {
    QList< CommonActor* > res;
    for (int i = 0; i < actors.size(); ++i) {
        if (me == actors.at(i)) {
            continue;
        }
        Hitbox hbox2 = actors.at(i)->getHitbox();
        if ((hbox.left < hbox2.right && hbox.right > hbox2.left) &&
            (hbox.top < hbox2.bottom && hbox.bottom > hbox2.top)) {
            res << actors.at(i);
        }
    }
    return res;
}

void CarrotQt5::debugSetGravity() {
    gravity = QInputDialog::getDouble(this,"Set new gravity","Gravity:",gravity,-3.0,3.0,4);
}

void CarrotQt5::debugSetLighting() {
    lightingLevel = QInputDialog::getInt(this,"Set new lighting","Lighting:",lightingLevel,0,100);
}

Hitbox CarrotQt5::calcHitbox(const Hitbox& hbox, double hor, double ver) {
    Hitbox nbox(hbox);
    nbox.left += hor;
    nbox.right += hor;
    nbox.top += ver;
    nbox.bottom += ver;
    return nbox;
}

// check if this is used anywhere
Hitbox CarrotQt5::calcHitbox(int x, int y, int w, int h) {
    Hitbox nbox;
    nbox.left = x - w/2;
    nbox.right = x + w/2;
    nbox.top = y - h/2;
    nbox.bottom = y + h/2;
    return nbox;
}

void CarrotQt5::setSavePoint() {
    lastSavePoint.player_lives = players[0]->getLives() - 1;
    lastSavePoint.player_pos = players[0]->getPosition();
    lastSavePoint.spr_layer_copy = game_tiles->prepareSavePointLayer();
}

void CarrotQt5::loadSavePoint() {
    clearActors();
    game_events->deactivateAll();
    players[0]->moveInstantly(lastSavePoint.player_pos);
    game_tiles->loadSavePointLayer(lastSavePoint.spr_layer_copy);
}

void CarrotQt5::clearActors() {
    for (int i = actors.size() - 1; i >= 0; --i) {
        if (i >= actors.size()) {
            // one actor likely destroyed another in its destructor,
            // causing the vector to suddenly drop in length.
            continue;
        }
        if (dynamic_cast< Player* >(actors.at(i)) == nullptr) {
            delete actors.at(i);
        }
    }
}

unsigned long CarrotQt5::getFrame() {
    return frame;
}

void CarrotQt5::createDebris(unsigned tile_id, int x, int y) {
    for (int i = 0; i < 4; ++i) {
        DestructibleDebris* d = new DestructibleDebris(game_tiles->getTilesetTexture(), window, x, y, tile_id % 10, tile_id / 10,i);
        debris.push_back(d);
    }
}

void CarrotQt5::setLighting(int target, bool immediate) {
    targetLightingLevel = target;
    if (target == lightingLevel) {
        return;
    }
    if (immediate) {
        lightingLevel = target;
    } else {
        QTimer::singleShot(50,this,SLOT(setLightingStep()));
    }
}

void CarrotQt5::setLightingStep() {
    if (targetLightingLevel == lightingLevel) {
        return;
    }
    short dir = (targetLightingLevel < lightingLevel) ? -1 : 1;
    lightingLevel += dir;
    QTimer::singleShot(50,this,SLOT(setLightingStep()));
}

void CarrotQt5::initLevelChange(ExitType e) {
    last_exit = e;
    QTimer::singleShot(6000,this,SLOT(delayedLevelChange()));
}

void CarrotQt5::delayedLevelChange() {
    if (last_exit == NEXT_NORMAL) {
        LevelCarryOver o = players[0]->prepareLevelCarryOver();
        cleanUpLevel();
        loadLevel("Levels/" + nextLevel);
        players[0]->receiveLevelCarryOver(o);
        last_exit = NEXT_NONE;
    }
}

bool CarrotQt5::isPositionEmpty(const Hitbox& hbox, bool downwards, CommonActor* me, SolidObject*& collided) {
    collided = nullptr;
    if (!game_tiles->isTileEmpty(hbox,downwards)) {
        return false;
    }

    // check for solid objects
    QList< CommonActor* > collision = findCollisionActors(hbox,me);
    for (int i = 0; i < collision.size(); ++i) {
        
        SolidObject* object = dynamic_cast<SolidObject*>(collision.at(i));
        if (object == nullptr) {
            continue;
        }

        if (!object->isOneWay() || downwards) {
            collided = object;
            return false;
        }
    }
    return true;
}

// alternate version to be used if we don't care what solid object we collided with
bool CarrotQt5::isPositionEmpty(const Hitbox& hbox, bool downwards, CommonActor* me) {
    SolidObject* placeholder;
    return isPositionEmpty(hbox,downwards,me,placeholder);
}

unsigned CarrotQt5::getViewHeight() {
    return game_view->getSize().y;
}

unsigned CarrotQt5::getViewWidth() {
    return game_view->getSize().x;
}

Player* CarrotQt5::getPlayer(unsigned no) {
    if (no > 32) {
        return nullptr;
    } else {
        return players[no];
    }
}

int CarrotQt5::getLightingLevel() {
    return lightingLevel;
}

sf::Texture* CarrotQt5::getCachedTexture(const QString& filename) {
    if (!tex_cache.contains(filename)) {
        // try to add the texture to the cache
        sf::Texture* t = new sf::Texture();
        if (t->loadFromFile(filename.toStdString())) {
            tex_cache.insert(filename,t);
            return t;
        } else {
            delete t;
            return nullptr;
        }
    }
    return tex_cache.value(filename);
}

void CarrotQt5::clearTextureCache() {
    QMapIterator<QString, sf::Texture*> iter(tex_cache);
    while (iter.hasNext()) {
        iter.next();
        delete tex_cache.value(iter.key());
    }
    tex_cache.clear();
}

void CarrotQt5::invokeFunction(InvokableRootFunction func, QVariant param) {
    (this->*func)(param);
}

void CarrotQt5::quitFromMainMenu(QVariant param) {
    this->close();
}
