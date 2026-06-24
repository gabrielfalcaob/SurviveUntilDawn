/**********************************************************************************
// TrailFire (Codigo Fonte)
//
// Criacao:     24 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Rastro de fogo do poder orbital
//
**********************************************************************************/

#include "TrailFire.h"
#include "SurviveUntilDawn.h"

// ---------------------------------------------------------------------------------

TrailFire::TrailFire(float startX, float startY)
{
    MoveTo(startX, startY);
    type = 0; // sem colisao

    ts = new TileSet("Resources/Fire_02.png", 71, 64, 9, 9);
    anim = new Animation(ts, 0.05f, false);
    anim->Add(0, seq, 9);
    anim->Select(0);
}

// ---------------------------------------------------------------------------------

TrailFire::~TrailFire()
{
    delete anim;
    delete ts;
}

// ---------------------------------------------------------------------------------

void TrailFire::Update()
{
    anim->NextFrame();
    if (anim->Inactive())
        SurviveUntilDawn::scene->Delete(this, MOVING);
}

// ---------------------------------------------------------------------------------

void TrailFire::Draw()
{
    anim->Draw(x, y, Layer::LOWER, 0.6f, 0.0f);
}

// ---------------------------------------------------------------------------------
