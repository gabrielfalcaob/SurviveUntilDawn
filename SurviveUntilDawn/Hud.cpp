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
#include "Engine.h"
#include <cmath>

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
    // -------------------------------
    // Debug HUD (toggle com H)
    // -------------------------------
    if (SurviveUntilDawn::viewHUD)
    {
        Color textColor{ 0.7f, 0.7f, 0.7f, 1.0f };

        text.str("");
        text << "Survive Until Dawn";
        bold->Draw(40, 62, text.str(), textColor);

        text.str("");
        text << "FPS: " << fps;
        font->Draw(window->Width() - 150, 20, text.str(), textColor);

        text.str("");
        text << "Nivel: " << playerLevel;
        font->Draw(40, 172, text.str(), textColor);

        text.str("");
        text << "XP: " << playerXP;
        font->Draw(40, 192, text.str(), textColor);
    }

    // -------------------------------
    // Barras de HP e XP (sempre visiveis)
    // -------------------------------
    if (playerRef != nullptr)
    {
        // compensacao da camera para desenhar em espaco de tela
        float camX = game->viewport.left;
        float camY = game->viewport.top;

        float cx = (window->Width() / 2.0f) + camX;
        float barW = 400.0f;
        float barH = 14.0f;
        float halfW = barW / 2.0f;
        float halfH = barH / 2.0f;

        // calculo da barra de XP
        float xpY = (window->Height() - 20.0f) + camY;
        float xpThisLvl = 50.0f * (Hud::playerLevel - 1) * (Hud::playerLevel - 1);
        float xpNextLvl = 50.0f * Hud::playerLevel * Hud::playerLevel;
        float xpRange = xpNextLvl - xpThisLvl;
        float xpProg = (float)Hud::playerXP - xpThisLvl;
        float xpPct = (xpRange > 0) ? (xpProg / xpRange) : 0.0f;
        if (xpPct < 0) xpPct = 0;
        if (xpPct > 1) xpPct = 1;

        // calculo da barra de HP
        float hpY = (window->Height() - 45.0f) + camY;
        float hpPct = (float)playerRef->GetHp() / (float)playerRef->GetMaxHp();
        if (hpPct < 0) hpPct = 0;

        // desenha as barras via renderizador de pixels
        Rect xpBg(cx - halfW, xpY - halfH, cx + halfW, xpY + halfH);
        Rect xpFill(cx - halfW, xpY - halfH, cx - halfW + xpPct * barW, xpY + halfH);
        Rect hpBg(cx - halfW, hpY - halfH, cx + halfW, hpY + halfH);
        Rect hpFill(cx - halfW, hpY - halfH, cx - halfW + hpPct * barW, hpY + halfH);

        Engine::renderer->BeginPixels();
        Engine::renderer->Draw(&xpBg, 0xFF444444);
        Engine::renderer->Draw(&xpFill, 0xFF33FF33);
        Engine::renderer->Draw(&hpBg, 0xFF444444);
        Engine::renderer->Draw(&hpFill, 0xFFFF3333);
        Engine::renderer->EndPixels();

        // texto "HP: X / Y" centralizado sobre a barra de HP
        text.str("");
        text << "HP: " << playerRef->GetHp() << " / " << playerRef->GetMaxHp();
        float estW = (float)(text.str().length()) * 8.0f;
        font->Draw(cx - estW / 2.0f, hpY - 4, text.str(), { 1,1,1,1 });
    }

    // -------------------------------
    // Indicadores de Cooldown (E e R)
    // -------------------------------
    float bh = window->Height();

    // Raio (Tecla E) — canto inferior esquerdo
    if (playerRef->GetLightningLevel() > 0)
    {
        float tempo = playerRef->GetLightningTime();
        text.str("");
        text << "RAIO [E]: ";
        if (tempo >= 4.0f)
            text << "PRONTO!";
        else
            text << (int)(4.0f - tempo) << "s";
        font->Draw(40, bh - 80, text.str(), tempo >= 4.0f ? Color{ 1,1,0,1 } : Color{ 0.5f,0.5f,0.5f,1 });
    }

    // Onda de Choque (Tecla R) — canto inferior esquerdo
    if (playerRef->GetShockwaveLevel() > 0)
    {
        float tempo = playerRef->GetShockwaveTime();
        text.str("");
        text << "ONDA [R]: ";
        if (tempo >= 3.0f)
            text << "PRONTO!";
        else
            text << (int)(3.0f - tempo) << "s";
        font->Draw(40, bh - 40, text.str(), tempo >= 3.0f ? Color{ 0,1,1,1 } : Color{ 0.5f,0.5f,0.5f,1 });
    }
}

// -------------------------------------------------------------------------------
