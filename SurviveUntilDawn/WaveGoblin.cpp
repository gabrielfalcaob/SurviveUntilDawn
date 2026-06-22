/**********************************************************************************
// WaveGoblin (C�digo Fonte)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Goblin
//
**********************************************************************************/

#include "WaveGoblin.h"
#include "SurviveUntilDawn.h"
#include "Goblin.h"
#include "Hud.h"

// ------------------------------------------------------------------------------

WaveGoblin::WaveGoblin() : posX(50, window->Width()-50), posY(50, game->Height()-50), secs(2.0f, 4.0f)
{
    // posi��o dos inimigos
    pX = posX.Rand();
    pY = posY.Rand();

    // atraso para a pr�xima onda
    delay = secs.Rand();
}

// ------------------------------------------------------------------------------

WaveGoblin::~WaveGoblin()
{

}

// -------------------------------------------------------------------------------

void WaveGoblin::Update()
{
    // se passou o tempo de atraso
    if (timer.Elapsed(delay) && Hud::goblins < 5)
    {
        // toca som de nova onda
        SurviveUntilDawn::audio->Play(GOBLIN);

        // adiciona novo inimigo
        SurviveUntilDawn::scene->Add(new Goblin(pX, pY, SurviveUntilDawn::player), MOVING);

        // nova posi��o do inimigo
        pX = posX.Rand();
        pY = posY.Rand();

        // reinicia o timer
        timer.Start();
    }
}

// -------------------------------------------------------------------------------

void WaveGoblin::Draw()
{

}

// -------------------------------------------------------------------------------
