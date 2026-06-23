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

// ---------------------------------------------------------------------------------

class Bomb : public Pickup
{
private:
    bool isExploding = false;           // bomba est� explodindo

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
