/**********************************************************************************
// Hud (C�digo Fonte)
//
// Cria��o:     02 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Heads Up Display
//
**********************************************************************************/

#include "Hud.h"
#include "SurviveUntilDawn.h"

// ------------------------------------------------------------------------------

// inicializa contadores
uint Hud::missiles = 0;
uint Hud::ogres = 0;
uint Hud::goblins = 0;
uint Hud::wizards = 0;
uint Hud::dragons = 0;
uint Hud::particles = 0;
uint Hud::xpOrbs = 0;
uint Hud::playerXP = 0;
uint Hud::playerLevel = 1;

// ------------------------------------------------------------------------------

Hud::Hud()
{
    // cria fonte para exibi��o de texto
    font = new Font("Resources/Tahoma14.png");
    font->Spacing("Resources/Tahoma14.dat");
    bold = new Font("Resources/Tahoma14b.png");
    bold->Spacing("Resources/Tahoma14b.dat");

    // carrega sprites
    infoBox = new Sprite("Resources/InfoBox.png");

    // inicializa contador de frames e tempo transcorrido
    frameCount = 0;
    totalTime = 0.0f;
    fps = 0;
}

// ------------------------------------------------------------------------------

Hud::~Hud()
{
    delete font;
    delete bold;
    delete infoBox;
}

// -------------------------------------------------------------------------------

void Hud::Update()
{
    // tempo acumulado dos frames
    totalTime += gameTime;

    // incrementa contador de frames
    frameCount++;

    // a cada 1000ms (1 segundo) atualiza indicador de FPS
    if (totalTime >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        totalTime -= 1.0f;
    }
}

// -------------------------------------------------------------------------------

void Hud::Draw()
{
    // desenha elementos da interface
    infoBox->Draw(game->viewport.left + 140, game->viewport.top + 100, Layer::FRONT);

    // define cor do texto
    Color textColor{ 0.7f, 0.7f, 0.7f, 1.0f };

    // desenha texto
    text.str("");
    text << "Survive Until Dawn";
    bold->Draw(40, 62, text.str(), textColor);

    text.str("");
    text << "FPS: " << fps;
    font->Draw(40, 92, text.str(), textColor);

    text.str("");
    text << "Part�culas: " << particles;
    font->Draw(40, 112, text.str(), textColor);

    text.str("");
    text << "Inimigos: " << ogres + goblins + wizards + dragons;
    font->Draw(40, 132, text.str(), textColor);

    text.str("");
    text << "M�sseis: " << missiles;
    font->Draw(40, 152, text.str(), textColor);

    text.str("");
    text << "N�vel: " << playerLevel;
    font->Draw(40, 172, text.str(), textColor);

    text.str("");
    text << "XP: " << playerXP;
    font->Draw(40, 192, text.str(), textColor);
}

// -------------------------------------------------------------------------------
