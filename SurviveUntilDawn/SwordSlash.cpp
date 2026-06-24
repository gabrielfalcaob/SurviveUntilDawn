/**********************************************************************************
// SwordSlash (C�digo Fonte)
//
// Cria��o:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Hitbox f�sica do ataque do jogador
//
**********************************************************************************/

#include "SwordSlash.h"
#include "SurviveUntilDawn.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Wizard.h"
#include "Dragon.h"
#include <cmath>

// ---------------------------------------------------------------------------------

SwordSlash::SwordSlash(float startX, float startY, float angle, int cleaveLevel)
{
    this->cleaveLevel = cleaveLevel;

    MoveTo(startX, startY);
    type = SWORDSLASH;
    lifeTimer.Start();

    // hitbox retangular (largura e altura escalam com o nivel)
    float hw = 25.0f + (cleaveLevel * 5.0f);   // meia largura
    float hh = 35.0f + (cleaveLevel * 5.0f);   // meia altura
    BBox(new Rect(-hw, -hh, hw, hh));
}

// ---------------------------------------------------------------------------------

SwordSlash::~SwordSlash()
{
}

// -------------------------------------------------------------------------------

void SwordSlash::OnCollision(Object* obj)
{
    uint id = obj->Type();

    float mult = SurviveUntilDawn::player->globalDamageMultiplier;

    // nao deleta a si mesmo -- a espada atravessa e varre inimigos
    if (id == GOBLIN)
        ((Goblin*)obj)->Kill();
    else if (id == OGRE)
    {
        int dmg = (int)mult;
        if (dmg < 1) dmg = 1;
        for (int i = 0; i < dmg; i++)
            ((Ogre*)obj)->Kill();
    }
    else if (id == WIZARD)
        ((Wizard*)obj)->Kill();
    else if (id == DRAGON)
        ((Dragon*)obj)->Kill();
}

// -------------------------------------------------------------------------------

void SwordSlash::Update()
{
    Player* p = SurviveUntilDawn::player;

    // deslocamento frontal baseado na direcao do jogador
    float offsetX = 40.0f + (cleaveLevel * 8.0f);
    if (p->IsFacingLeft())
        offsetX = -offsetX;

    // segue o jogador a cada frame com o offset direcional
    MoveTo(p->X() + offsetX, p->Y());

    // auto-destroi apos 0.15s
    if (lifeTimer.Elapsed(0.15f))
        SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void SwordSlash::Draw()
{
}

// -------------------------------------------------------------------------------
