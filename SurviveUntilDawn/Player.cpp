// -------------------------------------------------------------------------------
/**********************************************************************************
// Player (Codigo Fonte)
//
// Criacao:     10 Out 2012
// Atualizacao: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descricao:   Define a classe jogador
//
**********************************************************************************/

#include "Player.h"
#include "Missile.h"
#include "SurviveUntilDawn.h"
#include "Hud.h"
#include "SwordSlash.h"
#include "Orbital.h"
#include "Shockwave.h"
#include "Lightning.h"
#include <cstdlib>
#include <iostream>

Image * Player::missile = nullptr;

// -------------------------------------------------------------------------------

Player::Player()
{
    // inicializa controle
    gamepad = new Controller();
    gamepadOn = gamepad->Initialize();

    // configuracao do objeto
    tsIdle = new TileSet("Resources/Warrior_Idle.png", 85, 91, 8, 8);
    tsRun = new TileSet("Resources/Warrior_Run.png", 95, 93, 6, 6);
    tsAttack = new TileSet("Resources/Warrior_Attack1.png", 122, 106, 4, 4);

    animIdle = new Animation(tsIdle, 0.100f, true);
    animRun = new Animation(tsRun, 0.080f, true);
    animAttack = new Animation(tsAttack, 0.080f, false);

    animIdle->Add(0, seqIdle, 8);
    animRun->Add(0, seqRun, 6);
    animAttack->Add(0, seqAtk, 4);

    animIdle->Select(0);
    animRun->Select(0);
    animAttack->Select(0);

    currentAnim = animIdle;

    missile = new Image("Resources/FireBall.png");
    speed.RotateTo(90.0f);
    speed.ScaleTo(0.0f);
    BBox(new Circle(18.0f));
    MoveTo(game->CenterX(), game->CenterY());
    type = PLAYER;

    // configuracao do emissor de particulas
    Generator emitter;
    emitter.imgFile = "Resources/Steps.png";    // arquivo de imagem
    emitter.angle = 270.0f;                     // angulo base do emissor
    emitter.spread = 25;                        // espalhamento em graus
    emitter.lifetime = 0.3f;                    // tempo de vida em segundos
    emitter.frequency = 0.010f;                 // tempo entre geracao de novas particulas
    emitter.percentToDim = 0.6f;                // desaparece apos 60% da vida
    emitter.minSpeed = 50.0f;                   // velocidade minima das particulas
    emitter.maxSpeed = 100.0f;                  // velocidade maxima das particulas
    emitter.color.r = 1.0f;                     // componente Red da particula
    emitter.color.g = 1.0f;                     // componente Green da particula
    emitter.color.b = 1.0f;                     // componente Blue da particula
    emitter.color.a = 1.0f;                     // transparencia da particula

    // cria sistema de particulas
    tail = new Particles(emitter);
    tailCount = 0;

    // diparo habilitado
    firingAngle = 0.0f;
    keysPressed = false;
    axisCtrl = true;
    keysCtrl = true;
    start = 0;
    timer.Start();
    attackTimer.Start();
    invulnTimer.Start();
    shockwaveTimer.Start();
    lightningTimer.Start();
}

// -------------------------------------------------------------------------------

Player::~Player()
{
    delete animIdle;
    delete animRun;
    delete animAttack;
    delete tsIdle;
    delete tsRun;
    delete tsAttack;
    delete missile;
    delete tail;
    delete gamepad;
}

// -------------------------------------------------------------------------------

void Player::OnCollision(Object * obj)
{
    // o dano e aplicado diretamente pelo inimigo ao colidir com o jogador
}

// -------------------------------------------------------------------------------

bool Player::KeysTimed(bool pressed, float time)
{
    if (keysCtrl)
    {
        if (pressed)
        {
            keysCtrl = false;
            start = timer.Stamp();
            return true;
        }
    }
    else if (timer.Elapsed(start, time))
    {
        keysCtrl = true;
    }

    return false;
}

// -------------------------------------------------------------------------------

bool Player::AxisTimed(int axisX, int axisY, float time)
{
    if (axisCtrl)
    {
        float magnitude = Point::Distance(Point(0, 0), Point(float(gamepad->Axis(axisX)), float(gamepad->Axis(axisY))));

        if (magnitude > 0)
        {
            axisCtrl = false;
            start = timer.Stamp();
            return true;
        }
    }
    else if (timer.Elapsed(start, time))
    {
        axisCtrl = true;
    }

    return false;
}

// -------------------------------------------------------------------------------

void Player::Move(Vector && v)
{
    speed.Add(v);

    if (speed.Magnitude() > 10.0f)
        speed.ScaleTo(10.0f);
}

// -------------------------------------------------------------------------------

void Player::TakeDamage(int amount)
{
    if (isImmortal)
        return;

    if (!invulnTimer.Elapsed(invulnTime))
        return;

    int danoReduzido = (int)(amount * damageTakenMultiplier);
    if (danoReduzido < 1) danoReduzido = 1;
    hp -= danoReduzido;
    invulnTimer.Start();

    if (hp <= 0)
    {
        // TODO: Game Over trigger
        hp = 0;
    }
}

// -------------------------------------------------------------------------------

void Player::Heal(int amount)
{
    hp += amount;
    if (hp > maxHp)
        hp = maxHp;
}

// -------------------------------------------------------------------------------

void Player::ActivateMagnet()
{
    magnetActive = true;
    magnetTimer.Start();
}

// -------------------------------------------------------------------------------

bool Player::IsMagnetActive()
{
    return magnetActive;
}

// -------------------------------------------------------------------------------

void Player::Update()
{
    float accel = 40.0f * gameTime;

    // -----------------
    // Controle
    // -----------------

    if (gamepadOn)
    {
        gamepad->UpdateState();

        float ang = Line::Angle(Point(0, 0), Point(gamepad->Axis(AxisX) / 25.0f, gamepad->Axis(AxisY) / 25.0f));
        float mag = Point::Distance(Point(0, 0), Point(gamepad->Axis(AxisX) / 25.0f, gamepad->Axis(AxisY) / 25.0f));

        if (mag == 0)
        {
            if (speed.Magnitude() < 0.1)
            {
                speed.ScaleTo(0.0f);
            }
            else
            {
                Move(Vector(speed.Angle() + 180.0f, 5.0f * gameTime));
            }
        }
        else
        {
            Move(Vector(ang, mag * gameTime));
        }

        if (AxisTimed(AxisRX, AxisRY, 0.150f))
        {
            float ang = Line::Angle(Point(0,0), Point(float(gamepad->Axis(AxisRX)), float(gamepad->Axis(AxisRY))));
            SurviveUntilDawn::audio->Play(FIRE);
            SurviveUntilDawn::scene->Add(new Missile(ang), STATIC);
        }
    }

    // -----------------
    // Teclado
    // -----------------

    else
    {
        if (window->KeyDown('D') && window->KeyDown('W'))
            Move(Vector(45.0f, accel));
        else if (window->KeyDown('A') && window->KeyDown('W'))
            Move(Vector(135.0f, accel));
        else if (window->KeyDown('A') && window->KeyDown('S'))
            Move(Vector(225.0f, accel));
        else if (window->KeyDown('D') && window->KeyDown('S'))
            Move(Vector(315.0f, accel));
        else if (window->KeyDown('D'))
            Move(Vector(0.0f, accel));
        else if (window->KeyDown('A'))
            Move(Vector(180.0f, accel));
        else if (window->KeyDown('W'))
            Move(Vector(90.0f, accel));
        else if (window->KeyDown('S'))
            Move(Vector(270.0f, accel));
        else
            if (speed.Magnitude() > 0.1f)
                Move(Vector(speed.Angle() + 180.0f, 5.0f * gameTime));
            else
                speed.ScaleTo(0.0f);

        if (window->KeyDown(VK_RIGHT) && window->KeyDown(VK_UP)) {
            keysPressed = true;
            firingAngle = 45.0f;
        }
        else if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_UP)) {
            keysPressed = true;
            firingAngle = 135.0f;
        }
        else if (window->KeyDown(VK_LEFT) && window->KeyDown(VK_DOWN)) {
            keysPressed = true;
            firingAngle = 225.0f;
        }
        else if (window->KeyDown(VK_RIGHT) && window->KeyDown(VK_DOWN)) {
            keysPressed = true;
            firingAngle = 315.0f;
        }
        else if (window->KeyDown(VK_RIGHT)) {
            keysPressed = true;
            firingAngle = 0.0f;
        }
        else if (window->KeyDown(VK_LEFT)) {
            keysPressed = true;
            firingAngle = 180.0f;
        }
        else if (window->KeyDown(VK_UP)) {
            keysPressed = true;
            firingAngle = 90.0f;
        }
        else if (window->KeyDown(VK_DOWN)) {
            keysPressed = true;
            firingAngle = 270.0f;
        }
        else
        {
            keysPressed = false;
        }

        if (KeysTimed(keysPressed, 0.150f))
        {
            SurviveUntilDawn::audio->Play(FIRE);
            SurviveUntilDawn::scene->Add(new Missile(firingAngle), STATIC);
        }
    }

    // -----------------
    // God Mode
    // -----------------

    if (window->KeyPress('I'))
        isImmortal = !isImmortal;

    // -----------------
    // Magnet
    // -----------------

    if (magnetActive && magnetTimer.Elapsed(magnetDuration))
        magnetActive = false;

    // -----------------
    // Shockwave (Tecla R)
    // -----------------

    if (shockwaveLevel > 0 && window->KeyDown('R'))
    {
        if (shockwaveTimer.Elapsed(3.0f))
        {
            SurviveUntilDawn::scene->Add(new Shockwave(x, y, shockwaveLevel), MOVING);
            shockwaveTimer.Start();
        }
    }

    // -----------------
    // Lightning (Tecla E)
    // -----------------

    if (lightningLevel > 0 && window->KeyDown('E'))
    {
        if (lightningTimer.Elapsed(4.0f))
        {
            int maxStrikes = 1 + lightningLevel;

            // coleta inimigos visiveis da cena
            SurviveUntilDawn::scene->Begin();
            Object* targets[64];
            int targetCount = 0;
            Object* obj = nullptr;
            while ((obj = SurviveUntilDawn::scene->Next()) != nullptr)
            {
                uint id = obj->Type();
                if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
                {
                    if (targetCount < 64)
                        targets[targetCount++] = obj;
                }
            }

            // dispara raios em inimigos aleatorios
            for (int i = 0; i < maxStrikes && targetCount > 0; i++)
            {
                int idx = rand() % targetCount;
                SurviveUntilDawn::scene->Add(
                    new Lightning(targets[idx]->X(), targets[idx]->Y()), MOVING);
            }

            lightningTimer.Start();
        }
    }

    // -----------------
    // Animacao
    // -----------------

    if (speed.XComponent() > 0.1f)
        facingRight = true;
    else if (speed.XComponent() < -0.1f)
        facingRight = false;

    if (state == ATTACK)
    {
        if (animLockTimer.Elapsed(attackDuration))
            state = IDLE;
    }
    else
    {
        if (attackTimer.Elapsed(attackCooldown))
        {
            attackTimer.Start();
            animLockTimer.Start();
            state = ATTACK;
            currentAnim = animAttack;
            animAttack->Restart();

            float slashAngle = facingRight ? 0.0f : 180.0f;
            SurviveUntilDawn::scene->Add(new SwordSlash(x, y, slashAngle, cleaveLevel), MOVING);
        }
        else if (speed.Magnitude() > 0.1f)
        {
            state = RUN;
            currentAnim = animRun;
        }
        else
        {
            state = IDLE;
            currentAnim = animIdle;
        }
    }

    currentAnim->NextFrame();

    Translate(speed.XComponent() * 50.0f * (1.0f + speedBonus) * gameTime, -speed.YComponent() * 50.0f * (1.0f + speedBonus) * gameTime);

    tail->Config().angle = speed.Angle() + 180;
    tail->Generate(x - 10 * cos(speed.Radians()), y + 10 * sin(speed.Radians()));
    tail->Update(gameTime);

    Hud::particles -= tailCount;
    tailCount = tail->Size();
    Hud::particles += tailCount;

    if (x < 50)
        MoveTo(50, y);
    if (y < 50)
        MoveTo(x, 50);
    if (x > game->Width() - 50)
        MoveTo(game->Width() - 50, y);
    if (y > game->Height() - 50)
        MoveTo(x, game->Height() - 50);
}

// ---------------------------------------------------------------------------------

void Player::Draw()
{
    currentAnim->Draw(x, y, Layer::MIDDLE, 1.0f, 0.0f);
    if (!isGamePaused)
        tail->Draw(Layer::LOWER, 1.0f);
}

// -------------------------------------------------------------------------------

void Player::LevelUp()
{
    SurviveUntilDawn::audio->Play(LEVEL_UP);
    TriggerLevelUpScreen();
}

// -------------------------------------------------------------------------------

void Player::TriggerLevelUpScreen()
{
    isGamePaused = true;
    std::cout << "\n===============================\n";
    std::cout << "          LEVEL UP!            \n";
    std::cout << " Escolha seu poder no teclado: \n";
    std::cout << " [1] Orbital (Bola de Fogo)    \n";
    std::cout << " [2] Velocidade de Movimento   \n";
    std::cout << " [3] Aumentar Raio do Ima      \n";
    std::cout << " [4] Lance do Corte            \n";
    std::cout << " [5] Onda de Choque (Tecla R)   \n";
    std::cout << " [6] Bencao da Sorte (Tecla E)   \n";
    std::cout << " [7] Aumento de Dano (Passiva)   \n";
    std::cout << " [8] Resistencia (Armadura)      \n";
    std::cout << " [9] Bonus de XP (+30%)          \n";
    std::cout << "===============================\n";
    choice1 = (rand() % 9) + 1;
    choice2 = (rand() % 9) + 1;
    choice3 = (rand() % 9) + 1;
}

// -------------------------------------------------------------------------------

void Player::ApplyPowerUp(int powerId)
{
    if (powerId == 1)
    {
        orbitalCount++;
        orbitalRadius += 15.0f;
        orbitalHitCooldown -= 0.05f;
        if (orbitalHitCooldown < 0.1f) orbitalHitCooldown = 0.1f;

        SurviveUntilDawn::scene->Begin();
        Object* obj = nullptr;
        while ((obj = SurviveUntilDawn::scene->Next()) != nullptr)
        {
            if (obj->Type() == ORBITAL)
                SurviveUntilDawn::scene->Delete();
        }

        for (int i = 0; i < orbitalCount; i++)
            SurviveUntilDawn::scene->Add(new Orbital(this, i), MOVING);
    }
    else if (powerId == 2)
    {
        speedBonus += 0.05f;
    }
    else if (powerId == 3)
    {
        pickupRadius += 80.0f;
    }
    else if (powerId == 4)
    {
        cleaveLevel++;
    }
    else if (powerId == 5)
    {
        shockwaveLevel++;
    }
    else if (powerId == 6)
    {
        lightningLevel++;
    }
    else if (powerId == 7)
    {
        globalDamageMultiplier += 0.20f;
    }
    else if (powerId == 8)
    {
        damageTakenMultiplier -= 0.15f;
        if (damageTakenMultiplier < 0.1f) damageTakenMultiplier = 0.1f;
    }
    else if (powerId == 9)
    {
        xpMultiplier += 0.3f;
    }
}

// -------------------------------------------------------------------------------
