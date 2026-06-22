/**********************************************************************************
// Goblin (Arquivo de Cabe�alho)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz um persegui��o direta
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_GOBLIN_H_
#define _SurviveUntilDawn_GOBLIN_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"

// ---------------------------------------------------------------------------------

class Goblin : public Object
{
private:
    Sprite * sprite;                            // sprite do objeto
    Player * player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e dire��o

public:
    Goblin(float pX, float pY, Player * p);     // construtor
    ~Goblin();                                  // destrutor

    void Kill();                                // elimina��o do inimigo
    void OnCollision(Object * obj);             // resolu��o da colis�o
    void Update();                              // atualiza��o
    void Draw();                                // desenho
};

// ---------------------------------------------------------------------------------

inline void Goblin::Draw()
{ sprite->Draw(x, y, Layer::LOWER, scale, rotation); }

// ---------------------------------------------------------------------------------


#endif