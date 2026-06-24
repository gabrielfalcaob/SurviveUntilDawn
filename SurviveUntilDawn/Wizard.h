/**********************************************************************************
// Wizard (Arquivo de Cabe�alho)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz uma persegui��o suavizada
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_WIZARD_H_
#define _SurviveUntilDawn_WIZARD_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class Wizard : public Object
{
private:
    Player * player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e dire��o
    float speedModifier = 1.0f;                 // modificador de lentidao
    float slowDuration = 0.0f;                  // duracao restante da lentidao
    float factor;                               // fator de escala

    TileSet * tsRun;                            // folha de sprites corrida
    Animation * animRun;                        // anima��o corrida
    uint seqRun[6] = { 0,1,2,3,4,5 };          // sequ�ncia corrida

    Timer attackTimer;                          // timer do ataque magico
    float attackCooldown = 2.5f;                // cooldown entre disparos

public:
    Wizard(float pX, float pY, Player * p);     // construtor
    ~Wizard();                                  // destrutor

    void Kill();                                // elimina��o do inimigo
    void OnCollision(Object* obj);              // resolu��o da colis�o
    void Update();                              // atualiza��o
    void Draw();                                // desenho
    void ApplySlow(float intensity, float duration); // aplica lentidao
};

// ---------------------------------------------------------------------------------


#endif