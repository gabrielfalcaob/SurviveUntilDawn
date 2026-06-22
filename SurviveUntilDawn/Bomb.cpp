/**********************************************************************************
// Bomb (C�digo Fonte)
//
// Cria��o:     21 Jun 2026
// Compilador:  Visual C++ 2022
//
// Descri��o:   Elimina todos os inimigos na tela
//
**********************************************************************************/

#include "Bomb.h"
#include "SurviveUntilDawn.h"
#include "Wizard.h"
#include "Dragon.h"
#include "Goblin.h"
#include "Ogre.h"
#include <vector>

// ---------------------------------------------------------------------------------

Bomb::Bomb(float pX, float pY) : Pickup(pX, pY, new Sprite("Resources/Bomb.png"))
{
    type = BOMB;
}

// ---------------------------------------------------------------------------------

Bomb::~Bomb()
{
}

// -------------------------------------------------------------------------------

void Bomb::OnCollect()
{
    SurviveUntilDawn::scene->Begin();
    Object* obj = SurviveUntilDawn::scene->Next();

    while (obj != nullptr)
    {
        uint id = obj->Type();

        // Verifica se é um dos monstros
        if (id == GOBLIN || id == OGRE || id == WIZARD || id == DRAGON)
        {
            // Como a IA criou o método Kill(), chame-o aqui
            // (Você pode precisar fazer um cast para a classe base dos inimigos ou testar um por um)
            if (id == GOBLIN) ((Goblin*)obj)->Kill();
            else if (id == OGRE) ((Ogre*)obj)->Kill();
            else if (id == WIZARD) ((Wizard*)obj)->Kill();
            else if (id == DRAGON) ((Dragon*)obj)->Kill();
        }

        obj = SurviveUntilDawn::scene->Next();
    }
}

// -------------------------------------------------------------------------------
