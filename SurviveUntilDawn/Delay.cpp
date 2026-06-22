/**********************************************************************************
// Delay (C�digo Fonte)
//
// Cria��o:     02 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Sincroniza uma a��o
//
**********************************************************************************/

#include "Delay.h"
#include "SurviveUntilDawn.h"
#include "WaveOgre.h"
#include "WaveGoblin.h"
#include "WaveWizard.h"
#include "WaveDragon.h"
#include "Hud.h"

// ------------------------------------------------------------------------------

Delay::Delay()
{
    logo = new Sprite("Resources/Logo.png");
    timer.Start();

    notPlayed = true;
    fase1 = false;
    fase2 = false;
    fase3 = false;
    fase4 = false;
}

// ------------------------------------------------------------------------------

Delay::~Delay()
{
    delete logo;
}

// -------------------------------------------------------------------------------

void Delay::Update()
{
    if (notPlayed && timer.Elapsed(2.0f))
    {
        // toca �udio de introdu��o
        SurviveUntilDawn::audio->Play(START);
        notPlayed = false;
    }

    if (!fase1 && timer.Elapsed(6.0f))
    {
        // toca m�sica do jogo
        SurviveUntilDawn::audio->Play(THEME, true);
        SurviveUntilDawn::scene->Add(new WaveOgre(), STATIC);
        SurviveUntilDawn::viewHUD = true;
        fase1 = true;
    }

    if (!fase2 && timer.Elapsed(8.0f))
    {
        SurviveUntilDawn::scene->Add(new WaveGoblin(), STATIC);
        fase2 = true;
    }

    if (!fase3 && timer.Elapsed(10.0f))
    {
        SurviveUntilDawn::scene->Add(new WaveWizard(), STATIC);
        fase3 = true;
    }

    if (!fase4 && timer.Elapsed(15.0f))
    {
        SurviveUntilDawn::scene->Add(new WaveDragon(), STATIC);
        SurviveUntilDawn::scene->Delete();
        fase4 = true;
    }
}

// -------------------------------------------------------------------------------

void Delay::Draw()
{
    if (!fase1)
        logo->Draw(game->viewport.left + window->CenterX() , game->viewport.top + window->CenterY(), Layer::FRONT);
}

// -------------------------------------------------------------------------------
