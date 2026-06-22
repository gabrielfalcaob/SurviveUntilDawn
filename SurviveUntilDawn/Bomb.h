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
public:
    Bomb(float pX, float pY);                       // construtor
    ~Bomb();                                        // destrutor

    void OnCollect();                               // coleta da bomba
};

// ---------------------------------------------------------------------------------

#endif
