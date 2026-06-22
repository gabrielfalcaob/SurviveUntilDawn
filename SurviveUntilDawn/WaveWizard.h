/**********************************************************************************
// WaveWizard (Arquivo de Cabe�alho)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Wizard
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_WAVEWIZARD_H_
#define _SURVIVEUNTILDAWN_WAVEWIZARD_H_

// --------------------------------------------------------------------------------

#include "Object.h"
#include "Timer.h"
#include "Random.h"

// --------------------------------------------------------------------------------

class WaveWizard : public Object
{
private:
    float pX, pY;               // posi��o dos inimigos
    Random<float> posX;         // valor aleat�rio para posi��o X
    Random<float> posY;         // valor aleat�rio para posi��o Y
    Random<float> secs;         // valor aleat�rio de segundos
    Timer timer;                // medidor de tempo
    float delay;                // atraso para a pr�xima onda

public:
    WaveWizard();               // construtor
    ~WaveWizard();              // destrutor

    void Update();              // atualiza��o
    void Draw();                // desenho
};

// ------------------------------------------------------------------------------

#endif