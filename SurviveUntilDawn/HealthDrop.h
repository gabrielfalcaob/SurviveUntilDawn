/**********************************************************************************
// HealthDrop (Arquivo de Cabeçalho)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Item coletável que cura o jogador
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_HEALTHDROP_H_
#define _SURVIVEUNTILDAWN_HEALTHDROP_H_

// ---------------------------------------------------------------------------------

#include "Pickup.h"

// ---------------------------------------------------------------------------------

class HealthDrop : public Pickup
{
public:
    HealthDrop(float pX, float pY);             // construtor
    ~HealthDrop();                              // destrutor

    void OnCollect();                           // coleta — cura o jogador
    void Draw();                                // desenho
};

// ---------------------------------------------------------------------------------

#endif
