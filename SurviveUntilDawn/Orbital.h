/**********************************************************************************
// Orbital (Arquivo de Cabe�alho)
//
// Cria��o:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Proj�til orbital ao redor do jogador
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_ORBITAL_H_
#define _SURVIVEUNTILDAWN_ORBITAL_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Particles.h"
#include "Timer.h"

class Player;

// ---------------------------------------------------------------------------------

class Orbital : public Object
{
public:
    Orbital(Player* p, int idx);
    ~Orbital();
    void Update();
    void Draw();
    void OnCollision(Object* obj);

private:
    Player* playerRef;
    int index;
    Particles* tail = nullptr;
    Timer damageTimer;
    float fireTrailTimer = 0.0f;
};

// ---------------------------------------------------------------------------------

#endif
