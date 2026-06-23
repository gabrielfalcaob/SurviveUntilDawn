/**********************************************************************************
// MagicMissile (Arquivo de Cabeçalho)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Projétil mágico disparado pelo Wizard
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_MAGICMISSILE_H_
#define _SURVIVEUNTILDAWN_MAGICMISSILE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Timer.h"

// ---------------------------------------------------------------------------------

class MagicMissile : public Object
{
private:
    Sprite* sprite;                     // sprite do projétil
    float dx, dy;                       // direção normalizada
    Timer lifeTimer;                    // timer de vida

public:
    MagicMissile(float startX, float startY, float targetX, float targetY); // construtor
    ~MagicMissile();                                                            // destrutor

    void OnCollision(Object* obj);      // colisão com jogador
    void Update();                      // movimento e auto-destruct
    void Draw();                        // desenho
};

// ---------------------------------------------------------------------------------

#endif
