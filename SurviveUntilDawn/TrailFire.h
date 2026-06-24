/**********************************************************************************
// TrailFire (Arquivo de Cabecalho)
//
// Criacao:     24 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Rastro de fogo do poder orbital
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_TRAILFIRE_H_
#define _SURVIVEUNTILDAWN_TRAILFIRE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class TrailFire : public Object
{
public:
    TrailFire(float startX, float startY);
    ~TrailFire();

    void Update();
    void Draw();

private:
    TileSet* ts;
    Animation* anim;
    uint seq[9] = { 0,1,2,3,4,5,6,7,8 };
};

// ---------------------------------------------------------------------------------

#endif
