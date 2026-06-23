/**********************************************************************************
// Orbital (Arquivo de Cabe�alho)
//
// Cria��o:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Proj�til orbital ao redor do jogador (TODO)
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_ORBITAL_H_
#define _SURVIVEUNTILDAWN_ORBITAL_H_

// ---------------------------------------------------------------------------------

#include "Object.h"

// ---------------------------------------------------------------------------------

class Orbital : public Object
{
public:
    Orbital();
    void Update();
    void Draw();
};

// ---------------------------------------------------------------------------------

#endif
