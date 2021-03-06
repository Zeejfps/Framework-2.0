############################### README ###############################
# 1. Uncoment the OGRE_PATH, BOOST_PATH, and TINYXML_PATH variables
# 2. Commend my OGRE_PATH and BOOST_PATH
# 3. Uncoment LDFLAGS
# 4. Uncoment the -I include for tiny xml and -L
# 5. ...
# 6. Profit!
######################################################################

CXX := g++
CXXFLAGS := -Wall -c
LDFLAGS := -lboost_system -llua -lOgreMain -ltinyxml -lOIS -lbass -lBulletDynamics -lBulletCollision -lLinearMath -lCEGUIBase-0 -lCEGUIOgreRenderer-0
#LDFLAGS := -lboost_system-mgw51-mt-1_63 -lOgreMain -ltinyxml -lOIS -lbass -lCEGUIBase

############## PATHS FOR WINDOWS ###############
################################################
#OGRE_PATH := J:/Game_Engine/Ogre-1.8.1
#BOOST_PATH := J:/Game_Engine/boost_1_63_0
#TINYXML_PATH := J:/TDM-GCC-64/tinyxml_2.6.2
#OIS_PATH := J:/Game_Engine/ois
#BASS_PATH := J:/Game_Engine/bass-2.4
################################################

OGRE_PATH := /home/zeejfps/Programming/Game-Engine/libs/ogre_src_v1-8-1
BOOST_PATH := /home/zeejfps/Programming/Game-Engine/libs/boost_1_63_0
OIS_PATH := /home/zeejfps/Programming/Game-Engine/libs/lib_ois
BASS_PATH := /home/zeejfps/Programming/Game-Engine/libs/bass24
CEGUI_PATH := /home/zeejfps/Programming/Game-Engine/libs/cegui-0.8.7
BULLET_PATH := /home/zeejfps/Programming/Game-Engine/libs/bullet3-2.85.1
LUA_PATH := /home/zeejfps/Programming/Game-Engine/libs/lua-5.3.4
LUA_BRIDGE_PATH := /home/zeejfps/Programming/Game-Engine/libs/LuaBridge

EXECUTABLE := ZsAwesomeGame9000
OBJ_DIR := out
SRC_DIR := src
INC_DIR := -I$(SRC_DIR)/include -I$(OIS_PATH)/include -I$(OGRE_PATH)/OgreMain/include -I$(BOOST_PATH) -I$(OGRE_PATH)/build/include -I$(BASS_PATH)/include -I$(CEGUI_PATH)/include -I$(BULLET_PATH)/include -I$(LUA_PATH)/src -I$(LUA_BRIDGE_PATH) #-I$(TINYXML_PATH)/include
LIB_DIR := -L$(OBJ_DIR) -L$(OIS_PATH)/lib -L$(OGRE_PATH)/build/lib -L$(BOOST_PATH)/stage/lib -L$(BASS_PATH)/lib -L$(CEGUI_PATH)/lib -L$(BULLET_PATH)/lib -L/usr/local/lua #-L$(TINYXML_PATH)/lib

SOURCES := main.cpp MyGame.cpp OgreGame.cpp OgreSceneParser.cpp OISInputHandler.cpp BassAudioPlayer.cpp PhysicsHandler.cpp DebugDraw.cpp DynamicLineDrawer.cpp
OBJECTS := $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)


$(EXECUTABLE) : $(OBJ_DIR) $(OBJECTS)
	$(CXX) $(LIB_DIR) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS)


run: $(EXECUTABLE)
	export LD_LIBRARY_PATH=$(OGRE_PATH)/build/lib:$(BASS_PATH)/lib:$(CEGUI_PATH)/lib && ./$(EXECUTABLE)


$(OBJ_DIR):
	@mkdir $(OBJ_DIR)


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) -MMD $(CXXFLAGS) $(INC_DIR) $< -o $@


ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:%.cpp=$(OBJ_DIR)/%.d)
endif


.PHONY: clean
clean:
	@rm -f $(OBJ_DIR)/*.o $(EXECUTABLE) Ogre.log *$(OBJ_DIR)/*.d
