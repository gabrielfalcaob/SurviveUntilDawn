/**********************************************************************************
// Magnet (Código Fonte)
//
// Criação:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Atrai todos os orbs de XP na tela por 3 segundos
//
**********************************************************************************/

#include "Magnet.h"
#include "SurviveUntilDawn.h"
#include "Player.h"

// ---------------------------------------------------------------------------------

Magnet::Magnet(float pX, float pY) : Pickup(pX, pY, new Sprite("Resources/Magnet.png"))
{
    type = MAGNET;
    BBox(new Circle(10.0f));
}

// ---------------------------------------------------------------------------------

Magnet::~Magnet()
{
}

// -------------------------------------------------------------------------------

void Magnet::OnCollect()
{
    // o im� usa OnCollision personalizado — OnCollect vazio
}

// -------------------------------------------------------------------------------

void Magnet::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
        ((Player*)obj)->ActivateMagnet();
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// -------------------------------------------------------------------------------
