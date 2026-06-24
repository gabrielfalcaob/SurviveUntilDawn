/**********************************************************************************
// HealthDrop (Código Fonte)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Item coletável que cura o jogador
//
**********************************************************************************/

#include "HealthDrop.h"
#include "SurviveUntilDawn.h"
#include "Player.h"

// ---------------------------------------------------------------------------------

HealthDrop::HealthDrop(float pX, float pY) : Pickup(pX, pY, new Sprite("Resources/Orange.png"))
{
    type = HEALTHDROP;
    BBox(new Circle(10.0f));
}

// ---------------------------------------------------------------------------------

HealthDrop::~HealthDrop()
{
}

// -------------------------------------------------------------------------------

void HealthDrop::OnCollect()
{
    SurviveUntilDawn::audio->Play(HEAL);
    SurviveUntilDawn::player->Heal(1);
}

// -------------------------------------------------------------------------------

void HealthDrop::Draw()
{
    if (sprite)
        sprite->Draw(x, y, Layer::LOWER, scale, rotation);
}

// -------------------------------------------------------------------------------
