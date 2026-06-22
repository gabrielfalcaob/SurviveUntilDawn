/**********************************************************************************
// Object (Código Fonte)
//
// Criação:     01 Out 2007
// Atualização: 13 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Essa é a classe base para todos objetos do jogo.
//
//              Um objeto do jogo é qualquer coisa que possámos querer desenhar
//              ou interagir dentro do jogo. Por exemplo, um soldado, um prédio,
//              um projétil, uma forma geométrica, etc.
//
**********************************************************************************/

#include "Object.h"
#include "Engine.h"

// -------------------------------------------------------------------------------
// Inicialização de variáveis estáticas da classe

Window* & Object::window   = Engine::window;        // ponteiro para a janela
Game*   & Object::game     = Engine::game;          // ponteiro para o jogo
float   & Object::gameTime = Engine::frameTime;     // tempo do último quadro

// -------------------------------------------------------------------------------

Object::Object()
{
    posX = posY = 0.0f;     // posição
    posZ = 0.5f;            // profundidade
    scaleFactor = 1.0f;     // escala
    rotationAngle = 0.0f;   // rotação
    type = 0;               // tipo
    bbox = nullptr;         // bounding box
}

// -------------------------------------------------------------------------------

Object::~Object()
{
    if (bbox)
        delete bbox;
}

// -------------------------------------------------------------------------------

void Object::BBox(Geometry* bb)
{
    if (bbox)
        delete bbox;

    bbox = bb;
    bbox->MoveTo(x, y);
    bbox->ScaleTo(scaleFactor);
    bbox->RotateTo(rotationAngle);    
}

// -------------------------------------------------------------------------------

void Object::Translate(float dx, float dy, float dz)
{
    posX += dx;
    posY += dy;
    posZ += dz;

    if (bbox)
        bbox->Translate(dx, dy);
}

// -------------------------------------------------------------------------------

void Object::Scale(float factor)
{
    scaleFactor *= factor;

    if (bbox)
        bbox->Scale(factor);
}

// -------------------------------------------------------------------------------

void Object::ScaleTo(float value)
{
    scaleFactor = value;
    if (bbox)
        bbox->ScaleTo(value);
}

// -------------------------------------------------------------------------------

void Object::Rotate(float angle)
{
    rotationAngle += angle;

    if (bbox)
        bbox->Rotate(angle);
}

// -------------------------------------------------------------------------------

void Object::RotateTo(float value)
{
    rotationAngle = value;

    if (bbox)
        bbox->RotateTo(value);
}

// -------------------------------------------------------------------------------

void Object::MoveTo(float px, float py, float pz)
{
    posX = px;
    posY = py;
    posZ = pz;

    if (bbox)
        bbox->MoveTo(px, py);
}

// -------------------------------------------------------------------------------

void Object::MoveTo(float px, float py)
{
    posX = px;
    posY = py;

    if (bbox)
        bbox->MoveTo(px, py);
}

// -------------------------------------------------------------------------------

void Object::OnCollision(Object* obj)
{
}

// -------------------------------------------------------------------------------