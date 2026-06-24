/**********************************************************************************
// Lightning (Codigo Fonte)
//
// Criacao:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Raio da Bencao da Sorte - dano + lentidao
//
**********************************************************************************/

#include "Lightning.h"
#include "SurviveUntilDawn.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Wizard.h"
#include "Dragon.h"

// ---------------------------------------------------------------------------------

Lightning::Lightning(float startX, float startY)
{
    MoveTo(startX, startY);
    type = LIGHTNING;

    BBox(new Circle(20.0f));
    sprite = new Sprite("Resources/Explo.png");
    lifeTimer.Start();
}

// ---------------------------------------------------------------------------------

Lightning::~Lightning()
{
    delete sprite;
}

// ---------------------------------------------------------------------------------

void Lightning::OnCollision(Object* obj)
{
    uint id = obj->Type();

    if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
    {
        float mult = SurviveUntilDawn::player->globalDamageMultiplier;

        // aplica lentidao ANTES do dano (evita use-after-free)
        if (id == GOBLIN)
            ((Goblin*)obj)->ApplySlow(0.4f, 2.5f);
        else if (id == OGRE)
        {
            ((Ogre*)obj)->ApplySlow(0.4f, 2.5f);
            int dmg = (int)mult;
            if (dmg < 1) dmg = 1;
            for (int i = 0; i < dmg; i++)
                ((Ogre*)obj)->Kill();
        }
        else if (id == WIZARD)
            ((Wizard*)obj)->ApplySlow(0.4f, 2.5f);
        else if (id == DRAGON)
            ((Dragon*)obj)->ApplySlow(0.4f, 2.5f);

        // aplica dano em inimigos de um hit
        if (id == GOBLIN)
            ((Goblin*)obj)->Kill();
        else if (id == WIZARD)
            ((Wizard*)obj)->Kill();
        else if (id == DRAGON)
            ((Dragon*)obj)->Kill();
    }
}

// ---------------------------------------------------------------------------------

void Lightning::Update()
{
    if (lifeTimer.Elapsed(0.25f))
    {
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// ---------------------------------------------------------------------------------

void Lightning::Draw()
{
    sprite->Draw(x, y, Layer::FRONT, 0.5f, 0.0f, { 1.0f, 1.0f, 0.0f, 1.0f });
}

// ---------------------------------------------------------------------------------
