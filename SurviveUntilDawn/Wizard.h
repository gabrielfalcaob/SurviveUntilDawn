/**********************************************************************************
// Wizard (Arquivo de Cabe�alho)
//
// Cria��o:     10 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Objeto faz uma persegui��o suavizada
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_WIZARD_H_
#define _SurviveUntilDawn_WIZARD_H_

// ---------------------------------------------------------------------------------

#include "Object.h"
#include "Types.h"
#include "Sprite.h"
#include "Vector.h"
#include "Player.h"

// ---------------------------------------------------------------------------------

class Wizard : public Object
{
private:
    Sprite * sprite;                            // sprite do objeto
    Player * player;                            // ponteiro para jogador
    Vector speed;                               // velocidade e dire��o
    float factor;                               // fator de escala

public:
    Wizard(float pX, float pY, Player * p);     // construtor
    ~Wizard();                                  // destrutor

    void Kill();                                // elimina��o do inimigo
    void OnCollision(Object* obj);              // resolu��o da colis�o
    void Update();                              // atualiza��o
    void Draw();                                // desenho
};

// ---------------------------------------------------------------------------------

inline void Wizard::Draw()
{ sprite->Draw(x, y, Layer::LOWER, scale, rotation); }

// ---------------------------------------------------------------------------------


#endif