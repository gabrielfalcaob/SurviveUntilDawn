/**********************************************************************************
// Goblin (Código Fonte)
//
// Criação:     10 Out 2012
// Atualização: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descrição:   Objeto faz um perseguição direta
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
#include "HealthDrop.h"

#include <cstdlib>

// ---------------------------------------------------------------------------------

Goblin::Goblin(float pX, float pY, Player * p)
{
    player = p;

    tsRun = new TileSet("Resources/Pawn_Run Axe.png", 79, 79, 6, 6);
    animRun = new Animation(tsRun, 0.100f, true);
    animRun->Add(0, seqRun, 6);
    animRun->Select(0);

    BBox(new Circle(20.0f));

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
    delete animRun;
    delete tsRun;

    // decrementa contador
    --Hud::goblins;
}

// -------------------------------------------------------------------------------

void Goblin::Kill()
{
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

void Goblin::OnCollision(Object * obj)
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
            dx = 1.0f; // evita divisão por zero se perfeitamente sobreposto
        float length = sqrt(dx * dx + dy * dy);
        if (length > 0.0f)
            Translate((dx / length) * 1.5f, (dy / length) * 1.5f);
    }
}

// -------------------------------------------------------------------------------

void Goblin::ApplySlow(float intensity, float duration)
{
    speedModifier = intensity;
    slowDuration = duration;
}

// -------------------------------------------------------------------------------

void Goblin::Update()
{
    // decai a lentidao
    if (slowDuration > 0.0f)
    {
        slowDuration -= gameTime;
        if (slowDuration <= 0.0f)
            speedModifier = 1.0f;
    }

    // ajusta ângulo do vetor
    speed.RotateTo(Line::Angle(Point(x, y), Point(player->X(), player->Y())));
    Rotate(200 * gameTime);

    // movimenta objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 60.0f * gameTime * speedModifier, -speed.YComponent() * 60.0f * gameTime * speedModifier);

    // atualiza animação
    animRun->NextFrame();
}

// -------------------------------------------------------------------------------

void Goblin::Draw()
{
    animRun->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
}

// -------------------------------------------------------------------------------
