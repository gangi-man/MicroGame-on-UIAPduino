#pragma once
#include "Display.h"


class DinoStage {
    struct Box {
        int16_t x;
        int8_t y;
        bool enabled;

        Box() { enabled = false; }
    };

    static const uint8_t BoxNumber = 16;
    static const int16_t GenerateMinFrameGap = 16;
    static const int16_t GenerateMaxFrameGap = 128;
    

    Box m_boxList[BoxNumber];
    int16_t m_frameCount;
    int16_t m_lastGenerateFrame;
    int8_t m_speedPerFrame8;
    uint8_t m_generateRatio;
    
    void updateLevel();
    void generateBox();
    void updateBoxPositions(Display* display);
public:
    DinoStage();
    void initialize();
    void update(Display* display);

    bool checkCollision(uint8_t x, uint8_t altitude);
};


class Dinosaur {
    const int16_t AccelG = 10;
    const uint8_t DinosaurX = 4;
    enum Style {RunA, RunB, DuckA, DuckB };
    
    unsigned int m_distance;
    int16_t m_altitude;
    int16_t m_frameCount;
    int16_t m_velocity;
    Style m_style;

    bool m_onJump;
    bool m_ducking;
    Display* m_display;

    DinoStage m_stage;

    void checkButton();
    void updateDisplay();
    void updateStyle();
    uint8_t* styleToSprite();
    
public:
    void initialize(Display* display);
    void update();
    void gameOver();
};
