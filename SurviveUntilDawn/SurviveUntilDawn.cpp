/**********************************************************************************
// SurviveUntilDawn (C�digo Fonte)
//
// Cria��o:     23 Out 2012
// Atualiza��o: 11 Nov 2021
// Compilador:  Visual C++ 2022
//
// Descri��o:   Demonstra��o de todas as IAs

//
**********************************************************************************/

#include <iostream>
#include "Resources.h"
#include "SurviveUntilDawn.h"
#include "Engine.h"
#include "Delay.h"
#include "Spawner.h"

// ------------------------------------------------------------------------------

Player * SurviveUntilDawn::player  = nullptr;
Audio  * SurviveUntilDawn::audio   = nullptr;
Scene  * SurviveUntilDawn::scene   = nullptr;
bool     SurviveUntilDawn::viewHUD = false;

bool isGamePaused = false;
Image  * SurviveUntilDawn::wizard  = nullptr;
Image  * SurviveUntilDawn::dragon  = nullptr;
Image  * SurviveUntilDawn::goblin  = nullptr;
Image  * SurviveUntilDawn::ogre    = nullptr;

// ------------------------------------------------------------------------------

void SurviveUntilDawn::Init()
{
    // cria sistema de �udio
    audio = new Audio();
    audio->Add(START, "Resources/Start.wav");
    audio->Add(THEME, "Resources/Theme.wav");
    audio->Add(FIRE, "Resources/Fire.wav", 2);
    audio->Add(HITWALL, "Resources/Hitwall.wav", 5);
    audio->Add(EXPLODE, "Resources/Explode.wav", 3);
    audio->Add(OGRE, "Resources/Orange.wav", 1);
    audio->Add(GOBLIN, "Resources/Magenta.wav", 2);
    audio->Add(WIZARD, "Resources/Blue.wav", 2);
    audio->Add(DRAGON, "Resources/Green.wav", 2);

    // ajusta volumes
    audio->Volume(START, 0.30f);
    audio->Volume(THEME, 0.60f);
    audio->Volume(FIRE, 0.10f);
    audio->Volume(EXPLODE, 0.15f);
    audio->Volume(OGRE, 0.90f);
    audio->Volume(GOBLIN, 0.40f);
    audio->Volume(WIZARD, 0.20f);
    audio->Volume(DRAGON, 0.75f);

    // carrega imagens das geometrias
    wizard = new Image("Resources/Wizard.png");
    dragon = new Image("Resources/Dragon.png");
    goblin = new Image("Resources/Goblin.png");
    ogre   = new Image("Resources/Ogre.png");

    // carrega/incializa objetos
    backg   = new Background("Resources/map.jpg");
    player  = new Player();
    scene   = new Scene();
    hud     = new Hud();
    hud->playerRef = player;

    // inicializa fonte e �cones para a tela de level up
    fontUI = new Font("Resources/Tahoma14.png");
    fontUI->Spacing("Resources/Tahoma14.dat");
    iconPower1 = new Sprite("Resources/Orange.png"); // Orbital
    iconPower2 = new Sprite("Resources/Green.png");  // Velocidade
    iconPower3 = new Sprite("Resources/Blue.png");   // Ima
    iconPower4 = new Sprite("Resources/Magenta.png"); // Lance do Corte
    iconPower5 = new Sprite("Resources/Explo.png"); // Onda de Choque
    iconPower6 = new Sprite("Resources/Explo.png"); // Bencao da Sorte (Raios)
    iconPower7 = new Sprite("Resources/Orange.png");   // Aumento de Dano (Passiva)
    iconPower8 = new Sprite("Resources/Blue.png");     // Resistencia (Armadura)
    iconPower9 = new Sprite("Resources/Green.png");    // Bonus de XP

    // adiciona objetos na cena
    scene->Add(player, MOVING);
    scene->Add(new Spawner(player), STATIC);
    scene->Add(new Delay(), STATIC);

    // ----------------------
    // inicializa a viewport
    // ----------------------

    // calcula posi��o para manter viewport centralizada
    float difx = (game->Width() - window->Width()) / 2.0f;
    float dify = (game->Height() - window->Height()) / 2.0f;

    // inicializa viewport para o centro do mundo
    viewport.left = 0.0f + difx;
    viewport.right = viewport.left + window->Width();
    viewport.top = 0.0f + dify;
    viewport.bottom = viewport.top + window->Height();
}

// ------------------------------------------------------------------------------

void SurviveUntilDawn::Update()
{
    // sai com o pressionamento da tecla ESC
    if (window->KeyDown(VK_ESCAPE))
        window->Close();

    if (!isGamePaused)
    {
        // atualiza cena e detecta colisões
        scene->Update();
        scene->CollisionDetection();
    }
    else
    {
        // --- LÓGICA DO MENU DE LEVEL UP ---
        if (window->KeyPress('1') || window->KeyPress(VK_NUMPAD1)) {
            player->ApplyPowerUp(player->choice1);
            isGamePaused = false;
            std::cout << "Poder 1 escolhido!\n";
        }
        else if (window->KeyPress('2') || window->KeyPress(VK_NUMPAD2)) {
            player->ApplyPowerUp(player->choice2);
            isGamePaused = false;
            std::cout << "Poder 2 escolhido!\n";
        }
        else if (window->KeyPress('3') || window->KeyPress(VK_NUMPAD3)) {
            player->ApplyPowerUp(player->choice3);
            isGamePaused = false;
            std::cout << "Poder 3 escolhido!\n";
        }
    }

    // ---------------------------------------------------
    // atualiza a viewport
    // ---------------------------------------------------

    viewport.left   = player->X() - window->CenterX();
    viewport.right  = player->X() + window->CenterX();
    viewport.top    = player->Y() - window->CenterY();
    viewport.bottom = player->Y() + window->CenterY();

    if (viewport.left < 0)
    {
        viewport.left  = 0;
        viewport.right = window->Width();
    }
    else if (viewport.right > game->Width())
    {
        viewport.left  = game->Width() - window->Width();
         viewport.right = game->Width();
    }

    if (viewport.top < 0)
    {
        viewport.top  = 0;
        viewport.bottom = window->Height();
    }
    else if (viewport.bottom > game->Height())
    {
        viewport.top = game->Height() - window->Height();
        viewport.bottom = game->Height();
    }

    // ---------------------------------------------------

    // atualiza o painel de informa��es
    hud->Update();

    // ativa ou desativa a bounding box
    if (window->KeyPress('B'))
        viewBBox = !viewBBox;

    // ativa ou desativa o heads up display
    if (window->KeyPress('H'))
        viewHUD = !viewHUD;
}

// ------------------------------------------------------------------------------

void SurviveUntilDawn::Draw()
{
    // desenha pano de fundo
    backg->Draw(viewport);

    // desenha a cena
    scene->Draw();

    // desenha o painel de informacao (HUD sempre por cima da cena)
    if (hud)
        hud->Draw();

    // tela de level up quando o jogo estah pausado
    if (isGamePaused)
    {
        // Font::Draw usa coordenadas de TELA (screen space)
        float cx = window->Width()  / 2.0f;
        float cy = window->Height() / 2.0f;

        const char* txt1 = ""; Sprite* spr1 = nullptr;
        if (player->choice1 == 1) { txt1 = "1 - Orbital"; spr1 = iconPower1; }
        else if (player->choice1 == 2) { txt1 = "1 - Velocidade"; spr1 = iconPower2; }
        else if (player->choice1 == 3) { txt1 = "1 - Raio do Ima"; spr1 = iconPower3; }
        else if (player->choice1 == 4) { txt1 = "1 - Lance do Corte"; spr1 = iconPower4; }
        else if (player->choice1 == 5) { txt1 = "1 - Onda de Choque (R)"; spr1 = iconPower5; }
        else if (player->choice1 == 6) { txt1 = "1 - Bencao da Sorte (E)"; spr1 = iconPower6; }
        else if (player->choice1 == 7) { txt1 = "1 - Aumento de Dano (Passiva)"; spr1 = iconPower7; }
        else if (player->choice1 == 8) { txt1 = "1 - Resistencia (Armadura)"; spr1 = iconPower8; }
        else if (player->choice1 == 9) { txt1 = "1 - Bonus de XP (+30%)"; spr1 = iconPower9; }

        const char* txt2 = ""; Sprite* spr2 = nullptr;
        if (player->choice2 == 1) { txt2 = "2 - Orbital"; spr2 = iconPower1; }
        else if (player->choice2 == 2) { txt2 = "2 - Velocidade"; spr2 = iconPower2; }
        else if (player->choice2 == 3) { txt2 = "2 - Raio do Ima"; spr2 = iconPower3; }
        else if (player->choice2 == 4) { txt2 = "2 - Lance do Corte"; spr2 = iconPower4; }
        else if (player->choice2 == 5) { txt2 = "2 - Onda de Choque (R)"; spr2 = iconPower5; }
        else if (player->choice2 == 6) { txt2 = "2 - Bencao da Sorte (E)"; spr2 = iconPower6; }
        else if (player->choice2 == 7) { txt2 = "2 - Aumento de Dano (Passiva)"; spr2 = iconPower7; }
        else if (player->choice2 == 8) { txt2 = "2 - Resistencia (Armadura)"; spr2 = iconPower8; }
        else if (player->choice2 == 9) { txt2 = "2 - Bonus de XP (+30%)"; spr2 = iconPower9; }

        const char* txt3 = ""; Sprite* spr3 = nullptr;
        if (player->choice3 == 1) { txt3 = "3 - Orbital"; spr3 = iconPower1; }
        else if (player->choice3 == 2) { txt3 = "3 - Velocidade"; spr3 = iconPower2; }
        else if (player->choice3 == 3) { txt3 = "3 - Raio do Ima"; spr3 = iconPower3; }
        else if (player->choice3 == 4) { txt3 = "3 - Lance do Corte"; spr3 = iconPower4; }
        else if (player->choice3 == 5) { txt3 = "3 - Onda de Choque (R)"; spr3 = iconPower5; }
        else if (player->choice3 == 6) { txt3 = "3 - Bencao da Sorte (E)"; spr3 = iconPower6; }
        else if (player->choice3 == 7) { txt3 = "3 - Aumento de Dano (Passiva)"; spr3 = iconPower7; }
        else if (player->choice3 == 8) { txt3 = "3 - Resistencia (Armadura)"; spr3 = iconPower8; }
        else if (player->choice3 == 9) { txt3 = "3 - Bonus de XP (+30%)"; spr3 = iconPower9; }

        fontUI->Draw(cx - 80, cy - 60, "LEVEL UP! Escolha seu poder:", { 1,1,1,1 });
        fontUI->Draw(cx - 80, cy - 20, txt1, { 1,1,0,1 });
        fontUI->Draw(cx - 80, cy + 10, txt2, { 0,1,1,1 });
        fontUI->Draw(cx - 80, cy + 40, txt3, { 1,0.5f,0,1 });

        // Sprite::Draw usa coordenadas de MUNDO (world space)
        float wx = viewport.left + cx;
        float wy = viewport.top  + cy;

        if (spr1) spr1->Draw(wx - 130, wy - 20, Layer::FRONT, 0.5f);
        if (spr2) spr2->Draw(wx - 130, wy + 10, Layer::FRONT, 0.5f);
        if (spr3) spr3->Draw(wx - 130, wy + 40, Layer::FRONT, 0.5f);
    }

    // desenha bounding box por ULTIMO (acima de sprites, HUD e level-up)
    if (viewBBox)
        scene->DrawBBox();
}

// ------------------------------------------------------------------------------

void SurviveUntilDawn::Finalize()
{
    delete fontUI;
    delete iconPower1;
    delete iconPower2;
    delete iconPower3;
    delete iconPower4;
    delete iconPower5;
    delete iconPower6;
    delete iconPower7;
    delete iconPower8;
    delete iconPower9;
    delete audio;
    delete hud;
    delete scene;
    delete backg;

    delete wizard;
    delete dragon;
    delete goblin;
    delete ogre;
}


// ------------------------------------------------------------------------------
//                                  WinMain
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // cria motor do jogo
    Engine * engine = new Engine();

    // configura janela
    //engine->window->Mode(WINDOWED);
    //engine->window->Size(1152, 648);
    engine->window->Mode(BORDERLESS);
    engine->window->Color(0, 0, 0);
    engine->window->Title("Survive Until Dawn");
    engine->window->Icon(IDI_ICON);
    engine->window->Cursor(IDC_CURSOR);
    engine->window->HideCursor(true);
    //engine->graphics->VSync(true);

    // cria o jogo
    Game * game = new SurviveUntilDawn();

    // configura o jogo
    game->Size(3840, 2160);

    // inicia execu��o
    int status = engine->Start(game);

    // destr�i motor
    delete engine;

    // encerra
    return status;
}

// ----------------------------------------------------------------------------
