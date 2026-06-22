/**********************************************************************************
// XPOrb (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Orb de experi�ncia colet�vel pelo jogador
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_XPORB_H_
#define _SURVIVEUNTILDAWN_XPORB_H_

// ---------------------------------------------------------------------------------

#include "Pickup.h"

// ---------------------------------------------------------------------------------

class XPOrb : public Pickup
{
private:
    uint amount;                                // quantidade de XP

public:
    XPOrb(float pX, float pY, uint val);        // construtor
    ~XPOrb();                                   // destrutor

    void OnCollect();                           // coleta do orb
    void Update();                              // atualiza��o
};

// ---------------------------------------------------------------------------------

#endif