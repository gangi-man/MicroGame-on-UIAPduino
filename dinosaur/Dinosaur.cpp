#include "Dinosaur.h"


DinoStage::DinoStage()
{
    initialize();
}

void DinoStage::initialize()
{
    m_frameCount = 0;
    m_lastGenerateFrame = 0;
    m_speedPerFrame8 = 10;
    m_generateRatio = 4; //  4/ 256

    for (uint8_t i=0; i<BoxNumber; i++)
        m_boxList[i].enabled = false;
}

void DinoStage::generateBox()
{
    if ((m_frameCount - m_lastGenerateFrame) < GenerateMinFrameGap)
        return;
    
    if ( ((m_frameCount - m_lastGenerateFrame) > GenerateMaxFrameGap) ||
         (random(255) < m_generateRatio) ) {
        m_lastGenerateFrame = m_frameCount;

        for (uint8_t i; i<BoxNumber; i++) {
            if (!m_boxList[i].enabled) {
                m_boxList[i].y = random(12) + 4;
                m_boxList[i].x = 60 * 8;
                m_boxList[i].enabled = true;
                break;
            }
        }
    }
}

bool DinoStage::checkCollision(uint8_t x, uint8_t y)
{
    for (uint8_t i=0; i<BoxNumber; i++) {
        if (!m_boxList[i].enabled)
            continue;

        Box& box = m_boxList[i];
        uint8_t xL = x+2;
        uint8_t xR = x+6;
        uint8_t XL = box.x/8+1;
        uint8_t XR = box.x/8+2;

        uint8_t y1 = y+2;
        uint8_t y2 = y+9;
        uint8_t Y1 = Display::Height-box.y+1;
        uint8_t Y2 = Display::Height-box.y+2;

        if ((xL <= XR) && (xR >= XL) &&
            (y1 <= Y2) && (y2 >= Y1))
            return true;
    }
    return false;
}

void DinoStage::updateLevel()
{
    if (m_frameCount % 2000) // distance 500
        return;

    uint8_t speedLevel = (m_frameCount / 2000) % 4;

    switch (speedLevel) {
    case 0:
        m_speedPerFrame8 = 14;
        break;
    case 1:
        m_speedPerFrame8 = 20;
        break;
    case 2:;
        m_speedPerFrame8 = 8;
        break;
    case 3:
        m_speedPerFrame8 = 10;
        break;
    default:
        m_speedPerFrame8 = 10;
    }

    m_generateRatio = 5; // 5/256
}

void DinoStage::update(Display* display)
{
    m_frameCount++;
    updateLevel();
    generateBox();
    updateBoxPositions(display);
    if (m_frameCount > 30000) {
        m_frameCount = 0;
        m_lastGenerateFrame = 0;
    }
}

void DinoStage::updateBoxPositions(Display* display)
{
    for (uint8_t i=0; i<BoxNumber; i++) {
        if (!m_boxList[i].enabled)
            continue;
        display->putSprite(m_boxList[i].x/8, Display::Height-m_boxList[i].y, PIX_BOX4, true);
        
        m_boxList[i].x -= m_speedPerFrame8;
        if (m_boxList[i].x < 0)
            m_boxList[i].enabled = false;
        else
            display->putSprite(m_boxList[i].x/8, Display::Height-m_boxList[i].y, PIX_BOX4);
    }
}


void Dinosaur::initialize(Display* display)
{
    m_distance = 0;
    m_altitude = 0;
    m_onJump = false;
    m_ducking = false;
    m_display = display;
    m_style = RunA;
}

void Dinosaur::gameOver()
{
    m_display->putSprite(2, 12, PIX_G);
    m_display->putSprite(10, 12, PIX_A);
    m_display->putSprite(18, 12, PIX_M);
    m_display->putSprite(26, 12, PIX_E);    
    m_display->putSprite(34, 12, PIX_O);
    m_display->putSprite(42, 12, PIX_V);
    m_display->putSprite(50, 12, PIX_E);
    m_display->putSprite(58, 12, PIX_R);    
    m_display->flush();
    while (digitalRead(PC3)) { }

    initialize(m_display);
    m_stage.initialize();
}

int16_t calcY(int16_t altitude) {
    int16_t y = 21 - altitude;

    if (y>21)
        y = 21;
    if (y<0)
        y = 0;
    return y;
}

void Dinosaur::updateStyle()
{
    if (m_style==RunA) {
        if (m_ducking)
            m_style=DuckB;
        else
            m_style=RunB;
    } else if (m_style==RunB) {
        if (m_ducking)
            m_style=DuckA;
        else
            m_style=RunA;
    } else if (m_style==DuckA) {
        if (m_ducking)
            m_style = DuckB;
        else
            m_style = RunA;
    } else if (m_style==DuckB) {
        if (m_ducking)
            m_style = DuckA;
        else
            m_style = RunB;
    }
}

uint8_t* Dinosaur::styleToSprite()
{
    switch (m_style) {
    case RunA: return PIX_MANA;
    case RunB: return PIX_MANB;
    case DuckA: return PIX_MANC;
    case DuckB: return PIX_MAND;
    default:
        m_style = RunA;
        return PIX_MANA;
    }
}

void Dinosaur::updateDisplay()
{
    int16_t altitudeTmp = m_altitude;
    if (m_onJump) {
        m_altitude += m_velocity;
        if (m_frameCount % 2)
            m_velocity--;

        if (m_altitude<=0) {
            m_altitude = 0;
            m_velocity = 0;
            m_onJump = false;
        }
    }

    int16_t y = calcY(altitudeTmp);
    m_display->putSprite(DinosaurX, y, styleToSprite(), true);
    
    updateStyle();
    y = calcY(m_altitude);
    m_display->putSprite(DinosaurX, y, styleToSprite(), false);

    m_display->showNumber(Display::Width - 6*4, 0, m_distance);
    m_display->flush();
}

void Dinosaur::update()
{
    if ((m_frameCount % 4) == 0 )
        m_distance++;
    if (m_distance > 30000)
        m_distance = 30000;
    m_frameCount++;
    checkButton();

    m_stage.update(m_display);
    if (m_stage.checkCollision(DinosaurX, calcY(m_altitude) + (m_ducking ? 4 : 0))) {
        gameOver();
        m_display->clear();
        m_display->flush();
        delay(1000);
    }

    updateDisplay();
}

void Dinosaur::checkButton()
{
    if (!digitalRead(PD0) && !m_onJump) {
        m_velocity = 4;
        m_onJump = true;
        m_ducking = false;
    }

    if (!digitalRead(PC5) && !m_onJump) {
        m_ducking = true;
    } else {
        m_ducking = false;
    }
}
