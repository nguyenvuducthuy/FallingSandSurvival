#pragma once

//#define b2_maxTranslation 10.0f
//#define b2_maxTranslationSquared (b2_maxTranslation * b2_maxTranslation)

#include "Macros.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL_gpu.h>
#include <iostream>
#include <algorithm> 
#include <unordered_map>

#include "Networking.h"

#include "lib/sparsehash/sparse_hash_map.h"
#ifndef INC_World
#include "world.h"
#endif
#include "UTime.h"
#include "Textures.h"
#include "Background.h"
#include <box2d/b2_distance_joint.h>
#include "Settings.h"
#include "UI.h"
#include "Controls.h"
#include <chrono>
#include <thread>
#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include "Drawing.h"
#include "Shaders.h"

#if BUILD_WITH_STEAM
#include "steam_api.h"
#endif

#if BUILD_WITH_DISCORD
#include "discord.h"
#endif

#include "ProfilerConfig.h"

enum GameState {
    MAIN_MENU,
    LOADING,
    INGAME
};

class Game {
public:

    GameState state = LOADING;
    GameState stateAfterLoad = MAIN_MENU;
    int networkMode = -1;
    Client* client = nullptr;
    Server* server = nullptr;

    bool discordAPI = false;
    #if BUILD_WITH_DISCORD
    discord::Core* core {};
    #endif

    bool steamAPI = false;
    #if BUILD_WITH_STEAM
    void SteamHookMessages();
    #endif

    CAudioEngine audioEngine;

    const int WIDTH = 1200;
    const int HEIGHT = 800;

    int scale = 4;

    int ofsX = 0;
    int ofsY = 0;

    float plPosX = 0;
    float plPosY = 0;

    float camX = 0;
    float camY = 0;

    float desCamX = 0;
    float desCamY = 0;

    float freeCamX = 0;
    float freeCamY = 0;

    #define frameTimeNum 100
    uint16_t* frameTime = new uint16_t[frameTimeNum];

    TTF_Font* font64 = nullptr;
    TTF_Font* font16 = nullptr;
    TTF_Font* font14 = nullptr;

    SDL_Window* window = nullptr;

    GPU_Target* realTarget = nullptr;
    GPU_Target* target = nullptr;

    GPU_Image* backgroundImage = nullptr;

    GPU_Image* loadingTexture = nullptr;
    vector< unsigned char > pixelsLoading;
    unsigned char* pixelsLoading_ar = nullptr;
    int loadingScreenW = 0;
    int loadingScreenH = 0;

    GPU_Image* worldTexture = nullptr;
    GPU_Image* lightingTexture = nullptr;

    GPU_Image* texture = nullptr;
    vector< unsigned char > pixels;
    unsigned char* pixels_ar = nullptr;
    GPU_Image* textureLayer2 = nullptr;
    vector< unsigned char > pixelsLayer2;
    unsigned char* pixelsLayer2_ar = nullptr;
    GPU_Image* textureBackground = nullptr;
    vector< unsigned char > pixelsBackground;
    unsigned char* pixelsBackground_ar = nullptr;
    GPU_Image* textureObjects = nullptr;
    vector< unsigned char > pixelsObjects;
    unsigned char* pixelsObjects_ar = nullptr;
    GPU_Image* textureObjectsBack = nullptr;
    GPU_Image* textureParticles = nullptr;
    vector< unsigned char > pixelsParticles;
    unsigned char* pixelsParticles_ar = nullptr;
    GPU_Image* textureEntities = nullptr;

    GPU_Image* textureFire = nullptr;
    GPU_Image* texture2Fire = nullptr;
    vector< unsigned char > pixelsFire;
    unsigned char* pixelsFire_ar = nullptr;

    GPU_Image* temperatureMap = nullptr;
    vector< unsigned char > pixelsTemp;
    unsigned char* pixelsTemp_ar = nullptr;

    int ent_prevLoadZoneX = 0;
    int ent_prevLoadZoneY = 0;
    ctpl::thread_pool* updateDirtyPool = nullptr;
    ctpl::thread_pool* rotateVectorsPool = nullptr;

    GPU_Image* lightMap = nullptr;

    GPU_Image* lightTex = nullptr;
    SDL_Surface* lightSurf = nullptr;

    vector<UI*> uis;
    UI* debugUI = nullptr;
    UI* debugDrawUI = nullptr;
    int brushSize = 5;
    UILabel* brushSizeLabel = nullptr;
    UI* mainMenuUI = nullptr;

    UI* chiselUI = nullptr;

    int tickTime = 0;

    bool running = true;

    World* world = nullptr;

    float accLoadX = 0;
    float accLoadY = 0;

    int mx = 0;
    int my = 0;

    bool* objectDelete = nullptr;

    Material* selectedMaterial = &Materials::GENERIC_AIR;

    Uint32 water_shader = 0;
    GPU_ShaderBlock water_shader_block {};

    RaycastLightingShader* raycastLightingShader = nullptr;
    SimpleLightingShader*  simpleLightingShader = nullptr;
    SimpleLighting2Shader* simpleLighting2Shader = nullptr;
    FireShader* fireShader = nullptr;
    Fire2Shader* fire2Shader = nullptr;

    int init(int argc, char *argv[]);

    int fps = 0;
    long long lastTime = 0;
    long long lastTick = 0;
    long long lastLoadingTick = 0;
    long long now = 0;
    long long startTime = 0;
    long mspt = 0;
    uint32 loadingOnColor = 0;
    uint32 loadingOffColor = 0;

    DrawTextParams dt_versionInfo1;
    DrawTextParams dt_versionInfo2;
    DrawTextParams dt_versionInfo3;

    DrawTextParams dt_fps;

    DrawTextParams dt_frameGraph[5];
    DrawTextParams dt_loading;

    int run(int argc, char *argv[]);

    void updateFrameEarly();
    void tick();
    void updateFrameLate();

    void renderEarly();
    void renderLate();

    void renderLightmap(World* world);
    void renderTemperatureMap(World* world);

    int getAimSolidSurface(int dist);
    int getAimSurface(int dist);

    long long fadeInStart = 0;
    long long fadeInLength = 0;
    int fadeInWaitFrames = 0;

    long long fadeOutStart = 0;
    long long fadeOutLength = 0;
    int fadeOutWaitFrames = 0;
    std::function<void()> fadeOutCallback = []() {};

    string gameDir;
    string getFileInGameDir(string filePathRel);
    string getWorldDir(string worldName);

};
