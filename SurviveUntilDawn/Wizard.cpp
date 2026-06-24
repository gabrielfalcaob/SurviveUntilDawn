/**********************************************************************************
// Wizard (Código Fonte)
//
// Criação:     10 Out 2012
// Atualização: 23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Inimigo arqueiro — mantém distância e atira MagicMissile
//
**********************************************************************************/

#include "SurviveUntilDawn.h"
#include "Wizard.h"
#include "Random.h"
#include "Hud.h"
#include "Explosion.h"
#include "XPOrb.h"
#include "Magnet.h"
#include "Bomb.h"
#include "HealthDrop.h"
#include "MagicMissile.h"

#include <cmath>
#include <cstdlib>

// ---------------------------------------------------------------------------------

Wizard::Wizard(float pX, float pY, Player* p)
{
    player = p;

    tsRun = new TileSet("Resources/Archer.png", 75, 92, 4, 4);
    animRun = new Animation(tsRun, 0.100f, true);
    animRun->Add(0, seqRun, 4);
    animRun->Select(0);

    BBox(new Circle(20.0f));
    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);
    MoveTo(pX, pY);
    factor = -0.25f;
    type = WIZARD;

    attackTimer.Start();

    ++Hud::wizards;
}

// ---------------------------------------------------------------------------------

Wizard::~Wizard()
{
    delete animRun;
    delete tsRun;

    --Hud::wizards;
}

// -------------------------------------------------------------------------------

void Wizard::Kill()
{
    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);

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

void Wizard::OnCollision(Object * obj)
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

void Wizard::ApplySlow(float intensity, float duration)
{
    speedModifier = intensity;
    slowDuration = duration;
}

// -------------------------------------------------------------------------------

void Wizard::Update()
{
    // decai a lentidao
    if (slowDuration > 0.0f)
    {
        slowDuration -= gameTime;
        if (slowDuration <= 0.0f)
            speedModifier = 1.0f;
    }

    float dist = Point::Distance(Point(x, y), Point(player->X(), player->Y()));

    // IA de distância: foge se < 250, persegue se > 350, parado no meio
    Vector target(Line::Angle(Point(x, y), Point(player->X(), player->Y())), 2.0f * gameTime);

    if (dist < 250.0f)
    {
        // foge — inverte a direção
        target.Rotate(180.0f);
        speed.Add(target);
    }
    else if (dist > 350.0f)
    {
        // persegue — direção normal
        speed.Add(target);
    }
    else
    {
        // zona segura — desacelera
        if (speed.Magnitude() > 0.1f)
            speed.ScaleTo(speed.Magnitude() * 0.9f);
        else
            speed.ScaleTo(0.0f);
    }

    // limita a magnitude máxima
    if (speed.Magnitude() > 4.5f)
        speed.ScaleTo(4.5f);

    // move o objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 50.0f * gameTime * speedModifier, -speed.YComponent() * 50.0f * gameTime * speedModifier);

    // efeito de escala (amplia e reduz)
    Scale(1.0f + factor * gameTime);
    if (scale < 0.85f)
        factor = 0.25f;
    if (scale > 1.00f)
        factor = -0.25f;

    // mantém o objeto dentro do mundo do jogo
    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);

    // ataque à distância
    if (attackTimer.Elapsed(attackCooldown) && dist < 400.0f)
    {
        attackTimer.Start();
        SurviveUntilDawn::scene->Add(
            new MagicMissile(x, y, player->X(), player->Y()), MOVING);
    }

    // atualiza animação
    animRun->NextFrame();
}

// -------------------------------------------------------------------------------

void Wizard::Draw()
{
    animRun->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
}

// -------------------------------------------------------------------------------
