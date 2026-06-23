/**********************************************************************************
// Dragon (Arquivo de Cabe�alho)
//
// Cria��o:     15 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz uma fuga suavizada
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_DRAGON_H_
#define _SurviveUntilDawn_DRAGON_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class Dragon : public Object
{
private:
    Player * player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e dire��o
    int distance;                               // dist�ncia do jogador

    TileSet * tsRun;                            // folha de sprites corrida
    Animation * animRun;                        // anima��o corrida
    uint seqRun[6] = { 0,1,2,3,4,5 };          // sequ�ncia corrida

public:
    Dragon(float pX, float pY, Player * p);     // construtor
    ~Dragon();                                  // destrutor

    void Kill();                                // elimina��o do inimigo
    void OnCollision(Object* obj);              // resolu��o da colis�o
    void Update();                              // atualiza��o
    void Draw();                                // desenho
};

// ---------------------------------------------------------------------------------


#endif