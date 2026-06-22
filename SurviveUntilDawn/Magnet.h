/**********************************************************************************
// Magnet (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Coleta todos os orbs de XP na tela
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

    void OnCollect();                               // coleta do �m�
};

// ---------------------------------------------------------------------------------

#endif
