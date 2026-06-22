/**********************************************************************************
// Magnet (C�digo Fonte)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Coleta todos os orbs de XP na tela
//
**********************************************************************************/

#include "Magnet.h"
#include "SurviveUntilDawn.h"
#include "XPOrb.h"
#include <vector>

// ---------------------------------------------------------------------------------

Magnet::Magnet(float pX, float pY) : Pickup(pX, pY, new Sprite("Resources/Magnet.png"))
{
    type = MAGNET;
}

// ---------------------------------------------------------------------------------

Magnet::~Magnet()
{
}

// -------------------------------------------------------------------------------

void Magnet::OnCollect()
{
    // Prepara a cena para leitura
    SurviveUntilDawn::scene->Begin();

    // Pega o primeiro objeto
    Object* obj = SurviveUntilDawn::scene->Next();

    while (obj != nullptr)
    {
        if (obj->Type() == XPORB)
        {
            // Força a coleta e remove o Orb
            XPOrb* orb = (XPOrb*)obj;
            orb->OnCollect();
            // A remoção da cena já deve estar dentro do OnCollect do próprio XPOrb
        }

        // Pula para o próximo objeto da cena
        obj = SurviveUntilDawn::scene->Next();
    }
}

// -------------------------------------------------------------------------------
