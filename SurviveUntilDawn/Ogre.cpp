/**********************************************************************************
// Ogre (C�digo Fonte)
//
// Cria��o:     05 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz movimento retil�neo
//
**********************************************************************************/

#include "SurviveUntilDawn.h"
#include "Ogre.h"
#include "Random.h"
#include "Explosion.h"
#include "XPOrb.h"
#include "Magnet.h"
#include "Bomb.h"

// ---------------------------------------------------------------------------------

Ogre::Ogre(float pX, float pY, float ang)
{
    tsRun = new TileSet("Resources/Pawn_Run Axe.png", 79, 79, 6, 6);
    animRun = new Animation(tsRun, 0.100f, true);
    animRun->Add(0, seqRun, 6);
    animRun->Select(0);

    // ajusta o vetor velocidade
    speed.RotateTo(ang);
    speed.ScaleTo(400);
    RotateTo(-speed.Angle());
    BBox(new Circle(20.0f));
    MoveTo(pX, pY);
    type = OGRE;

    // configura��o do emissor de part�culas
    Generator emitter;
    emitter.imgFile = "Resources/Spark.png";    // arquivo de imagem
    emitter.angle = speed.Angle() + 180;        // �ngulo base do emissor
    emitter.spread = 5;                         // espalhamento em graus
    emitter.lifetime = 0.4f;                    // tempo de vida em segundos
    emitter.frequency = 0.010f;                 // tempo entre gera��o de novas part�culas
    emitter.percentToDim = 0.8f;                // desaparece ap�s 60% da vida
    emitter.minSpeed = 100.0f;                  // velocidade m�nima das part�culas
    emitter.maxSpeed = 200.0f;                  // velocidade m�xima das part�culas
    emitter.color.r = 1.0f;                     // componente Red da part�cula
    emitter.color.g = 0.5;                      // componente Green da part�cula
    emitter.color.b = 0.0f;                     // componente Blue da part�cula
    emitter.color.a = 1.0f;                     // transpar�ncia da part�cula

    // cria sistema de part�culas
    tail = new Particles(emitter);
    tailCount = 0;

    // incrementa contagem
    ++Hud::ogres;
}

// ---------------------------------------------------------------------------------

Ogre::~Ogre()
{
    delete animRun;
    delete tsRun;
    delete tail;

    // decrementa contagem
    Hud::particles -= tailCount;
    --Hud::ogres;
}

// -------------------------------------------------------------------------------

void Ogre::Kill()
{
    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);
    SurviveUntilDawn::scene->Add(new XPOrb(x, y, 20), MOVING);

    // TODO: balancear
    int chance = Random{ 1, 100 }.Rand();
    if (chance <= 3)
        SurviveUntilDawn::scene->Add(new Magnet(x, y), MOVING);
    else if (chance <= 6)
        SurviveUntilDawn::scene->Add(new Bomb(x, y), MOVING);

    SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Ogre::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
    {
        SurviveUntilDawn::scene->Delete(obj, STATIC);
        SurviveUntilDawn::audio->Play(EXPLODE);
        Kill();
    }
}

// -------------------------------------------------------------------------------

void Ogre::Update()
{
    // movimenta objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * gameTime, -speed.YComponent() * gameTime);

    // ajusta �ngulo do vetor na dire��o oposta
    if (x < 50 || y < 50 || x > game->Width() - 50 || y > game->Height() - 50)
    {
        Rotate(180);
        speed.Rotate(180);
        Translate(speed.XComponent() * gameTime, -speed.YComponent() * gameTime);
    }

    // atualiza calda do objeto
    tail->Config().angle = speed.Angle();
    tail->Generate(x - 10 * cos(speed.Radians()), y + 10 * sin(speed.Radians()));
    tail->Update(gameTime);

    Hud::particles -= tailCount;
    tailCount = tail->Size();
    Hud::particles += tailCount;

    // atualiza anima��o
    animRun->NextFrame();
}

// ---------------------------------------------------------------------------------

void Ogre::Draw()
{
    animRun->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
    tail->Draw(Layer::LOWER, 1.0f);
}

// -------------------------------------------------------------------------------
