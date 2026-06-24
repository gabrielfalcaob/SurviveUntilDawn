/**********************************************************************************
// Lightning (Arquivo de Cabecalho)
//
// Criacao:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Raio da Bencao da Sorte - dano + lentidao nos inimigos
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_LIGHTNING_H_
#define _SURVIVEUNTILDAWN_LIGHTNING_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Animation.h"
#include "TileSet.h"
#include "Timer.h"

// ---------------------------------------------------------------------------------

class Lightning : public Object
{
public:
    Lightning(float startX, float startY);
    ~Lightning();

    void OnCollision(Object* obj);
    void Update();
    void Draw();

private:
    TileSet* ts;
    Animation* anim;
    uint seq[4] = { 0,1,2,3 };
    Timer lifeTimer;
    int damage = 2;
};

// ---------------------------------------------------------------------------------

#endif
