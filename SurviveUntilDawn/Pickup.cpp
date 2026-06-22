/**********************************************************************************
// Pickup (C�digo Fonte)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Classe base abstrata para itens colet�veis
//
**********************************************************************************/

#include "Pickup.h"
#include "SurviveUntilDawn.h"

// ---------------------------------------------------------------------------------

Pickup::Pickup(float pX, float pY, Sprite * spr)
{
    sprite = spr;
    BBox(new Circle(8.0f));
    MoveTo(pX, pY);
}

// ---------------------------------------------------------------------------------

Pickup::~Pickup()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void Pickup::OnCollision(Object * obj)
{
    if (obj->Type() == PLAYER)
    {
        OnCollect();
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// -------------------------------------------------------------------------------

void Pickup::Update()
{
}

// -------------------------------------------------------------------------------
