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
    MoveTo(startX, startY);

    // deslocamento frontal baseado no angulo e nivel do cleave
    float offset = 30.0f + (cleaveLevel * 10.0f);
    float rad = angle * 3.14159f / 180.0f;
    Translate(cos(rad) * offset, -sin(rad) * offset);

    // raio da hitbox escala com o nivel do cleave
    float hitRadius = 25.0f + (cleaveLevel * 15.0f);
    BBox(new Circle(hitRadius));
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
    // auto-destroi apos 0.15s; o engine remove com seguranca durante a iteracao MOVING
    if (lifeTimer.Elapsed(0.15f))
        SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void SwordSlash::Draw()
{
}

// -------------------------------------------------------------------------------
