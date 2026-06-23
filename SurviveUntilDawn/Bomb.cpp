/**********************************************************************************
// Bomb (Código Fonte)
//
// Criação:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Elimina todos os inimigos na tela
//
**********************************************************************************/

#include "Bomb.h"
#include "SurviveUntilDawn.h"
#include "Wizard.h"
#include "Dragon.h"
#include "Goblin.h"
#include "Ogre.h"

// ---------------------------------------------------------------------------------

Bomb::Bomb(float pX, float pY) : Pickup(pX, pY, new Sprite("Resources/Magenta.png"))
{
    type = BOMB;
    BBox(new Circle(10.0f));
}

// ---------------------------------------------------------------------------------

Bomb::~Bomb()
{
}

// -------------------------------------------------------------------------------

void Bomb::OnCollect()
{
    // a bomba usa OnCollision personalizado — OnCollect vazio
}

// -------------------------------------------------------------------------------

void Bomb::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
        isExploding = true;
        MoveTo(-9999.0f, -9999.0f); // move para fora da tela
    }
}

// -------------------------------------------------------------------------------

void Bomb::Update()
{
    if (isExploding)
    {
        // varre a cena e elimina inimigos num raio de 600px do jogador
        SurviveUntilDawn::scene->Begin();
        Object* obj = SurviveUntilDawn::scene->Next();
        while (obj != nullptr)
        {
            uint id = obj->Type();
            if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
            {
                float dist = Point::Distance(
                    Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y()),
                    Point(obj->X(), obj->Y()));
                if (dist < 600.0f)
                {
                    if (id == GOBLIN) ((Goblin*)obj)->Kill();
                    else if (id == OGRE) ((Ogre*)obj)->Kill();
                    else if (id == WIZARD) ((Wizard*)obj)->Kill();
                    else if (id == DRAGON) ((Dragon*)obj)->Kill();
                }
            }
            obj = SurviveUntilDawn::scene->Next();
        }
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// -------------------------------------------------------------------------------

void Bomb::Draw()
{
    if (!isExploding && sprite)
        sprite->Draw(x, y, Layer::LOWER, scale, rotation);
}

// -------------------------------------------------------------------------------
