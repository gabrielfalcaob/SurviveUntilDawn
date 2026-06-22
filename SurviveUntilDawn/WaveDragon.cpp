/**********************************************************************************
// WaveDragon (C�digo Fonte)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Dragon
//
**********************************************************************************/

#include "WaveDragon.h"
#include "SurviveUntilDawn.h"
#include "Dragon.h"
#include "Hud.h"

// ------------------------------------------------------------------------------

WaveDragon::WaveDragon() : secs(10.0f, 15.0f)
{
    // atraso para a pr�xima onda
    delay = secs.Rand();
}

// ------------------------------------------------------------------------------

WaveDragon::~WaveDragon()
{

}

// -------------------------------------------------------------------------------

void WaveDragon::Update()
{
    // contador de inimigos
    static uint counter = 8;

    // se passou o tempo de atraso
    if (timer.Elapsed(delay) && Hud::dragons < 16)
    {
        if (counter > 0)
        {
            // toca som de nova onda
            SurviveUntilDawn::audio->Play(DRAGON);

            // adiciona nova inimigo
            SurviveUntilDawn::scene->Add(new Dragon(50, 50, SurviveUntilDawn::player), MOVING);
            SurviveUntilDawn::scene->Add(new Dragon(game->Width() - 50, 50, SurviveUntilDawn::player), MOVING);
            SurviveUntilDawn::scene->Add(new Dragon(game->Width() - 50, game->Height() - 50, SurviveUntilDawn::player), MOVING);
            SurviveUntilDawn::scene->Add(new Dragon(50, game->Height() - 50, SurviveUntilDawn::player), MOVING);

            delay = 0.450f;
            timer.Start();
            --counter;
        }
        else
        {
            // nova onda
            counter = 8;
            delay = secs.Rand();
            timer.Start();
        }
    }
}

// -------------------------------------------------------------------------------

void WaveDragon::Draw()
{

}

// -------------------------------------------------------------------------------
