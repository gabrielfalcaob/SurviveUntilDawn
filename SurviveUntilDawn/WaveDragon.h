/**********************************************************************************
// WaveDragon (Arquivo de Cabe�alho)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Dragon
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_WAVEDRAGON_H_
#define _SURVIVEUNTILDAWN_WAVEDRAGON_H_

// --------------------------------------------------------------------------------

#include "Object.h"
#include "Timer.h"
#include "Random.h"

// --------------------------------------------------------------------------------

class WaveDragon : public Object
{
private:
    Random<float> secs;         // valor aleat�rio de segundos
    Timer timer;                // medidor de tempo
    float delay;                // atraso para a pr�xima onda

public:
    WaveDragon();               // construtor
    ~WaveDragon();              // destrutor

    void Update();              // atualiza��o
    void Draw();                // desenho
};

// ------------------------------------------------------------------------------

#endif