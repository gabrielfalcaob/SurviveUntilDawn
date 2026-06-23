/**********************************************************************************
// SwordSlash (Arquivo de Cabe�alho)
//
// Cria��o:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Hitbox f�sica do ataque do jogador
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_SWORDSLASH_H_
#define _SURVIVEUNTILDAWN_SWORDSLASH_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Timer.h"

// ---------------------------------------------------------------------------------

class SwordSlash : public Object
{
private:
    Timer lifeTimer;                            // tempo de vida do hitbox
    bool facingRight;                           // dire��o do ataque

public:
    SwordSlash(float startX, float startY, bool dir);   // construtor
    ~SwordSlash();                                      // destrutor

    void OnCollision(Object* obj);                      // resolu��o da colis�o
    void Update();                                      // atualiza��o
    void Draw();                                        // desenho
};

// ---------------------------------------------------------------------------------

#endif
