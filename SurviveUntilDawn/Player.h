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

    uint seqIdle[8] = { 0,1,2,3,4,5,6,7 };     // sequencia idle
    uint seqRun[6] = { 0,1,2,3,4,5 };          // sequencia corrida
    uint seqAtk[4] = { 0,1,2,3 };              // sequencia ataque

    PlayerState state = IDLE;               // estado atual do jogador
    bool facingRight = true;                // jogador virado direita

    Timer attackTimer;                  // timer do ataque autom�tico
    float attackCooldown = 1.5f;        // cooldown entre ataques
    
    Controller * gamepad;               // leitura do controle
    bool gamepadOn;                     // controle esta ligado
    
    Timer timer;                        // controla tempo dos disparos
    llong start;                        // marcacao de incio do disparo
    bool axisCtrl;                      // habilita leitura de disparos
    bool keysCtrl;                      // habilita disparos pelas setas
    bool keysPressed;                   // qualquer seta pressionada
    float firingAngle;                  // direcao dos disparos

public:
    static Image * missile;             // imagem do missil
    Vector speed;                       // velocidade e direcao de movimento

    Player();                           // construtor
    ~Player();                          // destrutor
    
    bool AxisTimed(int axisX, int axisY, float time);
    bool KeysTimed(bool pressed, float time);

    void Move(Vector && v);             // movimenta jogador
    void OnCollision(Object * obj);     // resolucao de colisao
    void Update();                      // atualiza��o
    void Draw();                        // desenho
}; 
// ---------------------------------------------------------------------------------

#endif