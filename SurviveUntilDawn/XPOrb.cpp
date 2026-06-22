/**********************************************************************************
// XPOrb (C�digo Fonte)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Orb de experi�ncia colet�vel pelo jogador
//
**********************************************************************************/

#include "XPOrb.h"
#include "SurviveUntilDawn.h"
#include "Hud.h"
#include <cmath>

// ---------------------------------------------------------------------------------

XPOrb::XPOrb(float pX, float pY, uint val) : Pickup(pX, pY, new Sprite("Resources/XPOrb.png"))
{
    amount = val;
    type = XPORB;

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
    // adiciona XP e recalcula n�vel
    Hud::playerXP += amount;
    Hud::playerLevel = (uint)(1 + sqrt(Hud::playerXP / 50.0f));
}

// -------------------------------------------------------------------------------

void XPOrb::Update()
{
    // atrai o orb na dire��o do jogador se estiver pr�ximo
    if (Point::Distance(Point(x, y), Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y())) < 80.0f)
    {
        Vector dir(Line::Angle(Point(x, y), Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y())), 1.0f);
        Translate(dir.XComponent() * 120.0f * gameTime, -dir.YComponent() * 120.0f * gameTime);
    }
}

// -------------------------------------------------------------------------------
