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

// ---------------------------------------------------------------------------------

SwordSlash::SwordSlash(float startX, float startY, bool dir)
{
    facingRight = dir;
    MoveTo(startX, startY);
    BBox(new Circle(30.0f));
    type = SWORDSLASH;
    lifeTimer.Start();
}

// ---------------------------------------------------------------------------------

SwordSlash::~SwordSlash()
{
}

// -------------------------------------------------------------------------------

void SwordSlash::OnCollision(Object* obj)
{
    uint id = obj->Type();

    if (id == GOBLIN)
        ((Goblin*)obj)->Kill();
    else if (id == OGRE)
        ((Ogre*)obj)->Kill();
    else if (id == WIZARD)
        ((Wizard*)obj)->Kill();
    else if (id == DRAGON)
        ((Dragon*)obj)->Kill();
}

// -------------------------------------------------------------------------------

void SwordSlash::Update()
{
    // auto-destroi apos 0.15s; o engine remove com seguranca durante a iteracao MOVING
    if (lifeTimer.Elapsed(0.15f))
        SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void SwordSlash::Draw()
{
}

// -------------------------------------------------------------------------------
