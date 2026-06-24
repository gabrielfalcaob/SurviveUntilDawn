/**********************************************************************************
// Bomb (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Elimina todos os inimigos na tela
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_BOMB_H_
#define _SURVIVEUNTILDAWN_BOMB_H_

// ---------------------------------------------------------------------------------

#include "Pickup.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class Bomb : public Pickup
{
private:
    bool isExploding = false;           // bomba esta explodindo
    TileSet* expTS;
    Animation* expAnim;
    uint expSeq[11] = { 0,1,2,3,4,5,6,7,8,9,10 };
    float explX = 0.0f, explY = 0.0f;

public:
    Bomb(float pX, float pY);           // construtor
    ~Bomb();                            // destrutor

    void OnCollect();                   // coleta (vazio — usa OnCollision)
    void OnCollision(Object* obj);      // colis�o com o jogador
    void Update();                      // explos�o e destrui��o
    void Draw();                        // desenho (esconde ao explodir)
};

// ---------------------------------------------------------------------------------

#endif
