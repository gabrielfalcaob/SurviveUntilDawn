/**********************************************************************************
// Dragon (C�digo Fonte)
//
// Cria��o:     15 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz uma fuga suavizada
//
**********************************************************************************/

#include "Dragon.h"
#include "SurviveUntilDawn.h"
#include "Random.h"
#include "Explosion.h"
#include "XPOrb.h"
#include "Magnet.h"
#include "Bomb.h"

// ---------------------------------------------------------------------------------

Dragon::Dragon(float pX, float pY, Player * p)
{
    player = p;
    sprite = new Sprite(SurviveUntilDawn::dragon);
    BBox(new Circle(20.0f));
    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);
    MoveTo(pX, pY);
    type = DRAGON;

    // mant�m certa dist�ncia do jogador
    distance = Random{100,400}.Rand();

    // incrementa contador
    ++Hud::dragons;
}

// ---------------------------------------------------------------------------------

Dragon::~Dragon()
{
    delete sprite;

    // decrementa contador
    --Hud::dragons;
}

// -------------------------------------------------------------------------------

void Dragon::Kill()
{
    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);
    SurviveUntilDawn::scene->Add(new XPOrb(x, y, 100), MOVING);

    // TODO: balancear
    int chance = Random{ 1, 100 }.Rand();
    if (chance <= 3)
        SurviveUntilDawn::scene->Add(new Magnet(x, y), MOVING);
    else if (chance <= 6)
        SurviveUntilDawn::scene->Add(new Bomb(x, y), MOVING);

    SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Dragon::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
    {
        SurviveUntilDawn::scene->Delete(obj, STATIC);
        SurviveUntilDawn::audio->Play(EXPLODE);
        Kill();
    }
}

// -------------------------------------------------------------------------------

void Dragon::Update()
{
    // a magnitude do vetor target controla qu�o r�pido o objeto converge para a dire��o do alvo
    Vector target = Vector(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 20.0f * gameTime);

    // fugir se o player chegar muito perto
    if (Point::Distance(Point(x, y), Point(player->X(), player->Y())) < distance)
    {
        target.Rotate(180.0f);
        target.ScaleTo(100.0f * gameTime);
    }

    speed.Add(target);

    // limita a magnitude da velocidade para impedir
    // seu crescimento indefinido na soma vetorial
    if (speed.Magnitude() > 8)
        speed.ScaleTo(8.0f);

    // move o objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 50.0f * gameTime, -speed.YComponent() * 50.0f * gameTime);
    Rotate(50 * gameTime);

    // mant�m o objeto dentro do mundo do jogo
    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);
}

// -------------------------------------------------------------------------------
