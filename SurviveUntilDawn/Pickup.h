/**********************************************************************************
// Pickup (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Classe base abstrata para itens colet�veis
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_PICKUP_H_
#define _SURVIVEUNTILDAWN_PICKUP_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"

// ---------------------------------------------------------------------------------

class Pickup : public Object
{
protected:
    Sprite * sprite = nullptr;                  // sprite do pickup

public:
    Pickup(float pX, float pY, Sprite * spr);   // construtor
    virtual ~Pickup();                          // destrutor

    virtual void OnCollect() = 0;               // coleta do item
    void OnCollision(Object * obj);             // resolu��o da colis�o
    virtual void Update();                      // atualiza��o
    virtual void Draw();                        // desenho
};

// ---------------------------------------------------------------------------------

inline void Pickup::Draw()
{ sprite->Draw(x, y, Layer::LOWER, scale, rotation); }

// ---------------------------------------------------------------------------------

#endif