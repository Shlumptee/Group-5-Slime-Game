#include <iostream>

#include "Screen.h"
#include "GameManager.h"

#include "GameManager.h"
#include <cstring>
#include <cmath>


static void DrawMenuText(
    GamesEngineeringBase::Window& window,
    const char* text, int x, int y, int scale,
    unsigned char r, unsigned char g, unsigned char b)
{
    static const char* letters = "SLIMEGATRXYNOHD";

    static const unsigned char patterns[15][7] = {
        { 15,16,16,14,1,1,30 },    // S
        { 16,16,16,16,16,16,31 },  // L
        { 31,4,4,4,4,4,31 },       // I
        { 17,27,21,21,17,17,17 }, // M
        { 31,16,16,30,16,16,31 }, // E
        { 14,17,16,23,17,17,15 }, // G
        { 14,17,17,31,17,17,17 }, // A
        { 31,4,4,4,4,4,4 },       // T
        { 30,17,17,30,20,18,17 }, // R
        { 17,17,10,4,10,17,17 }, // X
        { 17,17,10,4,4,4,4 },     // Y
        { 17,25,21,19,17,17,17 },// N
        { 14,17,17,17,17,17,14 },// O
        { 17,17,17,31,17,17,17 },// H
        { 30,17,17,17,17,17,30 } // D
    };

    for (const char* p = text; *p; ++p)
    {
        const char* found = std::strchr(letters, *p);

        if (found)
        {
            int index = static_cast<int>(found - letters);

            for (int row = 0; row < 7; ++row)
            {
                for (int col = 0; col < 5; ++col)
                {
                    if (!(patterns[index][row] & (16 >> col)))
                        continue;

                    for (int dy = 0; dy < scale; ++dy)
                    {
                        for (int dx = 0; dx < scale; ++dx)
                        {
                            int px = x + col * scale + dx;
                            int py = y + row * scale + dy;

                            if (px >= 0 && py >= 0 &&
                                px < static_cast<int>(window.getWidth()) &&
                                py < static_cast<int>(window.getHeight()))
                            {
                                window.draw(px, py, r, g, b);
                            }
                        }
                    }
                }
            }
        }

        x += 6 * scale;
    }
}


// Draw a rectangular button
static void DrawMenuButton(
    GamesEngineeringBase::Window& window,
    int x, int y, int width, int height,
    unsigned char r, unsigned char g, unsigned char b)
{
    for (int py = y; py < y + height; ++py)
    {
        for (int px = x; px < x + width; ++px)
        {
            if (px >= 0 && py >= 0 &&
                px < static_cast<int>(window.getWidth()) &&
                py < static_cast<int>(window.getHeight()))
            {
                window.draw(px, py, r, g, b);
            }
        }
    }
}



// Standard init for loading images etc
void MenuScreen::Init()
{
	std::cout << "Menu Screen Loaded" << std::endl;


    wasMouseDown = window.mouseButtonPressed(
        GamesEngineeringBase::MouseLeft);

    if (!menuSlimeLoaded)
    {
        const char* paths[] = {
            "..\\Assets\\slime-spritesheet-50x50.png",
            "Assets\\slime-spritesheet-50x50.png",
            "..\\..\\Assets\\slime-spritesheet-50x50.png"
        };

        for (const char* path : paths)
        {
            if (menuSlime.Load(path))
            {
                menuSlimeLoaded = true;
                break;
            }
        }
    }

    bounceTimer = 0.0f;

    if (menuSlimeLoaded)
    {
        menuSlime.Spawn({ 512.0f, 355.0f });
    }

}

// Any inputs needed, add here

void MenuScreen::HandleInput()
{

    int mx = window.getMouseInWindowX();
    int my = window.getMouseInWindowY();

    bool mouseDown = window.mouseButtonPressed(
        GamesEngineeringBase::MouseLeft);


    if (mouseDown && !wasMouseDown)
    {
  
        if (mx >= 272 && mx < 412 &&
            my >= 395 && my < 450)
        {
            gameManager.GetDifficulty().SetDifficultyType(
                DifficultyType::Easy);
        }


        else if (mx >= 442 && mx < 582 &&
            my >= 395 && my < 450)
        {
            gameManager.GetDifficulty().SetDifficultyType(
                DifficultyType::Medium);
        }


        else if (mx >= 612 && mx < 752 &&
            my >= 395 && my < 450)
        {
            gameManager.GetDifficulty().SetDifficultyType(
                DifficultyType::Hard);
        }


        else if (mx >= 362 && mx < 662 &&
            my >= 480 && my < 540)
        {
            gameManager.SetState(GameState::Game);
            return;
        }


        else if (mx >= 362 && mx < 662 &&
            my >= 570 && my < 630)
        {
            gameManager.Quit();
            return;
        }
    }

    wasMouseDown = mouseDown;
}

// Update logic here
void MenuScreen::Update(float deltaTime)
{


    if (!menuSlimeLoaded)
        return;

    menuSlime.Update(deltaTime, { 0.0f, 0.0f });

    bounceTimer += deltaTime;

    float bounceY = 355.0f -
        std::abs(std::sin(bounceTimer * 3.0f)) * 25.0f;

    menuSlime.SetPosition({ 512.0f, bounceY });


}

// Draw calls here
void MenuScreen::Draw(GamesEngineeringBase::Window& window)
{

    int centerX = static_cast<int>(window.getWidth()) / 2;

    for (int y = 0; y < static_cast<int>(window.getHeight()); ++y)
    {
        for (int x = 0; x < static_cast<int>(window.getWidth()); ++x)
        {
            window.draw(x, y, 0, 0, 0);
        }
    }

    const char* title = "SLIME GAME";
    int titleScale = 7;
    int titleWidth = static_cast<int>(std::strlen(title)) * 6 * titleScale;

    DrawMenuText(window, title,
        centerX - titleWidth / 2, 175,
        titleScale, 255, 105, 70);

    // Draw the bouncing slime
    if (menuSlimeLoaded)
    {
        menuSlime.Draw(window, { 0.0f, 0.0f });
    }


    // Get the difficulty currently selected by the player
    DifficultyType selected =
        gameManager.GetDifficulty().GetDifficultyType();

    // EASY
    DrawMenuButton(window, 272, 395, 140, 55,
        selected == DifficultyType::Easy ? 255 : 53,
        selected == DifficultyType::Easy ? 180 : 53,
        selected == DifficultyType::Easy ? 55 : 53);

    // NORMAL
    DrawMenuButton(window, 442, 395, 140, 55,
        selected == DifficultyType::Medium ? 255 : 53,
        selected == DifficultyType::Medium ? 180 : 53,
        selected == DifficultyType::Medium ? 55 : 53);

    // HARD
    DrawMenuButton(window, 612, 395, 140, 55,
        selected == DifficultyType::Hard ? 255 : 53,
        selected == DifficultyType::Hard ? 180 : 53,
        selected == DifficultyType::Hard ? 55 : 53);


    DrawMenuText(window, "EASY", 306, 411, 3, 255, 255, 255);
    DrawMenuText(window, "NORMAL", 458, 411, 3, 255, 255, 255);
    DrawMenuText(window, "HARD", 646, 411, 3, 255, 255, 255);


    DrawMenuButton(window, 362, 480, 300, 60, 255, 105, 70);
    DrawMenuText(window, "START GAME", 413, 499, 3, 255, 255, 255);

    DrawMenuButton(window, 362, 570, 300, 60, 53, 53, 53);
    DrawMenuText(window, "EXIT GAME", 431, 589, 3, 255, 255, 255);

}

// Destroy pointers etc, any stuff that would cause a mem leak
void MenuScreen::Destroy()
{

}