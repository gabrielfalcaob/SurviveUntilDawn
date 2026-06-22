/**********************************************************************************
// Goblin (C�digo Fonte)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz um persegui��o direta
//
**********************************************************************************/

#include "SurviveUntilDawn.h"
#include "Goblin.h"
#include "Random.h"
#include "Hud.h"
#include "Explosion.h"
#include "XPOrb.h"
#include "Magnet.h"
#include "Bomb.h"

// ---------------------------------------------------------------------------------

Goblin::Goblin(float pX, float pY, Player * p)
{
    player = p;
    sprite = new Sprite(SurviveUntilDawn::goblin);
    BBox(new Circle(18.0f));

    speed.RotateTo(0);
    speed.ScaleTo(2.0f);

    MoveTo(pX, pY);
    type = GOBLIN;

    // incrementa contador
    ++Hud::goblins;
}

// ---------------------------------------------------------------------------------

Goblin::~Goblin()
{
    delete sprite;

    // decrementa contador
    --Hud::goblins;
}

// -------------------------------------------------------------------------------

void Goblin::Kill()
{
    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);
    SurviveUntilDawn::scene->Add(new XPOrb(x, y, 10), MOVING);

    // TODO: balancear
    int chance = Random{ 1, 100 }.Rand();
    if (chance <= 3)
        SurviveUntilDawn::scene->Add(new Magnet(x, y), MOVING);
    else if (chance <= 6)
        SurviveUntilDawn::scene->Add(new Bomb(x, y), MOVING);

    SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Goblin::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
    {
        SurviveUntilDawn::scene->Delete(obj, STATIC);
        SurviveUntilDawn::audio->Play(EXPLODE);
        Kill();
    }
}

// -------------------------------------------------------------------------------

void Goblin::Update()
{
    // ajusta �ngulo do vetor
    speed.RotateTo(Line::Angle(Point(x, y), Point(player->X(), player->Y())));
    Rotate(200 * gameTime);

    // movimenta objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 60.0f * gameTime, -speed.YComponent() * 60.0f * gameTime);
}

// -------------------------------------------------------------------------------
