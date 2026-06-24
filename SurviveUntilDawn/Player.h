/**********************************************************************************
// Player (Arquivo de Cabe�alho)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Define a classe jogador
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_PLAYER_H_
#define _SurviveUntilDawn_PLAYER_H_

// ---------------------------------------------------------------------------------

#include "Object.h"                     // objetos do jogo
#include "Sprite.h"                     // desenho de sprites
#include "Vector.h"                     // representacao de vetores
#include "Particles.h"                  // sistema de particulas
#include "Controller.h"                 // entrada pelo controle
#include "Timer.h"                      // controle do tempo
#include "Animation.h"                  // sistema de animacao
#include "TileSet.h"                    // folha de sprites

// ---------------------------------------------------------------------------------

extern bool isGamePaused;

enum PlayerState { IDLE, RUN, ATTACK };

class Player : public Object
{
private:
    Particles * tail;                   // calda do jogador
    uint tailCount;                     // quantidade de partculas da calda

    TileSet * tsIdle;                   // folha de sprites idle
    TileSet * tsRun;                    // folha de sprites corrida
    TileSet * tsAttack;                 // folha de sprites ataque

    Animation * animIdle;               // animacao idle
    Animation * animRun;                // animacao corrida
    Animation * animAttack;             // animacao ataque
    Animation * currentAnim;            // animacao atual

    // variantes para direcao esquerda
    TileSet * tsIdleLeft;
    TileSet * tsRunLeft;
    TileSet * tsAttackLeft;
    Animation * animIdleLeft;
    Animation * animRunLeft;
    Animation * animAttackLeft;

    uint seqIdle[8] = { 0,1,2,3,4,5,6,7 };     // sequencia idle
    uint seqRun[6] = { 0,1,2,3,4,5 };          // sequencia corrida
    uint seqAtk[4] = { 0,1,2,3 };              // sequencia ataque

    PlayerState state = IDLE;               // estado atual do jogador
    bool isFacingLeft = false;              // jogador virado esquerda

    Timer attackTimer;                  // timer do ataque automatico
    float attackCooldown = 1.5f;        // cooldown entre ataques
    Timer animLockTimer;                // timer de travamento da animacao
    float attackDuration = 0.4f;        // duracao da animacao de ataque

    int hp = 10;                         // vida atual
    int maxHp = 30;                     // vida maxima
    bool isImmortal = false;            // modo Deus ativo
    Timer invulnTimer;                  // timer de invulnerabilidade
    float invulnTime = 1.0f;            // duracao da invulnerabilidade

    bool magnetActive = false;          // ima ativo
    Timer magnetTimer;                  // timer do ima
    float magnetDuration = 3.0f;        // duracao do ima (segundos)

    Controller * gamepad;               // leitura do controle
    bool gamepadOn;                     // controle esta ligado

    Timer timer;                        // controla tempo dos disparos
    llong start;                        // marcacao de incio do disparo
    bool axisCtrl;                      // habilita leitura de disparos
    bool keysCtrl;                      // habilita disparos pelas setas
    bool keysPressed;                   // qualquer seta pressionada
    float firingAngle;                  // direcao dos disparos

    int shockwaveLevel = 0;             // nivel da Onda de Choque
    Timer shockwaveTimer;               // timer de cooldown da onda

    int lightningLevel = 0;             // nivel da Bencao da Sorte
    Timer lightningTimer;               // timer de cooldown dos raios

    float throwCooldown = 3.5f;         // cooldown base do arremesso
    float throwTimer = 1.0f;            // timer atual do arremesso

public:
    static Image * missile;             // imagem do missil
    Vector speed;                       // velocidade e direcao de movimento
    float speedBonus = 0.0f;            // bonus de velocidade do level up
    float pickupRadius = 80.0f;         // raio de atracao dos orbs (public)
    int choice1 = 0, choice2 = 0, choice3 = 0;  // escolhas do level up (public)
    int orbitalCount = 0;               // quantidade de orbes ativos
    float orbitalBaseAngle = 0.0f;      // angulo base dos orbitais
    float orbitalRadius = 70.0f;        // raio da orbita dos orbitais
    float orbitalHitCooldown = 0.5f;    // cooldown entre acertos dos orbitais
    int cleaveLevel = 0;                // nivel do Lance do Corte
    float globalDamageMultiplier = 1.0f; // multiplicador global de dano
    float damageTakenMultiplier = 1.0f;  // multiplicador de dano recebido (armadura)
    float xpMultiplier = 1.0f;           // multiplicador de XP
    float attackSpeedMultiplier = 1.0f;  // multiplicador de velocidade de ataque (menor = mais rapido)

    // estatisticas da partida
    int totalXpCollected = 0;           // orbes de XP coletadas
    int totalFoodCollected = 0;         // comidas coletadas
    bool isDead = false;                // jogador morreu

    Player();                           // construtor
    ~Player();                          // destrutor

    bool AxisTimed(int axisX, int axisY, float time);
    bool KeysTimed(bool pressed, float time);

    void Move(Vector && v);             // movimenta jogador
    void TakeDamage(int amount);        // recebe dano
    void Heal(int amount);              // cura o jogador
    void ActivateMagnet();              // ativa o ima
    bool IsMagnetActive();              // retorna se o ima esta ativo
    void OnCollision(Object * obj);     // resolucao de colisao
    void Update();                      // atualizacao
    void Draw();                        // desenho

    void LevelUp();                     // sobe de nivel
    void TriggerLevelUpScreen();        // tela de selecao de poder
    void ApplyPowerUp(int powerId);     // aplica o poder escolhido

    // getters para o Hud
    int GetHp() { return hp; }
    int GetMaxHp() { return maxHp; }
    int GetLightningLevel() { return lightningLevel; }
    int GetShockwaveLevel() { return shockwaveLevel; }
    float GetLightningTime() { return lightningTimer.Elapsed(); }
    float GetShockwaveTime() { return shockwaveTimer.Elapsed(); }
    bool IsFacingLeft() const { return isFacingLeft; }
    Controller* GetGamepad() { return gamepad; }
};
// ---------------------------------------------------------------------------------

#endif
