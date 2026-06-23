/**********************************************************************************
// Spawner (Arquivo de Cabeçalho)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Gerenciador dinâmico de spawn de inimigos relativo à câmera
//
**********************************************************************************/

#ifndef _SURVIVEUNTILDAWN_SPAWNER_H_
#define _SURVIVEUNTILDAWN_SPAWNER_H_

// ---------------------------------------------------------------------------------

#include <cmath>
#include "Object.h"
#include "Timer.h"
#include "Player.h"

// ---------------------------------------------------------------------------------

class Spawner : public Object
{
private:
    Player* playerRef;              // referencia para o jogador
    Timer spawnTimer;               // timer do spawn continuo
    float spawnInterval = 1.0f;     // intervalo entre spawns (segundos)
    Timer waveTimer;                // timer da onda circular
    float waveInterval = 30.0f;     // intervalo entre ondas (segundos)

public:
    Spawner(Player* p);             // construtor
    void Update();                  // atualizacao
    void Draw();                    // desenho (vazio — objeto invisivel)
    void OnCollision(Object* obj);  // resolucao de colisao (vazio)
};

// ---------------------------------------------------------------------------------

#endif
