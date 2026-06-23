/**********************************************************************************
// Magnet (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Atrai todos os orbs de XP na tela por 3 segundos
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_MAGNET_H_
#define _SURVIVEUNTILDAWN_MAGNET_H_

// ---------------------------------------------------------------------------------

#include "Pickup.h"

// ---------------------------------------------------------------------------------

class Magnet : public Pickup
{
public:
    Magnet(float pX, float pY);                     // construtor
    ~Magnet();                                      // destrutor

    void OnCollect();                               // coleta (vazio — usa OnCollision)
    void OnCollision(Object* obj);                  // ativa o im� no jogador
};

// ---------------------------------------------------------------------------------

#endif
