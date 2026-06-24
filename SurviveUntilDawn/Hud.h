/**********************************************************************************
// Hud (Arquivo de Cabe�alho)
//
// Cria��o:     02 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Heads Up Display
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_HUD_H_
#define _SurviveUntilDawn_HUD_H_

// --------------------------------------------------------------------------------

#include "Object.h"
#include "Sprite.h"
#include "Font.h"
#include "Player.h"
#include <sstream>
using std::stringstream;

// --------------------------------------------------------------------------------

class Hud : public Object
{
private:
    Font * font = nullptr;              // fonte para exibi��o normal
    Font * bold = nullptr;              // fonte para exibi��o negrito

    stringstream text;                  // texto tempor�rio
    uint frameCount;                    // contador de quadros por segundo
    float totalTime;                    // tempo total transcorrido
    uint fps;                           // valor para exibi��o do fps

public:
    Player* playerRef = nullptr;        // referencia para o jogador
    static uint missiles;               // n�mero de m�sseis na tela
    static uint ogres;                  // n�mero de ogros
    static uint goblins;                // n�mero de goblins
    static uint wizards;                // n�mero de magos
    static uint dragons;                // n�mero de drag�es
    static uint particles;              // n�mero de part�culas ativas
    static uint xpOrbs;                 // n�mero de orbs de XP na tela
    static uint playerXP;               // XP acumulado do jogador
    static uint playerLevel;            // n�vel atual do jogador

    Hud();                              // construtor
    ~Hud();                             // destrutor

    void Update();                      // atualiza��o
    void Draw();                        // desenho
    void OnCollision(Object* obj) {}    // colisao (vazio, apenas para garantir a interface)
};

// ------------------------------------------------------------------------------

#endif
