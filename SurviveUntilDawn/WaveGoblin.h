/**********************************************************************************
// WaveGoblin (Arquivo de Cabe�alho)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Goblin
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_WAVEGOBLIN_H_
#define _SURVIVEUNTILDAWN_WAVEGOBLIN_H_

// --------------------------------------------------------------------------------

#include "Object.h"
#include "Timer.h"
#include "Random.h"

// --------------------------------------------------------------------------------

class WaveGoblin : public Object
{
private:
    float pX, pY;               // posi��o dos inimigos
    Random<float> posX;         // valor aleat�rio para posi��o X
    Random<float> posY;         // valor aleat�rio para posi��o Y
    Random<float> secs;         // valor aleat�rio de segundos
    Timer timer;                // medidor de tempo
    float delay;                // atraso para a pr�xima onda

public:
    WaveGoblin();               // construtor
    ~WaveGoblin();              // destrutor

    void Update();              // atualiza��o
    void Draw();                // desenho
};

// ------------------------------------------------------------------------------

#endif