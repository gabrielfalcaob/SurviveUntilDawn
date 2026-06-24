/**********************************************************************************
// Scene (C�digo Fonte)
// 
// Cria��o:     16 Mar 2012
// Atualiza��o: 28 Set 2023
// Compilador:  Visual C++ 2022
//
// Descri��o:   Define uma classe para gerenciar o cen�rio do jogo.
//
//              Um gerenciador de cena � respons�vel por guardar os objetos
//              da cena, atualizando-os e desenhando-os de forma mais pr�tica.
//              Ele pode ser usado tamb�m para outras tarefas que impliquem em
//              percorrer a lista de objetos, como detec��o de colis�o.
//
**********************************************************************************/

#include "Engine.h"
#include "Scene.h"
#include "Object.h"

// ---------------------------------------------------------------------------------

Scene::Scene()
{

}

// ---------------------------------------------------------------------------------

Scene::~Scene()
{
    // remove da mem�ria os objetos est�ticos
    for (auto obj : statics)
        delete obj;

    // remove da mem�ria os objetos em movimento
    for (auto obj : moving)
        delete obj;
}

// ---------------------------------------------------------------------------------


void Scene::Add(Object* obj, int type)
{
    // insere novo objeto na cena
    if (type == STATIC)
        statics.push_back(obj);
    else
        moving.push_back(obj);
}

// ---------------------------------------------------------------------------------

void Scene::Remove(Object* obj, int type)
{
    // remove objeto da cena
    if (type == STATIC)
        statics.remove(obj);
    else
        moving.remove(obj);
}

// ---------------------------------------------------------------------------------

void Scene::Remove()
{
    // remove objeto da cena
    if (processing == STATIC)
        statics.remove(*it);
    else
        moving.remove(*it);
}

// ---------------------------------------------------------------------------------

void Scene::Delete(Object* obj, int type)
{
    toDelete.push_back(ObjectDel{ obj, type });
}

// ---------------------------------------------------------------------------------

void Scene::Delete()
{
    toDelete.push_back(ObjectDel{ *it, processing });
}

// ---------------------------------------------------------------------------------

uint Scene::Size()
{
    return uint(moving.size() + statics.size());
}

// ---------------------------------------------------------------------------------

void Scene::ProcessDeleted()
{
    // remove objetos duplicados
    toDelete.sort();
    toDelete.unique();

    for (const auto & [obj, type] : toDelete)
    {
        // libera mem�ria ocupada pelo objeto
        delete obj;

        // remove objeto da cena
        if (type == STATIC)
            statics.remove(obj);
        else
            moving.remove(obj);
    }

    // esvazia a lista 
    toDelete.clear();
}

// ---------------------------------------------------------------------------------

void Scene::Update()
{
    // atualiza todos os objetos est�ticos
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Update();

    // atualiza todos os objetos em movimento
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Update();

    ProcessDeleted();
}

// ---------------------------------------------------------------------------------

void Scene::Draw()
{
    // desenha todos os objetos est�ticos
    processing = STATIC;
    for (it = statics.begin(); it != statics.end(); ++it)
        (*it)->Draw();

    // desenha todos os objetos em movimento
    processing = MOVING;
    for (it = moving.begin(); it != moving.end(); ++it)
        (*it)->Draw();
}

// ---------------------------------------------------------------------------------

void Scene::DrawBBox()
{
    // descarrega todos os sprites pendentes primeiro
    Engine::renderer->Render();

    // inicia desenho de pixels (renderiza por cima de tudo)
    Engine::renderer->BeginPixels();

    // desenha bounding box dos objetos est�ticos
    for (const auto& obj : statics)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
            if (obj->BBox()->Type() == POLYGON_T)
                Engine::renderer->Draw(((Poly*)obj->BBox())->BBox(), 0x7fff00ff);
        }
    }

    // desenha bounding box dos objetos em movimento
    for (const auto& obj : moving)
    {
        if (obj->BBox())
        {
            Engine::renderer->Draw(obj->BBox(), 0xffff00ff);
            if (obj->BBox()->Type() == POLYGON_T)
                Engine::renderer->Draw(((Poly*)obj->BBox())->BBox(), 0x7fff00ff);
        }
    }

    // finaliza desenho de pixels
    Engine::renderer->EndPixels();
}

// ---------------------------------------------------------------------------------

void Scene::Begin()
{
    // aponta para o primeiro elemento de cada lista
    its = statics.begin();
    itm = moving.begin();
    processing = STATIC;
}

// ---------------------------------------------------------------------------------

Object * Scene::Next()
{
    // se apontador aponta para objeto v�lido
    if (its != statics.end())
    {
        // passa ao pr�ximo objeto
        // guarda apontador para o anterior
        it = its++;
        return *it;
    }
    else
        if (itm != moving.end())
        {
            processing = MOVING;
            it = itm++;
            return *it;
        }
        else
            // chegou ao fim das listas
            return nullptr;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point* p, Point* q)
{
    // se as coordenadas inteiras s�o iguais 
    if (int(p->X()) == int(q->X()) && int(p->Y()) == int(q->Y()))
        return true;

    // caso contr�rio n�o h� colis�o
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Rect * r)
{
    // se as coordenadas  do ponto est�o dentro do ret�ngulo
    if (p->X() >= r->Left() && p->X() <= r->Right())
        if (p->Y() >= r->Top() && p->Y() <= r->Bottom())
            return true;

    // caso contr�rio n�o h� colis�o
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point * p, Circle * c)
{
    // se a dist�ncia entre o ponto e o centro do c�rculo
    // for menor que o raio do c�rculo ent�o h� colis�o
    if (Point::Distance(*p, Point(c->CenterX(), c->CenterY())) <= c->Radius())
        return true;
    else
        return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Point* p, Poly* pol)
{
    // se o ponto colidir com a bounding box do pol�gono,
    // passe para uma investiga��o mais profunda e lenta
    // caso contr�rio n�o h� colis�o

    if (Collision(p, pol->BBox()))
    {
        bool crossings = false;
        int nvertex = pol->vertexCount;
        Point * vertex = pol->vertexList;

        float Ix, Iy, Jx, Jy;
        float Ixr, Iyr, Jxr, Jyr;
        float Ixs, Iys, Jxs, Jys;
        const double PIunder180 = 0.0174532925194444;

        // converte �ngulo de rota��o para radianos
        float theta = float(pol->Rotation() * PIunder180);

        for (int i = 0, j = nvertex - 1; i < nvertex; j = i++)
        {
            // aplica rota��o aos pontos
            Ixr = float(vertex[i].X() * cos(theta) - vertex[i].Y() * sin(theta));
            Iyr = float(vertex[i].X() * sin(theta) + vertex[i].Y() * cos(theta));
            Jxr = float(vertex[j].X() * cos(theta) - vertex[j].Y() * sin(theta));
            Jyr = float(vertex[j].X() * sin(theta) + vertex[j].Y() * cos(theta));

            // aplica escala aos pontos
            Ixs = Ixr * pol->Scale();
            Iys = Iyr * pol->Scale();
            Jxs = Jxr * pol->Scale();
            Jys = Jyr * pol->Scale();

            // transforma coordenadas locais em globais
            Ix = pol->X() + Ixs;
            Iy = pol->Y() + Iys;
            Jx = pol->X() + Jxs;
            Jy = pol->Y() + Jys;

            if (((Iy < p->Y()) && (Jy >= p->Y())) || ((Jy < p->Y()) && (Iy >= p->Y())))
                if (Jx - (((Jy - p->Y()) * (Jx - Ix)) / (Jy - Iy)) < p->X())
                    crossings = !crossings;
        }

        return crossings;
    }

    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * ra, Rect * rb)
{
    // verificando sobreposi��o no eixo x
    bool overlapX = (rb->Left() <= ra->Right() && ra->Left() <= rb->Right());

    // verificando sobreposi��o no eixo y
    bool overlapY = (rb->Top() <= ra->Bottom() && ra->Top() <= rb->Bottom());

    // existe colis�o se h� sobreposi��o nos dois eixos
    return overlapX && overlapY;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Circle * c)
{
    // encontra o ponto do ret�ngulo mais pr�ximo do centro do c�rculo
    float px, py;

    // eixo x
    if (c->CenterX() < r->Left())
        px = r->Left();
    else
        if (c->CenterX() > r->Right())
            px = r->Right();
        else
            px = c->CenterX();

    // eixo y
    if (c->CenterY() < r->Top())
        py = r->Top();
    else
        if (c->CenterY() > r->Bottom())
            py = r->Bottom();
        else
            py = c->CenterY();

    // verifica se h� colis�o entre este ponto e o c�rculo
    Point point(px, py);
    return Collision(&point, c);
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Rect * r, Poly * pol)
{
    // se o ret�ngulo colidir com a bounding box do pol�gono,
    // passe para uma investiga��o mais profunda e lenta
    // caso contr�rio n�o h� colis�o

    if (Collision(r, pol->BBox()))
    {
        // recupera os cantos do ret�ngulo
        Point corners[4] =
        {
            Point(r->Left(), r->Top()),
            Point(r->Right(), r->Top()),
            Point(r->Right(), r->Bottom()),
            Point(r->Left(), r->Bottom())
        };

        // verifica se algum canto do ret�ngulo est� dentro do pol�gono
        for (int i = 0; i < 4; ++i)
            if (Collision(&corners[i], pol))
                return true;

        // recupera v�rtices do pol�gono
        int nv = pol->vertexCount;
        Point * v = pol->vertexList;

        // converte �ngulo de rota��o para radianos
        const double PIunder180 = 0.0174532925194444;
        float theta = float(pol->Rotation() * PIunder180);

        // verifica se algum v�rtice est� dentro do ret�ngulo
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;

        for (int i = 0; i < nv; ++i)
        {
            // aplica rota��o aos pontos
            pXr = float(v[i].X() * cos(theta) - v[i].Y() * sin(theta));
            pYr = float(v[i].X() * sin(theta) + v[i].Y() * cos(theta));
            
            // aplica escala aos pontos
            pXs = pXr * pol->Scale();
            pYs = pYr * pol->Scale();
            
            // transforma coordenadas locais em globais
            pX = pol->X() + pXs;
            pY = pol->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, r))
                return true;
        }
    }

    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle * ca, Circle * cb)
{
    // deltas podem ser negativos se a subtra��o � feita na ordem errada
    // levando essa possibilidade em conta � melhor pegar os valores absolutos
    float deltaX = abs(ca->CenterX() - cb->CenterX());
    float deltaY = abs(ca->CenterY() - cb->CenterY());

    // calcule a dist�ncia entre os centros dos c�rculos
    float distance = float(sqrt(double(deltaX) * double(deltaX) + double(deltaY) * double(deltaY)));

    // se a dist�ncia � menor que a soma dos raios
    // existe colis�o entre os c�rculos
    if (distance <= (ca->Radius() + cb->Radius()))
        return true;

    // nenhum colis�o detectada
    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Circle* c, Poly* pol)
{
    // se o c�rculo colidir com a bounding box do pol�gono,
    // passe para uma investiga��o mais profunda e lenta
    // caso contr�rio n�o h� colis�o

    if (Collision(c, pol->BBox()))
    {
        // recupera v�rtices do pol�gono
        int nv = pol->vertexCount;
        Point * v = pol->vertexList;

        // TODO: identificar o ponto do c�rculo mais pr�ximo de cada aresta 
        // e verificar se este ponto est� dentro do pol�gono

        // converte �ngulo de rota��o para radianos
        const double PIunder180 = 0.0174532925194444;
        float theta = float(pol->Rotation() * PIunder180);

        // verifica se algum v�rtice est� dentro do c�rculo
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;

        for (int i = 0; i < nv; ++i)
        {
            // aplica rota��o aos pontos
            pXr = float(v[i].X() * cos(theta) - v[i].Y() * sin(theta));
            pYr = float(v[i].X() * sin(theta) + v[i].Y() * cos(theta));

            // aplica escala aos pontos
            pXs = pXr * pol->Scale();
            pYs = pYr * pol->Scale();

            // transforma coordenadas locais em globais
            pX = pol->X() + pXs;
            pY = pol->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, c))
                return true;
        }
    }

    return false;

}

// --------------------------------------------------------------------------------

bool Scene::Collision(Poly* pa, Poly* pb)
{
    // se as bounding boxes estiverem colidindo, 
    // passe para uma investiga��o mais profunda e lenta
    // caso contr�rio n�o h� colis�o

    if (Collision(pa->BBox(), pb->BBox()))
    {
        // recupera v�rtices do pol�gono A
        int nva = pa->vertexCount;
        Point * va = pa->vertexList;
        
        float pX, pY;
        float pXr, pYr;
        float pXs, pYs;
        float theta;

        // converte �ngulo de rota��o para radianos
        const double PIunder180 = 0.0174532925194444;
        theta = float(pa->Rotation() * PIunder180);

        // verifica se v�rtices de A est�o dentro do pol�gono B
        for (int i = 0; i < nva; ++i)
        {
            // aplica rota��o aos pontos
            pXr = float(va[i].X() * cos(theta) - va[i].Y() * sin(theta));
            pYr = float(va[i].X() * sin(theta) + va[i].Y() * cos(theta));

            // aplica escala aos pontos
            pXs = pXr * pa->Scale();
            pYs = pYr * pa->Scale();

            // transforma coordenadas locais em globais
            pX = pa->X() + pXs;
            pY = pa->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, pb))
                return true;
        }

        // recupera v�rtices do pol�gono B
        int nvb = pb->vertexCount;
        Point * vb = pb->vertexList;

        // converte �ngulo de rota��o para radianos
        theta = float(pb->Rotation() * PIunder180);

        // verifica se v�rtices de B est�o dentro do pol�gono A
        for (int i = 0; i < nvb; ++i)
        {
            // aplica rota��o aos pontos
            pXr = float(vb[i].X() * cos(theta) - vb[i].Y() * sin(theta));
            pYr = float(vb[i].X() * sin(theta) + vb[i].Y() * cos(theta));

            // aplica escala aos pontos
            pXs = pXr * pb->Scale();
            pYs = pYr * pb->Scale();

            // transforma coordenadas locais em globais
            pX = pb->X() + pXs;
            pY = pb->Y() + pYs;

            Point p(pX, pY);

            if (Collision(&p, pa))
                return true;
        }
    }

    return false;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Mixed * m, Geometry * s)
{
    bool collision = false;

    // percorra lista at� achar uma colis�o
    auto i = m->shapes.begin();

    while (!collision && i != m->shapes.end())
    {
        switch ((*i)->Type())
        {
            // Point
        case POINT_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Point*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Point*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Point*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Point*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Point*)(*i), (Mixed*)s); break;
            }
            break;

            // Circle
        case CIRCLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Circle*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Circle*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Circle*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Circle*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Circle*)(*i), (Mixed*)s); break;
            }
            break;

            // Rectangle
        case RECTANGLE_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Rect*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Rect*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Rect*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Rect*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Rect*)(*i), (Mixed*)s); break;
            }
            break;

            // Polygon
        case POLYGON_T:
            switch (s->Type())
            {
            case POINT_T:     collision = Collision((Poly*)(*i), (Point*)s); break;
            case CIRCLE_T:    collision = Collision((Poly*)(*i), (Circle*)s); break;
            case RECTANGLE_T: collision = Collision((Poly*)(*i), (Rect*)s); break;
            case POLYGON_T:   collision = Collision((Poly*)(*i), (Poly*)s); break;
            case MIXED_T:     collision = Collision((Poly*)(*i), (Mixed*)s); break;
            }
            break;

            // Mixed
        case MIXED_T:
            collision = Collision((Mixed*)(*i), s);
            break;
        }

        // passa para a pr�xima geometria da forma mista
        ++i;
    }

    return collision;
}

// --------------------------------------------------------------------------------

bool Scene::Collision(Object* oa, Object* ob)
{
    // um dos objetos n�o tem bounding box
    if (!(oa->BBox() && ob->BBox()))
        return false;

    switch (oa->BBox()->Type())
    {
        // Point    
    case POINT_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Point && Point
            return Collision((Point*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Point && Circle
            return Collision((Point*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Point && Rectangle
            return Collision((Point*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Point && Polygon
            return Collision((Point*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Point && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Point && Unknown
            return false;
        }

        // Circle    
    case CIRCLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Circle && Point
            return Collision((Circle*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Circle && Circle
            return Collision((Circle*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Circle && Rectangle
            return Collision((Circle*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Circle && Polygon
            return Collision((Circle*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Circle && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Circle && Unknown
            return false;
        }

        // Rectangle
    case RECTANGLE_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Rectangle && Point
            return Collision((Rect*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Rectangle && Circle
            return Collision((Rect*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Rectangle && Rectangle
            return Collision((Rect*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Rectangle && Polygon
            return Collision((Rect*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Rectangle && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Rectangle && Unknown
            return false;
        }

        // Polygon
    case POLYGON_T:

        switch (ob->BBox()->Type())
        {
        case POINT_T:
            // Polygon && Point
            return Collision((Poly*)oa->BBox(), (Point*)ob->BBox());

        case CIRCLE_T:
            // Polygon && Circle
            return Collision((Poly*)oa->BBox(), (Circle*)ob->BBox());

        case RECTANGLE_T:
            // Polygon && Rectangle
            return Collision((Poly*)oa->BBox(), (Rect*)ob->BBox());

        case POLYGON_T:
            // Polygon && Polygon
            return Collision((Poly*)oa->BBox(), (Poly*)ob->BBox());

        case MIXED_T:
            // Polygon && Mixed
            return Collision(oa->BBox(), (Mixed*)ob->BBox());

        default:
            // Polygon && Unknown
            return false;
        }

        // Mixed
    case MIXED_T:
        // Mixed && Geometry
        return Collision((Mixed*)oa->BBox(), ob->BBox());


        // Unknown
    default:
        return false;
    }
}

// ---------------------------------------------------------------------------------

void Scene::CollisionDetection()
{
    // iteradores para as listas
    list<Object*>::iterator start, end;

    // limpa lista de colis�es
    collisions.clear();

    // -------------------
    // Detec��o da colis�o
    // -------------------

    // se existe pelo menos dois objetos que se movem, teste a colis�o entre eles
    if (moving.size() >= 2)
    {
        // pega iterador para �ltimo elemento da lista
        end = moving.end();
        end--;

        // testa colis�o entre todos os pares de objetos que se movem
        for (auto i = moving.begin(); i != end; ++i)
        {
            start = i;

            // j inicia no segundo elemento da lista
            for (auto j = ++start; j != moving.end(); ++j)
            {
                if (Collision(*i, *j))
                    collisions.push_back(ObjectPair(*i, *j));
            }
        }
    }

    // testa colis�o entre objetos que se movem e objetos est�ticos
    for (auto i : moving)
    {
        for (auto j : statics)
        {
            if (Collision(i, j))
                collisions.push_back(ObjectPair(i, j));
        }
    }

    // --------------------
    // Resolu��o da colis�o
    // --------------------

    if (!collisions.empty())
    {
        for (const auto & [a,b] : collisions)
        {
            a->OnCollision(b);
            b->OnCollision(a);
        }
    }

    ProcessDeleted();
}

// --------------------------------------------------------------------------------
