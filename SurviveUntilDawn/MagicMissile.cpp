/**********************************************************************************
// MagicMissile (Código Fonte)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Projétil mágico disparado pelo Wizard
//
**********************************************************************************/

#include "MagicMissile.h"
#include "SurviveUntilDawn.h"
#include "Player.h"
#include <cmath>

// ---------------------------------------------------------------------------------

MagicMissile::MagicMissile(float startX, float startY, float targetX, float targetY)
{
    sprite = new Sprite("Resources/Arrow.png");

    // calcula direção normalizada do ponto inicial ao alvo
    float dirX = targetX - startX;
    float dirY = targetY - startY;
    float dist = sqrt(dirX * dirX + dirY * dirY);
    if (dist > 0.0f)
    {
        dx = dirX / dist;
        dy = dirY / dist;
    }
    else
    {
        dx = 0.0f;
        dy = 1.0f;
    }

    BBox(new Circle(8.0f));
    MoveTo(startX, startY);
    type = MAGICMISSILE;
    lifeTimer.Start();
}

// ---------------------------------------------------------------------------------

MagicMissile::~MagicMissile()
{
    delete sprite;
}

// -------------------------------------------------------------------------------

void MagicMissile::OnCollision(Object* obj)
{
    if (obj->Type() == PLAYER)
    {
        ((Player*)obj)->TakeDamage(1);
        SurviveUntilDawn::scene->Delete(this, MOVING);
    }
}

// -------------------------------------------------------------------------------

void MagicMissile::Update()
{
    // move na direção armazenada a velocidade 5.0f por frame
    Translate(dx * 5.0f, dy * 5.0f);

    // auto-destrói após 4 segundos (segurança caso saia da tela)
    if (lifeTimer.Elapsed(4.0f))
        SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void MagicMissile::Draw()
{
    if (sprite)
        sprite->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
}

// -------------------------------------------------------------------------------
