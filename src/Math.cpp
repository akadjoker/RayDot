#include "Math.hpp"
#include <raymath.h>


void AABB::render()
{

    

    Rectangle r;
    float w = x2-x1;
    float h = x2-x1;
    r.x     = x1;
    r.y     = y1;
    r.width  = w;
    r.height = h;

//    DrawRectangleLinesEx(r, 1.2f, LIME);
    
     DrawLine(x1, y1, x2, y1, RED);
     DrawLine(x2, y1, x2, y2, RED);
     DrawLine(x2, y2, x1, y2, RED);
     DrawLine(x1, y2, x1, y1, RED);


}

const Rectangle AABB::getRectangle()const 
{
      Rectangle r;
    float w = x2-x1;
    float h = x2-x1;
    r.x     = x1;
    r.y     = y1;
    r.width  = w;
    r.height = h;
    return r;

}

void AABB::Encapsulate(const float x, const float y)
{
    if (is_clean)
    {
        x1 = x2 = x;
        y1 = y2 = y;
        is_clean = false;
    }
    else
    {
        if (x < x1)
            x1 = x;
        if (x > x2)
            x2 = x;
        if (y < y1)
            y1 = y;
        if (y > y2)
            y2 = y;
    }
}

void AABB::Encapsulate(const AABB &other)
{
    Encapsulate(other.x1, other.y1);
    Encapsulate(other.x1 + other.x2, other.y1+ other.y2);

}

void AABB::Encapsulate(const Vector2 &point)
{
    Encapsulate(point.x, point.y);
}

bool AABB::Contains(const AABB &other) const
{
    if (x1 <= other.x1 && x2 >= other.x2 && y1 <= other.y1 && y2 >= other.y2) 
    {
        return true;
    }

    return false;
}

bool AABB::Contains(const Vector2 &point) const
{
    if (point.x >= x1 && point.x < x2 && point.y >= y1 && point.y < y2) 
    {
        return true;
    }

    return false;
}



bool AABB::Contains(const float x, const float y) const
{
     if (x >= x1 && x < x2 && y >= y1 && y < y2) 
     {
        return true;
    }

    return false;
  
}

bool AABB::Intersect(const AABB *rect) const
{
   if (fabs(x1 + x2 - rect->x1 - rect->x2) < (x2 - x1 + rect->x2 - rect->x1))
        if (fabs(y1 + y2 - rect->y1 - rect->y2) < (y2 - y1 + rect->y2 - rect->y1))
            {
            return true;
        }

    return false;
}

bool AABB::Intersect(const Rectangle *rect) const
{
    if (fabs(x1 + x2 - rect->x - rect->width) < (x2 - x1 + rect->width - rect->x))
        if (fabs(y1 + y2 - rect->y - rect->height) < (y2 - y1 + rect->height - rect->y))
            {
            return true;
        }

    return false;
    
}

bool AABB::Intersect(const AABB *rect, AABB *intersection) const
{
    
    if (fabs(x1 + x2 - rect->x1 - rect->x2) < (x2 - x1 + rect->x2 - rect->x1))
        if (fabs(y1 + y2 - rect->y1 - rect->y2) < (y2 - y1 + rect->y2 - rect->y1))
            {
            intersection->x1 = std::max(x1, rect->x1);
            intersection->x2 = std::min(x2, rect->x2);
            intersection->y1 = std::max(y1, rect->y1);
            intersection->y2 = std::min(y2, rect->y2);
            return true;
        }

    return false;
}

bool AABB::Intersect(const Rectangle *rect, Rectangle *intersection) const
{
    if (fabs(x1 + x2 - rect->x - rect->width) < (x2 - x1 + rect->width - rect->x))
        if (fabs(y1 + y2 - rect->y - rect->height) < (y2 - y1 + rect->height - rect->y))
            {
            intersection->x = std::max(x1, rect->x);
            intersection->width = std::min(x2, rect->width);
            intersection->y = std::max(y1, rect->y);
            intersection->height = std::min(y2, rect->height);
            return true;
        }

    return false;
    
}
bool AABB::Intersect(const float x, const float y, const float r) const
{
    float deltaX = x - std::max(x1, std::min(x, x1 + x2));
    float deltaY = y - std::max(y1, std::min(y, y1 + y2));
    return (deltaX * deltaX + deltaY * deltaY) <= (r * r);
 
}
void AABB::Set(const float _x1, const float _y1, const float _x2, const float _y2) 
{
    x1 = _x1;
    x2 = _x2;
    y1 = _y1;
    y2 = _y2;
    is_clean = false;
}


void AABB::Init(float x, float y, float width, float height)
{
    x1 = x;
    x2 = x + width;
    y1 = y;
    y2 = y + height;
    is_clean = false;
}

void AABB::Set(const float x, const float y, const float r) 
{
    x1 = x - (r/2.0f);
    x2 = x + (r/2.0f);
    y1 = y - (r/2.0f);
    y2 = y + (r/2.0f);
    is_clean = false;
}

 void AABB::Clear() 
 {
        is_clean = true;
}

AABB* AABB::Transform(const Vector2 &pos, const Vector2 &pivot, const Vector2 &scale, float rot,   float width, float height, AABB* rect) 
{
    rect->Clear();

    const auto tx1 = -pivot.x * scale.x;
    const auto ty1 = -pivot.y * scale.y;
    const auto tx2 = (width  - pivot.x) * scale.x;
    const auto ty2 = (height - pivot.y) * scale.y;

    if (rot != 0.0f) 
    {

        const auto cost = cosf(rot*DEG2RAD);
        const auto sint = sinf(rot*DEG2RAD);


        rect->Encapsulate(tx1 * cost - ty1 * sint + pos.x, tx1 * sint + ty1 * cost + pos.y);
        rect->Encapsulate(tx2 * cost - ty1 * sint + pos.x, tx2 * sint + ty1 * cost + pos.y);
        rect->Encapsulate(tx2 * cost - ty2 * sint + pos.x, tx2 * sint + ty2 * cost + pos.y);
        rect->Encapsulate(tx1 * cost - ty2 * sint + pos.x, tx1 * sint + ty2 * cost + pos.y);
    }
    else 
    {
        rect->Encapsulate(tx1 + pos.x, ty1 + pos.y);
        rect->Encapsulate(tx2 + pos.x, ty1 + pos.y);
        rect->Encapsulate(tx2 + pos.x, ty2 + pos.y);
        rect->Encapsulate(tx1 + pos.x, ty2 + pos.y);
    }

    return rect;
}

Matrix2D Matrix2DMult(const Matrix2D curr, const Matrix2D m)
{

    Matrix2D result;

    result.a = curr.a * m.a + curr.b * m.c;
    result.b = curr.a * m.b + curr.b * m.d;
    result.c = curr.c * m.a + curr.d * m.c;
    result.d = curr.c * m.b + curr.d * m.d;

    result.tx = curr.tx * m.a + curr.ty * m.c + m.tx;
    result.ty = curr.tx * m.b + curr.ty * m.d + m.ty;

    return result;
}



Matrix2D::Matrix2D()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

Matrix2D::~Matrix2D()
{
}

void Matrix2D::Identity()
{
    a = 1;
    b = 0;
    c = 0;
    d = 1;
    tx = 0;
    ty = 0;
}

void Matrix2D::Set(float a, float b, float c, float d, float tx, float ty)
{

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->tx = tx;
    this->ty = ty;
}

void Matrix2D::Transform(const Vector2 &center,const Vector2 &origin, const Vector2 &scale, float rotation)
{
    float acos = cos(rotation);
    float asin = sin(rotation);

    float a1 = scale.x * acos;
    float b1 = scale.x * asin;
    float c1 = scale.y * -asin;
    float d1 = scale.y * acos;
    float tx1 = center.x - origin.x * a1 - origin.y * c1;
    float ty1 = center.y - origin.x * b1 - origin.y * d1;

    this->a = a1;
    this->b = b1;
    this->c = c1;
    this->d = d1;
    this->tx = tx1;
    this->ty = ty1;
}

/*

float ExtractRotationFromMatrix(const Matrix2D& matrix)
{
    // Se a matriz for uma matriz 2x2 com apenas informações de rotação, você pode extrair o ângulo diretamente
    // Supondo que 'a' e 'b' representem os elementos de rotação na matriz (aqui é um exemplo genérico)
    // Se sua matriz for diferente, ajuste os elementos correspondentes
    float angle = atan2(matrix.b, matrix.a);

    // Convertendo o ângulo para o intervalo de 0 a 2*PI (opcional)
    // Isso garante que o ângulo esteja no mesmo intervalo de 0 a 2*PI
    if (angle < 0) {
        angle += 2 * M_PI; // Adiciona 2*PI se o ângulo for negativo para colocá-lo no intervalo correto
    }

    return angle;
}
*/

void Matrix2D::Concat(const Matrix2D &m)
{
    float a1 = this->a * m.a + this->b * m.c;
    this->b = this->a * m.b + this->b * m.d;
    this->a = a1;

    float c1 = this->c * m.a + this->d * m.c;
    this->d = this->c * m.b + this->d * m.d;

    this->c = c1;

    float tx1 = this->tx * m.a + this->ty * m.c + m.tx;
    this->ty = this->tx * m.b + this->ty * m.d + m.ty;
    this->tx = tx1;
}

AABB Matrix2D::Transform(const AABB &aabb)
{
    AABB result;

    float x1 = aabb.x1;
    float y1 = aabb.y1;
    float x2 = aabb.x2;
    float y2 = aabb.y2;

    float x1r = this->a * x1 + this->c * y1 + this->tx;
    float y1r = this->d * y1 + this->b * x1 + this->ty;
    float x2r = this->a * x2 + this->c * y2 + this->tx;
    float y2r = this->d * y2 + this->b * x2 + this->ty;

    result.x1 = std::min(x1r, x2r);
    result.y1 = std::min(y1r, y2r);
    result.x2 = std::max(x1r, x2r) ;
    result.y2 = std::max(y1r, y2r) ;

    return result;

}

Vector2 Matrix2D::TransformCoords(const Vector2 &point)
{

    Vector2 v;

    v.x = this->a * point.x + this->c * point.y + this->tx;
    v.y = this->d * point.y + this->b * point.x + this->ty;

    return v;
}

Vector2 Matrix2D::TransformCoords(float x, float y)
{
    Vector2 v;

    v.x = this->a * x + this->c * y + this->tx;
    v.y = this->d * y + this->b * x + this->ty;

    return v;
}


Matrix2D Matrix2D::Mult(const Matrix2D &m)
{
    Matrix2D result;

    result.a = this->a * m.a + this->b * m.c;
    result.b = this->a * m.b + this->b * m.d;
    result.c = this->c * m.a + this->d * m.c;
    result.d = this->c * m.b + this->d * m.d;

    result.tx = this->tx * m.a + this->ty * m.c + this->tx;
    result.ty = this->tx * m.b + this->ty * m.d + this->ty;

    return result;
}

void Matrix2D::Rotate(float angle)
{
    float acos = cos(angle);
    float asin = sin(angle);

    float a1 = this->a * acos - this->b * asin;
    this->b = this->a * asin + this->b * acos;
    this->a = a1;

    float c1 = this->c * acos - this->d * asin;
    this->d = this->c * asin + this->d * acos;
    this->c = c1;

    float tx1 = this->tx * acos - this->ty * asin;
    this->ty = this->tx * asin + this->ty * acos;
    this->tx = tx1;
}

void Matrix2D::Scale(float x, float y)
{
    this->a *= x;
    this->b *= y;

    this->c *= x;
    this->d *= y;

    this->tx *= x;
    this->ty *= y;
}

void Matrix2D::Translate(float x, float y)
{
    this->tx += x;
    this->ty += y;
}

void Matrix2D::Skew(float skewX, float skewY)
{
    float sinX = sin(skewX);
    float cosX = cos(skewX);
    float sinY = sin(skewY);
    float cosY = cos(skewY);

    Set(
        this->a * cosY - this->b * sinX,
        this->a * sinY + this->b * cosX,
        this->c * cosY - this->d * sinX,
        this->c * sinY + this->d * cosX,
        this->tx * cosY - this->ty * sinX,
        this->tx * sinY + this->ty * cosX);
}

const float *Matrix2D::getMatrix() const
{
    return m_matrix;
}

void Matrix2D::UpdateMatrix()
{
    m_matrix[0] = a;
    m_matrix[1] = b;
    m_matrix[4] = c;
    m_matrix[5] = d;
    m_matrix[12] = tx;
    m_matrix[13] = ty;
}

View::View()
{
    zoom = 1.0f;
    rotation = 0.0f;
    offset = {0, 0};
    center = {0, 0};
    
    screenHeight = GetScreenHeight();
    viewport = {0, 0, (float)GetScreenWidth(),(float) screenHeight};
    update();
}

View::~View()
{
}

View::View(const View &v)
{
    this->offset = v.offset;
    this->center = v.center;
    this->zoom = v.zoom;
    this->rotation = v.rotation;
    this->viewport.x = v.viewport.x;
    this->viewport.y = v.viewport.y;
    this->viewport.width = v.viewport.width;
    this->viewport.height = v.viewport.height;
    update();

}
View &View::operator=(const View &v)
{
    this->offset = v.offset;
    this->center = v.center;
    this->zoom = v.zoom;
    this->rotation = v.rotation;
    this->viewport.x = v.viewport.x;
    this->viewport.y = v.viewport.y;
    this->viewport.width = v.viewport.width;
    this->viewport.height = v.viewport.height;
    update();
    return *this;
}

void View::setViewPort(float x, float y ,float width, float height)
{
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
    update();
}

void View::update()
{
    camera.offset = offset;
    camera.target = center;
    camera.rotation = rotation;
    camera.zoom = zoom;
     area.width  = viewport.width  / zoom ;
     area.height = viewport.height / zoom ;
     area.x = center.x - (offset.x / zoom);
     area.y = center.y - (offset.y / zoom);





}
 


void View::begin()
{


    int     viewportTop = screenHeight - (viewport.y + viewport.height) ;
    rlViewport(viewport.x, viewportTop, viewport.width,viewport.height);
    BeginScissorMode(viewport.x, viewport.y, viewport.width,viewport.height);
    BeginMode2D(camera);
    DrawRectangleLinesEx(area, 1, RED);    


}

void View::end()
{
    EndMode2D();
    EndScissorMode();
    rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());

    
}

void View::setOffset(Vector2 offset)
{
    this->offset = offset;
    update();

}

void View::setOffset(float x, float y)
{
    this->offset.x = x;
    this->offset.y = y;
    update();
}

void View::setZoom(float zoom)
{
    this->zoom = zoom;
    update();
}
void View::setRotation(float rotation)
{
    this->rotation = rotation;
    update();
}

Rectangle View::getViewPort() 
{
    return viewport;
}

Rectangle View::getArea() 
{
    return area;
}

int View::getWidth() const
{
    return viewport.width;
}

int View::getHeight() const
{
    return viewport.height;
}

void View::folow(Vector2 v)
{
    center.x = v.x;
    center.y = v.y;
    update();

}

bool View::operator==(const View &right)
{
    return ( (this->rotation == right.rotation) && 
              (this->zoom == right.zoom) && 
              (this->offset.x == right.offset.x) && 
              (this->offset.y == right.offset.y) &&
              (this->center.x == right.center.x) && 
              (this->center.y == right.center.y) &&
              (this->viewport.x == right.viewport.x) &&
                (this->viewport.y == right.viewport.y) &&
                (this->viewport.width == right.viewport.width) &&
                (this->viewport.height == right.viewport.height)
              );

}

bool View::operator!=(const View &right)
{
    return !(*this == right);
}

