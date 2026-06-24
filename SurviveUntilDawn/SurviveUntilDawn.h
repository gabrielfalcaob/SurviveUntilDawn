/**********************************************************************************
// SurviveUntilDawn (Arquivo de Cabe�alho)
//
// Cria��o:     23 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Demonstra��o de todas as IAs
//
**********************************************************************************/

#ifndef _SurviveUntilDawn_H_
#define _SurviveUntilDawn_H_

// ------------------------------------------------------------------------------

#include "Game.h"
#include "Audio.h"
#include "Scene.h"
#include "Background.h"
#include "Player.h"
#include "Hud.h"
#include "Font.h"

// ------------------------------------------------------------------------------

enum Ids
{
    PLAYER, MISSILE, WALLHIT, WIZARD, DRAGON, GOBLIN, OGRE,
    THEME, START, FIRE, EXPLODE, HITWALL, EXPLOSION,
    XPORB, MAGNET, BOMB, PICKUP_SFX, SWORDSLASH, HEALTHDROP, MAGICMISSILE, LEVEL_UP, BONUS, HEAL, SWORD
    XPORB, MAGNET, BOMB, PICKUP_SFX, SWORDSLASH, HEALTHDROP, MAGICMISSILE, ORBITAL,
    SHOCKWAVE, LIGHTNING
};

// ------------------------------------------------------------------------------

class SurviveUntilDawn : public Game
{
private:
    Background * backg = nullptr;   // pano de fundo
    Hud * hud = nullptr;            // heads up display
    bool viewBBox = false;          // visualiza��o das bouding boxes
    Font * fontUI = nullptr;        // fonte para a tela de level up
    Sprite* iconPower1 = nullptr;   // Orbital
    Sprite* iconPower2 = nullptr;   // Velocidade
    Sprite* iconPower3 = nullptr;   // Ima
    Sprite* iconPower4 = nullptr;   // Lance do Corte
    Sprite* iconPower5 = nullptr;   // Onda de Choque
    Sprite* iconPower6 = nullptr;   // Bencao da Sorte (Raios)
    Sprite* iconPower7 = nullptr;   // Aumento de Dano (Passiva)
    Sprite* iconPower8 = nullptr;   // Resistencia (Armadura)
    Sprite* iconPower9 = nullptr;   // Bonus de XP

public:
    static Player * player;         // ponteiro para o jogador
    static Audio * audio;           // sitema de �udio
    static Scene * scene;           // cena do jogo

    static Image * wizard;          // imagem do inimigo wizard
    static Image * dragon;          // imagem do inimigo dragon
    static Image * goblin;          // imagem do inimigo goblin
    static Image * ogre;            // imagem do inimigo ogre

    static bool viewHUD;            // hud vis�vel

    void Init();                    // inicializa��o
    void Update();                  // atualiza��o
    void Draw();                    // desenho
    void Finalize();                // finaliza��o
};
// ---------------------------------------------------------------------------------

#endif
