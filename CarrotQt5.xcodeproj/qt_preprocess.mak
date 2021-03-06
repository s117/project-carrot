#############################################################################
# Makefile for building: Release/CarrotQt5.app/Contents/MacOS/CarrotQt5
# Generated by qmake (3.0) (Qt 5.7.0)
# Project:  CarrotQt5.pro
# Template: app
# Command: /usr/local/bin/qmake -spec macx-xcode -o CarrotQt5.xcodeproj/project.pbxproj CarrotQt5.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /usr/local/opt/qt5/bin/moc
UIC       = /usr/local/opt/qt5/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DNOMINMAX -DQT_DLL -DQT_WIDGETS_LIB -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GAMEPAD_LIB -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH       = -I. -I$(BASS_DIR) -I$(BASS_FX_DIR)/C -I$(BASS_FX_DIR) -IGeneratedFiles -I. -I/usr/local/include -IGeneratedFiles/Release -I/usr/local/opt/qt5/lib/QtWidgets.framework/Headers -I/usr/local/opt/qt5/lib/QtGamepad.framework/Headers -I/usr/local/opt/qt5/lib/QtGui.framework/Headers -I/usr/local/opt/qt5/lib/QtCore.framework/Headers -IGeneratedFiles/Release -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks/OpenGL.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks/AGL.framework/Headers -IGeneratedFiles -I/usr/local/opt/qt5/mkspecs/macx-clang -F/usr/local/opt/qt5/lib
DEL_FILE  = rm -f
MOVE      = mv -f

preprocess: compilers
clean preprocess_clean: compiler_clean

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

benchmark: first

compilers: GeneratedFiles/qrc_CarrotQt5.cpp GeneratedFiles/Release/moc_CarrotQt5.cpp GeneratedFiles/Release/moc_Player.cpp GeneratedFiles/Release/moc_LevelManager.cpp GeneratedFiles/ui_AboutCarrotDialog.h GeneratedFiles/ui_CarrotQt5.h
compiler_rcc_make_all: GeneratedFiles/qrc_CarrotQt5.cpp
compiler_rcc_clean:
	-$(DEL_FILE) GeneratedFiles/qrc_CarrotQt5.cpp
GeneratedFiles/qrc_CarrotQt5.cpp: CarrotQt5.qrc \
		/usr/local/opt/qt5/bin/rcc \
		Data/PCIconLowRes.png
	/usr/local/opt/qt5/bin/rcc -name CarrotQt5 CarrotQt5.qrc -o GeneratedFiles/qrc_CarrotQt5.cpp

compiler_moc_header_make_all: GeneratedFiles/Release/moc_CarrotQt5.cpp GeneratedFiles/Release/moc_Player.cpp GeneratedFiles/Release/moc_LevelManager.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) GeneratedFiles/Release/moc_CarrotQt5.cpp GeneratedFiles/Release/moc_Player.cpp GeneratedFiles/Release/moc_LevelManager.cpp
GeneratedFiles/Release/moc_CarrotQt5.cpp: src/Version.h \
		/usr/local/opt/qt5/lib/QtWidgets.framework/Headers/QMainWindow \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QCloseEvent \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QFocusEvent \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QKeyEvent \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QEvent \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QTimer \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QTime \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QString \
		/usr/local/opt/qt5/lib/QtWidgets.framework/Headers/QWidget \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QObject \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMap \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QVector \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QStack \
		$(BASS_DIR)/bass.h \
		/usr/local/include/SFML/System.hpp \
		/usr/local/include/SFML/Graphics.hpp \
		GeneratedFiles/ui_CarrotQt5.h \
		src/graphics/BitmapFont.h \
		src/graphics/BitmapString.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QStringList \
		src/graphics/CarrotCanvas.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPoint \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSize \
		/usr/local/include/SFML/Window.hpp \
		src/graphics/QSFMLCanvas.h \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEngine \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QShowEvent \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEvent \
		src/graphics/ShaderSource.h \
		src/gamestate/ControlManager.h \
		src/struct/Controls.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPair \
		src/gamestate/ResourceManager.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMultiMap \
		src/sound/SoundSystem.h \
		$(BASS_FX_DIR)/bass_fx.h \
		src/struct/CoordinatePair.h \
		src/graphics/GraphicsCache.h \
		src/struct/Resources.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSet \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QBitArray \
		src/struct/AnimState.h \
		src/struct/Hitbox.h \
		src/struct/NextLevelData.h \
		src/struct/Constants.h \
		src/struct/WeaponTypes.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QtGlobal \
		src/struct/DebugConfig.h \
		src/CarrotQt5.h \
		/usr/local/opt/qt5/bin/moc
	/usr/local/opt/qt5/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -D__APPLE_CC__ -I/usr/local/opt/qt5/mkspecs/macx-clang -I/Users/soulweaver/Documents/project-carrot -I'/Users/soulweaver/Documents/project-carrot/$(BASS_DIR)' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)/C' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)' -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles -I/Users/soulweaver/Documents/project-carrot -I/usr/local/include -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles/Release -I/usr/local/opt/qt5/lib/QtWidgets.framework/Headers -I/usr/local/opt/qt5/lib/QtGamepad.framework/Headers -I/usr/local/opt/qt5/lib/QtGui.framework/Headers -I/usr/local/opt/qt5/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include -I'/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks (framework directory)' -F/usr/local/opt/qt5/lib src/CarrotQt5.h -o GeneratedFiles/Release/moc_CarrotQt5.cpp

GeneratedFiles/Release/moc_Player.cpp: /usr/local/include/SFML/Graphics.hpp \
		src/actor/InteractiveActor.h \
		src/actor/CommonActor.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QObject \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QKeyEvent \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QBitArray \
		/usr/local/include/SFML/System.hpp \
		src/gamestate/AnimationUser.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QString \
		src/gamestate/TimerUser.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QVector \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPair \
		src/gamestate/LevelManager.h \
		src/EngineState.h \
		src/struct/Controls.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMap \
		src/graphics/CarrotCanvas.h \
		/usr/local/opt/qt5/lib/QtWidgets.framework/Headers/QWidget \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPoint \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSize \
		/usr/local/include/SFML/Window.hpp \
		src/graphics/QSFMLCanvas.h \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEngine \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QShowEvent \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEvent \
		src/struct/CoordinatePair.h \
		src/struct/Hitbox.h \
		src/struct/NextLevelData.h \
		src/struct/Constants.h \
		src/struct/WeaponTypes.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QtGlobal \
		src/struct/Resources.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSet \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMultiMap \
		$(BASS_DIR)/bass.h \
		src/struct/AnimState.h \
		src/struct/DebugConfig.h \
		src/struct/Layers.h \
		src/actor/lighting/RadialLightSource.h \
		src/actor/LightSource.h \
		src/gamestate/PlayerOSD.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QVariant \
		src/actor/MovingPlatform.h \
		src/actor/SolidObject.h \
		src/actor/collectible/GemCollectible.h \
		src/actor/collectible/Collectible.h \
		src/actor/collectible/CoinCollectible.h \
		src/actor/Player.h \
		/usr/local/opt/qt5/bin/moc
	/usr/local/opt/qt5/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -D__APPLE_CC__ -I/usr/local/opt/qt5/mkspecs/macx-clang -I/Users/soulweaver/Documents/project-carrot -I'/Users/soulweaver/Documents/project-carrot/$(BASS_DIR)' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)/C' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)' -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles -I/Users/soulweaver/Documents/project-carrot -I/usr/local/include -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles/Release -I/usr/local/opt/qt5/lib/QtWidgets.framework/Headers -I/usr/local/opt/qt5/lib/QtGamepad.framework/Headers -I/usr/local/opt/qt5/lib/QtGui.framework/Headers -I/usr/local/opt/qt5/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include -I'/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks (framework directory)' -F/usr/local/opt/qt5/lib src/actor/Player.h -o GeneratedFiles/Release/moc_Player.cpp

GeneratedFiles/Release/moc_LevelManager.cpp: /usr/local/opt/qt5/lib/QtCore.framework/Headers/QVector \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QString \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QObject \
		src/EngineState.h \
		src/struct/Controls.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPair \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMap \
		/usr/local/include/SFML/Graphics.hpp \
		src/graphics/CarrotCanvas.h \
		/usr/local/opt/qt5/lib/QtWidgets.framework/Headers/QWidget \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QPoint \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSize \
		/usr/local/include/SFML/Window.hpp \
		src/graphics/QSFMLCanvas.h \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEngine \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QShowEvent \
		/usr/local/opt/qt5/lib/QtGui.framework/Headers/QPaintEvent \
		src/struct/CoordinatePair.h \
		src/struct/Hitbox.h \
		src/struct/NextLevelData.h \
		src/struct/Constants.h \
		src/struct/WeaponTypes.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QtGlobal \
		src/struct/Resources.h \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QSet \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QMultiMap \
		/usr/local/opt/qt5/lib/QtCore.framework/Headers/QBitArray \
		/usr/local/include/SFML/System.hpp \
		$(BASS_DIR)/bass.h \
		src/struct/AnimState.h \
		src/struct/DebugConfig.h \
		src/gamestate/TimerUser.h \
		src/gamestate/LevelManager.h \
		/usr/local/opt/qt5/bin/moc
	/usr/local/opt/qt5/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -D__APPLE_CC__ -I/usr/local/opt/qt5/mkspecs/macx-clang -I/Users/soulweaver/Documents/project-carrot -I'/Users/soulweaver/Documents/project-carrot/$(BASS_DIR)' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)/C' -I'/Users/soulweaver/Documents/project-carrot/$(BASS_FX_DIR)' -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles -I/Users/soulweaver/Documents/project-carrot -I/usr/local/include -I/Users/soulweaver/Documents/project-carrot/GeneratedFiles/Release -I/usr/local/opt/qt5/lib/QtWidgets.framework/Headers -I/usr/local/opt/qt5/lib/QtGamepad.framework/Headers -I/usr/local/opt/qt5/lib/QtGui.framework/Headers -I/usr/local/opt/qt5/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include -I'/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks (framework directory)' -F/usr/local/opt/qt5/lib src/gamestate/LevelManager.h -o GeneratedFiles/Release/moc_LevelManager.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: GeneratedFiles/ui_AboutCarrotDialog.h GeneratedFiles/ui_CarrotQt5.h
compiler_uic_clean:
	-$(DEL_FILE) GeneratedFiles/ui_AboutCarrotDialog.h GeneratedFiles/ui_CarrotQt5.h
GeneratedFiles/ui_AboutCarrotDialog.h: src/ui/AboutCarrotDialog.ui \
		/usr/local/opt/qt5/bin/uic
	/usr/local/opt/qt5/bin/uic src/ui/AboutCarrotDialog.ui -o GeneratedFiles/ui_AboutCarrotDialog.h

GeneratedFiles/ui_CarrotQt5.h: src/ui/CarrotQt5.ui \
		/usr/local/opt/qt5/bin/uic
	/usr/local/opt/qt5/bin/uic src/ui/CarrotQt5.ui -o GeneratedFiles/ui_CarrotQt5.h

compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_rcc_clean compiler_moc_header_clean compiler_uic_clean 

