/**********************************************************************************
// Orbital (C�digo Fonte)
//
// Cria��o:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Proj�til orbital ao redor do jogador
//
**********************************************************************************/

#include "Orbital.h"
#include "Player.h"
#include "SurviveUntilDawn.h"
#include "Sprite.h"
#include "Ogre.h"
#include "TrailFire.h"
#include <cmath>

// ---------------------------------------------------------------------------------

Sprite* OrbitalSprite = nullptr;

Orbital::Orbital(Player* p, int idx) : playerRef(p), index(idx)
{
    type = ORBITAL;
    // TODO: trocar pela arte final da nave amarela
    if (!OrbitalSprite)
        OrbitalSprite = new Sprite("Resources/Missile.png");
    BBox(new Circle(8));

    // configura emissor de rastro de fuma�a
    Generator emitter;
    emitter.imgFile = "Resources/Spark.png";
    emitter.angle = 270.0f;
    emitter.spread = 60;
    emitter.lifetime = 0.3f;
    emitter.frequency = 0.02f;
    emitter.percentToDim = 0.5f;
    emitter.minSpeed = 15.0f;
    emitter.maxSpeed = 30.0f;
    emitter.color = { 0.4f, 0.4f, 0.4f, 0.6f };

    tail = new Particles(emitter);
    damageTimer.Start();
}

// ---------------------------------------------------------------------------------

Orbital::~Orbital()
{
    delete tail;
}

// ---------------------------------------------------------------------------------

void Orbital::Update()
{
    if (isGamePaused)
        return;

    playerRef->orbitalBaseAngle += 2.0f * gameTime;

    if (playerRef->orbitalCount == 0)
        return;

    float myAngle = playerRef->orbitalBaseAngle
                  + (index * (2.0f * 3.14159f) / playerRef->orbitalCount);

    float px = playerRef->X() + cos(myAngle) * playerRef->orbitalRadius;
    float py = playerRef->Y() + sin(myAngle) * playerRef->orbitalRadius;
    MoveTo(px, py);

    // rastro de fogo continuo
    fireTrailTimer -= gameTime;
    if (fireTrailTimer <= 0.0f)
    {
        SurviveUntilDawn::scene->Add(new TrailFire(x, y), MOVING);
        fireTrailTimer = 0.05f;
    }

    tail->Generate(x, y);
    tail->Update(gameTime);
}

// ---------------------------------------------------------------------------------

void Orbital::OnCollision(Object* obj)
{
    uint id = obj->Type();
    if (id == GOBLIN || id == OGRE
        || id == WIZARD || id == DRAGON)
    {
        if (damageTimer.Elapsed(playerRef->orbitalHitCooldown))
        {
            float mult = playerRef->globalDamageMultiplier;
            if (id == OGRE)
            {
                int dmg = (int)mult;
                if (dmg < 1) dmg = 1;
                for (int i = 0; i < dmg; i++)
                    ((Ogre*)obj)->Kill();
            }
            else
            {
                SurviveUntilDawn::scene->Delete(obj, MOVING);
            }
            damageTimer.Start();
        }
    }
}

// ---------------------------------------------------------------------------------

void Orbital::Draw()
{
    if (tail) tail->Draw(Layer::LOWER);

    if (OrbitalSprite)
        OrbitalSprite->Draw(x, y, Layer::FRONT, 1.0f, 0.0f);
}

// ---------------------------------------------------------------------------------
