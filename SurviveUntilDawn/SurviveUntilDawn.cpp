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
    audio->Add(EXPLODE, "Resources/Death.wav", 3);
    audio->Add(OGRE, "Resources/Ogre.wav", 1);
    audio->Add(GOBLIN, "Resources/Goblin.wav", 2);
    audio->Add(WIZARD, "Resources/Wizard.wav", 2);
    audio->Add(DRAGON, "Resources/Dragon.wav", 2);
	audio->Add(LEVEL_UP, "Resources/level_up.wav");
	audio->Add(BONUS, "Resources/Bonus.wav");
	audio->Add(HEAL, "Resources/Heal.wav");

    // ajusta volumes
    audio->Volume(START, 0.90f);
    audio->Volume(THEME, 0.60f);
    audio->Volume(FIRE, 0.10f);
    audio->Volume(EXPLODE, 0.15f);
    audio->Volume(OGRE, 0.10f);
    audio->Volume(GOBLIN, 0.40f);
    audio->Volume(WIZARD, 0.20f);
    audio->Volume(DRAGON, 0.75f);
    audio->Volume(BONUS, 0.50f);
	audio->Volume(HEAL, 0.50f);

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

    // inicializa fonte e �cones para a tela de level up
    fontUI = new Font("Resources/Tahoma14.png");
    fontUI->Spacing("Resources/Tahoma14.dat");
    iconOpcao1 = new Sprite("Resources/Player.png");    // TODO: trocar pela arte final
    iconOpcao2 = new Sprite("Resources/Green.png");     // TODO: trocar pela arte final
    iconOpcao3 = new Sprite("Resources/Blue.png");      // TODO: trocar pela arte final

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

    // tela de level up quando o jogo est� pausado
    if (isGamePaused)
    {
        // Font::Draw usa coordenadas de TELA (screen space)
        float cx = window->Width()  / 2.0f;
        float cy = window->Height() / 2.0f;

        fontUI->Draw(cx - 80, cy - 60, "LEVEL UP! Escolha seu poder:", { 1,1,1,1 });
        fontUI->Draw(cx - 80, cy - 20, "1 - Orbital (Bola de Fogo)",   { 1,1,0,1 });
        fontUI->Draw(cx - 80, cy + 10, "2 - Velocidade de Movimento",  { 0,1,1,1 });
        fontUI->Draw(cx - 80, cy + 40, "3 - Aumentar Raio do Ima",     { 1,0.5f,0,1 });

        // Sprite::Draw usa coordenadas de MUNDO (world space)
        // converter tela->mundo somando viewport.left/top
        float wx = viewport.left + cx;
        float wy = viewport.top  + cy;

        if (iconOpcao1) iconOpcao1->Draw(wx - 130, wy - 20, Layer::FRONT, 0.5f);
        if (iconOpcao2) iconOpcao2->Draw(wx - 130, wy + 10, Layer::FRONT, 0.5f);
        if (iconOpcao3) iconOpcao3->Draw(wx - 130, wy + 40, Layer::FRONT, 0.5f);
    }

    // desenha o painel de informa��es
    if (viewHUD)
        hud->Draw();

    // desenha bounding box
    if (viewBBox)
        scene->DrawBBox();
}

// ------------------------------------------------------------------------------

void SurviveUntilDawn::Finalize()
{
    delete fontUI;
    delete iconOpcao1;
    delete iconOpcao2;
    delete iconOpcao3;
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
