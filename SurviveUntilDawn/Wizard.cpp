/**********************************************************************************
// Wizard (C�digo Fonte)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz uma persegui��o suavizada
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

// ---------------------------------------------------------------------------------

Wizard::Wizard(float pX, float pY, Player* p)
{
    player = p;

    tsRun = new TileSet("Resources/Pawn_Run Axe.png", 79, 79, 6, 6);
    animRun = new Animation(tsRun, 0.100f, true);
    animRun->Add(0, seqRun, 6);
    animRun->Select(0);

    BBox(new Circle(20.0f));
    speed.RotateTo(0.0f);
    speed.ScaleTo(0.0f);
    MoveTo(pX, pY);
    factor = -0.25f;
    type = WIZARD;

    // incrementa contador
    ++Hud::wizards;
}

// ---------------------------------------------------------------------------------

Wizard::~Wizard()
{
    delete animRun;
    delete tsRun;

    // decrementa contador
    --Hud::wizards;
}

// -------------------------------------------------------------------------------

void Wizard::Kill()
{
    SurviveUntilDawn::scene->Add(new Explosion(x, y), STATIC);
    SurviveUntilDawn::scene->Add(new XPOrb(x, y, 25), MOVING);

    // TODO: balancear
    int chance = Random{ 1, 100 }.Rand();
    if (chance <= 3)
        SurviveUntilDawn::scene->Add(new Magnet(x, y), MOVING);
    else if (chance <= 6)
        SurviveUntilDawn::scene->Add(new Bomb(x, y), MOVING);

    SurviveUntilDawn::scene->Delete(this, MOVING);
}

// -------------------------------------------------------------------------------

void Wizard::OnCollision(Object * obj)
{
    if (obj->Type() == MISSILE)
    {
        SurviveUntilDawn::scene->Delete(obj, STATIC);
        SurviveUntilDawn::audio->Play(EXPLODE);
        Kill();
    }
}

// -------------------------------------------------------------------------------

void Wizard::Update()
{
    // a magnitude do vetor 'target' controla qu�o
    // r�pido o objeto converge para a dire��o do alvo
    Vector target { Line::Angle(Point(x, y), Point(player->X(), player->Y())), 2.0f * gameTime };
    speed.Add(target);

    // limita a magnitude da velocidade para impedir
    // que ela cres�a indefinidamente pelo soma vetorial
    if (speed.Magnitude() > 4.5)
        speed.ScaleTo(4.5f);

    // move o objeto pelo seu vetor velocidade
    Translate(speed.XComponent() * 50.0f * gameTime, -speed.YComponent() * 50.0f * gameTime);

    // aplica fator de escala
    Scale(1.0f + factor * gameTime);

    // amplia e reduz objeto
    if (scale < 0.85f)
        factor = 0.25f;
    if (scale > 1.00f)
        factor = -0.25f;

    // mant�m o objeto dentro do mundo do jogo
    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);

    // atualiza anima��o
    animRun->NextFrame();
}

// -------------------------------------------------------------------------------

void Wizard::Draw()
{
    animRun->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
}

// -------------------------------------------------------------------------------
