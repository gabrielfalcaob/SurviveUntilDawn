/**********************************************************************************
// Ogre (Código Fonte)
//
// Criação:     05 Ago 2019
// Atualização: 23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Inimigo tanque — persegue o jogador devagar com 5 de vida
//
**********************************************************************************/

#include "SurviveUntilDawn.h"
#include "Ogre.h"
#include "Random.h"
#include "Hud.h"
#include "Explosion.h"
#include "XPOrb.h"
#include "Magnet.h"
#include "Bomb.h"
#include "HealthDrop.h"

#include <cmath>
#include <cstdlib>

// ---------------------------------------------------------------------------------

Ogre::Ogre(float pX, float pY, float ang)
{
    tsRun = new TileSet("Resources/Pawn_Run Axe.png", 79, 79, 6, 6);
    animRun = new Animation(tsRun, 0.100f, true);
    animRun->Add(0, seqRun, 6);
    animRun->Select(0);

    BBox(new Circle(20.0f));

    speed.RotateTo(ang);
    speed.ScaleTo(2.0f);
    RotateTo(-speed.Angle());
    MoveTo(pX, pY);
    type = OGRE;

    // configuração do emissor de partículas
    Generator emitter;
    emitter.imgFile = "Resources/Spark.png";
    emitter.angle = speed.Angle() + 180;
    emitter.spread = 5;
    emitter.lifetime = 0.4f;
    emitter.frequency = 0.010f;
    emitter.percentToDim = 0.8f;
    emitter.minSpeed = 100.0f;
    emitter.maxSpeed = 200.0f;
    emitter.color.r = 1.0f;
    emitter.color.g = 0.5f;
    emitter.color.b = 0.0f;
    emitter.color.a = 1.0f;

    tail = new Particles(emitter);
    tailCount = 0;

    ++Hud::ogres;
}

// ---------------------------------------------------------------------------------

Ogre::~Ogre()
{
    delete animRun;
    delete tsRun;
    delete tail;

    Hud::particles -= tailCount;
    --Hud::ogres;
}

// -------------------------------------------------------------------------------

void Ogre::Kill()
{
    hp--;
    if (hp > 0)
        return;

    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);

    // rolagem de drops
    int roll = rand() % 100;
    if (roll < 1)
        SurviveUntilDawn::scene->Add(new Bomb(x, y), MOVING);
    else if (roll < 2)
        SurviveUntilDawn::scene->Add(new Magnet(x, y), MOVING);
    else if (roll < 5)
        SurviveUntilDawn::scene->Add(new HealthDrop(x, y), MOVING);
    else
        SurviveUntilDawn::scene->Add(new XPOrb(x, y), MOVING);

    SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Ogre::OnCollision(Object * obj)
{
    if (obj->Type() == PLAYER)
    {
        ((Player*)obj)->TakeDamage(1);
    }
    else if (obj->Type() == MISSILE)
    {
        SurviveUntilDawn::scene->Delete(obj, STATIC);
        SurviveUntilDawn::audio->Play(EXPLODE);
        Kill();
    }

    // separação de inimigos para evitar sobreposição
    uint id = obj->Type();
    if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
    {
        float dx = x - obj->X();
        float dy = y - obj->Y();
        if (dx == 0.0f && dy == 0.0f)
            dx = 1.0f;
        float length = sqrt(dx * dx + dy * dy);
        if (length > 0.0f)
            Translate((dx / length) * 1.5f, (dy / length) * 1.5f);
    }
}

// -------------------------------------------------------------------------------

void Ogre::Update()
{
    // persegue o jogador (50% mais lento que o Goblin)
    speed.RotateTo(Line::Angle(Point(x, y), Point(SurviveUntilDawn::player->X(), SurviveUntilDawn::player->Y())));

    // movimenta objeto pelo seu vetor velocidade (metade da velocidade do Goblin)
    Translate(speed.XComponent() * 30.0f * gameTime, -speed.YComponent() * 30.0f * gameTime);

    // atualiza calda do objeto
    tail->Config().angle = speed.Angle();
    tail->Generate(x - 10 * cos(speed.Radians()), y + 10 * sin(speed.Radians()));
    tail->Update(gameTime);

    Hud::particles -= tailCount;
    tailCount = tail->Size();
    Hud::particles += tailCount;

    // atualiza animação
    animRun->NextFrame();
}

// ---------------------------------------------------------------------------------

void Ogre::Draw()
{
    animRun->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
    tail->Draw(Layer::LOWER, 1.0f);
}

// -------------------------------------------------------------------------------
