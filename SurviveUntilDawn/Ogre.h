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

// ---------------------------------------------------------------------------------

class Ogre : public Object
{
private:
    Sprite * sprite;                        // sprite do objeto
    Particles * tail;                       // calda do objeto
    uint tailCount;                         // quantidade de part�culas da calda
    Vector speed;                           // velocidade e dire��o

public:
    Ogre(float pX, float pY, float ang);    // construtor
    ~Ogre();                                // destrutor

    void Kill();                            // elimina��o do inimigo
    void OnCollision(Object* obj);          // resolu��o da colis�o
    void Update();                          // atualiza��o
    void Draw();                            // desenho
};

// ---------------------------------------------------------------------------------


#endif