/**********************************************************************************
// Shockwave (Codigo Fonte)
//
// Criacao:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Onda de choque expansiva do jogador
//
**********************************************************************************/

#include "Shockwave.h"
#include "SurviveUntilDawn.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Wizard.h"
#include "Dragon.h"
#include <cmath>

// ---------------------------------------------------------------------------------

Shockwave::Shockwave(float startX, float startY, int level)
{
    MoveTo(startX, startY);
    type = SHOCKWAVE;

    currentRadius = 5.0f;
    maxRadius = 150.0f + (level * 50.0f);
    expansionSpeed = 200.0f;

    BBox(new Circle(currentRadius));
    ts = new TileSet("Resources/Explosion_01.png", 170, 192, 9, 9);
    anim = new Animation(ts, 0.08f, false);
    anim->Add(0, seq, 9);
    anim->Select(0);
}

// ---------------------------------------------------------------------------------

Shockwave::~Shockwave()
{
    delete anim;
    delete ts;
}

// ---------------------------------------------------------------------------------

void Shockwave::OnCollision(Object* obj)
{
    uint id = obj->Type();

    if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
    {
        // knockback leve: empurra o inimigo para longe do centro da onda
        float dx = obj->X() - x;
        float dy = obj->Y() - y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist > 0.1f)
        {
            dx = (dx / dist) * 20.0f;
            dy = (dy / dist) * 20.0f;
            obj->Translate(dx, dy);
        }

        float mult = SurviveUntilDawn::player->globalDamageMultiplier;

        // dano via Kill() de cada inimigo
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
}

// ---------------------------------------------------------------------------------

void Shockwave::Update()
{
    anim->NextFrame();

    // expande o raio
    currentRadius += expansionSpeed * gameTime;

    // atualiza a bounding box
    BBox(new Circle(currentRadius));

    // auto-destroi quando ultrapassa o raio maximo
    if (currentRadius >= maxRadius)
    {
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// ---------------------------------------------------------------------------------

void Shockwave::Draw()
{
    float scale = (currentRadius * 2.0f) / 170.0f;
    anim->Draw(x, y, Layer::UPPER, scale, 0.0f);
}

// ---------------------------------------------------------------------------------
