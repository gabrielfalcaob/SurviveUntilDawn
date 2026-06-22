/**********************************************************************************
// WaveWizard (C�digo Fonte)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Wizard
//
**********************************************************************************/

#include "WaveWizard.h"
#include "SurviveUntilDawn.h"
#include "Wizard.h"
#include "Hud.h"

// ------------------------------------------------------------------------------

WaveWizard::WaveWizard() : posX(0, window->Width()), posY(0, window->Height()), secs(8.0f, 12.0f)
{
    // posi��o dos inimigos
    pX = posX.Rand();
    pY = posY.Rand();

    // atraso para a pr�xima onda
    delay = secs.Rand();
}

// ------------------------------------------------------------------------------

WaveWizard::~WaveWizard()
{

}

// -------------------------------------------------------------------------------

void WaveWizard::Update()
{
    // contador de inimigos
    static uint counter = 8;

    // se passou o tempo de atraso
    if (timer.Elapsed(delay) && Hud::wizards < 15)
    {
        if (counter > 0)
        {
            // toca som de nova onda
            SurviveUntilDawn::audio->Play(WIZARD);

            // adiciona nova inimigo
            SurviveUntilDawn::scene->Add(new Wizard(pX, pY, SurviveUntilDawn::player), MOVING);

            delay = 0.850f;
            timer.Start();
            --counter;
        }
        else
        {
            // nova posi��o do inimigo
            pX = posX.Rand();
            pY = posY.Rand();

            // nova onda
            counter = 8;
            delay = secs.Rand();
            timer.Start();
        }
    }
}

// -------------------------------------------------------------------------------

void WaveWizard::Draw()
{

}

// -------------------------------------------------------------------------------
