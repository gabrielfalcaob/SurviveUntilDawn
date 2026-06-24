/**********************************************************************************
// Spawner (Código Fonte)
//
// Criação:     23 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descrição:   Gerenciador dinâmico de spawn de inimigos relativo à câmera
//
**********************************************************************************/

#include <ctime>
#include <cmath>
#include "SurviveUntilDawn.h"
#include "Spawner.h"
#include "Goblin.h"
#include "Ogre.h"
#include "Wizard.h"

// ---------------------------------------------------------------------------------

Spawner::Spawner(Player* p)
{
    playerRef = p;
    type = 0; // sem colisão (sem BBox definido)

    // semente unica para rand()
    static bool seeded = false;
    if (!seeded)
    {
        srand((unsigned)time(0));
        seeded = true;
    }

    spawnTimer.Start();
    waveTimer.Start();
}

// ---------------------------------------------------------------------------------

void Spawner::Update()
{
    if (playerRef == nullptr)
        return;

    // --- Spawn contínuo no perímetro da tela ---
    if (spawnTimer.Elapsed(spawnInterval))
    {
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float dist = 600.0f;
        float sx = playerRef->X() + cos(angle) * dist;
        float sy = playerRef->Y() + sin(angle) * dist;
        int roll = rand() % 100;
        if (roll < 60)
            SurviveUntilDawn::scene->Add(new Goblin(sx, sy, playerRef), MOVING);
        else if (roll < 85)
            SurviveUntilDawn::scene->Add(new Wizard(sx, sy, playerRef), MOVING);
        else
        {
            float ogreAng = Line::Angle(Point(sx, sy), Point(playerRef->X(), playerRef->Y()));
            SurviveUntilDawn::scene->Add(new Ogre(sx, sy, ogreAng), MOVING);
        }
        spawnTimer.Start();
    }

    // --- Onda circular de 30 goblins ---
    if (waveTimer.Elapsed(waveInterval))
    {
        for (int i = 0; i < 30; i++)
        {
            float angle = (i * 360.0f / 30.0f) * 3.14159f / 180.0f;
            float dist = 650.0f;
            float sx = playerRef->X() + cos(angle) * dist;
            float sy = playerRef->Y() + sin(angle) * dist;
            SurviveUntilDawn::scene->Add(new Goblin(sx, sy, playerRef), MOVING);
        }
        waveTimer.Start();
    }
}

// -------------------------------------------------------------------------------

void Spawner::Draw()
{
    // objeto invisivel — sem desenho
}

// -------------------------------------------------------------------------------

void Spawner::OnCollision(Object* obj)
{
    // objeto sem colisao — sem resolucao
}

// -------------------------------------------------------------------------------
