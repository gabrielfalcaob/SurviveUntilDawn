/**********************************************************************************
// Ogre (Arquivo de Cabe�alho)
//
// Cria��o:     05 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz movimento retil�neo
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_OGRE_H_
#define _SurviveUntilDawn_OGRE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class Ogre : public Object
{
private:
    Particles * tail;                       // calda do objeto
    uint tailCount;                         // quantidade de part�culas da calda
    Vector speed;                           // velocidade e dire��o
    float speedModifier = 1.0f;             // modificador de lentidao
    float slowDuration = 0.0f;              // duracao restante da lentidao
    int hp = 5;                             // vida do ogre (tank)

    TileSet * tsRun;                        // folha de sprites corrida
    Animation * animRun;                    // anima��o corrida
    uint seqRun[6] = { 0,1,2,3,4,5 };      // sequ�ncia corrida

public:
    Ogre(float pX, float pY, float ang);    // construtor
    ~Ogre();                                // destrutor

    void Kill();                            // elimina��o do inimigo
    void OnCollision(Object* obj);          // resolu��o da colis�o
    void Update();                          // atualiza��o
    void Draw();                            // desenho
    void ApplySlow(float intensity, float duration); // aplica lentidao
};

// ---------------------------------------------------------------------------------


#endif