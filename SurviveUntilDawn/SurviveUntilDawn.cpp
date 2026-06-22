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

#include "Resources.h"
#include "SurviveUntilDawn.h"
#include "Engine.h"
#include "Delay.h"

// ------------------------------------------------------------------------------

Player * SurviveUntilDawn::player  = nullptr;
Audio  * SurviveUntilDawn::audio   = nullptr;
Scene  * SurviveUntilDawn::scene   = nullptr;
bool     SurviveUntilDawn::viewHUD = false;
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

    // adiciona objetos na cena
    scene->Add(player, MOVING);
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

    // atualiza cena e calcula colis�es
    scene->Update();
    scene->CollisionDetection();

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
