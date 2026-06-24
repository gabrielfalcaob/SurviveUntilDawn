/**********************************************************************************
// Shockwave (Arquivo de Cabecalho)
//
// Criacao:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descricao:   Onda de choque expansiva do jogador
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_SHOCKWAVE_H_
#define _SURVIVEUNTILDAWN_SHOCKWAVE_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Animation.h"
#include "TileSet.h"

// ---------------------------------------------------------------------------------

class Shockwave : public Object
{
public:
    Shockwave(float startX, float startY, int level);
    ~Shockwave();

    void OnCollision(Object* obj);
    void Update();
    void Draw();

private:
    TileSet* ts;
    Animation* anim;
    uint seq[9] = { 0,1,2,3,4,5,6,7,8 };
    float currentRadius;
    float maxRadius;
    float expansionSpeed;
};

// ---------------------------------------------------------------------------------

#endif
