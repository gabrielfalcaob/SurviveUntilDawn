/**********************************************************************************
// WaveOgre (Arquivo de Cabe�alho)
//
// Cria��o:     06 Ago 2019
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cria uma onda de inimigos Ogre
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_WAVEOGRE_H_
#define _SURVIVEUNTILDAWN_WAVEOGRE_H_

// --------------------------------------------------------------------------------

#include "Object.h"
#include "Sprite.h"
#include "Timer.h"
#include "Random.h"

// --------------------------------------------------------------------------------

class WaveOgre : public Object
{
private:
    int numX, numY;             // n�mero de inimigos que cabem na tela
    float posX, posY;           // posi��o dos inimigos
    Random<int> position;       // valor aleat�rio para posi��o
    Random<float> delay;        // valor aleat�rio para tempo
    Timer timer;                // medidor de tempo
    float waveDelay;            // atraso para a pr�xima onda
    bool newWave;               // enviar nova onda

public:
    WaveOgre();                 // construtor
    ~WaveOgre();                // destrutor

    void Update();              // atualiza��o
    void Draw();                // desenho
};

// ------------------------------------------------------------------------------

#endif