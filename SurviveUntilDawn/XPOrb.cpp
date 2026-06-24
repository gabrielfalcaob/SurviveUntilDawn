/**********************************************************************************
// XPOrb (Código Fonte)
//
// Criação:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Orb de experiência coletável pelo jogador
//
**********************************************************************************/

#include "XPOrb.h"
#include "SurviveUntilDawn.h"
#include "Hud.h"
#include <cmath>

// ---------------------------------------------------------------------------------

XPOrb::XPOrb(float pX, float pY, uint val) : Pickup(pX, pY, new Sprite("Resources/Green.png"))
{
    amount = val;
    type = XPORB;
    BBox(new Circle(6.0f));

    // incrementa contador
    ++Hud::xpOrbs;
}

// ---------------------------------------------------------------------------------

XPOrb::~XPOrb()
{
    // decrementa contador
    --Hud::xpOrbs;
}

// -------------------------------------------------------------------------------

void XPOrb::OnCollect()
{
    // adiciona XP com bonus e recalcula nível
    uint oldLevel = Hud::playerLevel;
    Hud::playerXP += (uint)(amount * SurviveUntilDawn::player->xpMultiplier);
    Hud::playerLevel = (uint)(1 + sqrt(Hud::playerXP / 50.0f));
    if (Hud::playerLevel > oldLevel)
        SurviveUntilDawn::player->LevelUp();
}

// -------------------------------------------------------------------------------

void XPOrb::Update()
{
    // atração pelo ímã — move rapidamente em direção ao jogador
    if (SurviveUntilDawn::player->IsMagnetActive())
    {
        float dx = SurviveUntilDawn::player->X() - x;
        float dy = SurviveUntilDawn::player->Y() - y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist > 0.1f)
            Translate((dx / dist) * 300.0f * gameTime, (dy / dist) * 300.0f * gameTime);
    }
    else
    {
        // atrai o orb na direção do jogador se estiver próximo
        if (Point::Distance(Point(x, y), Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y())) < SurviveUntilDawn::player->pickupRadius)
        {
            Vector dir(Line::Angle(Point(x, y), Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y())), 1.0f);
            Translate(dir.XComponent() * 120.0f * gameTime, -dir.YComponent() * 120.0f * gameTime);
        }
    }
}

// -------------------------------------------------------------------------------

void XPOrb::Draw()
{
    if (sprite)
        sprite->Draw(x, y, Layer::LOWER, scale, rotation);
}

// -------------------------------------------------------------------------------
