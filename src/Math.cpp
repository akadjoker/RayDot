#include "Math.hpp"



bool AABB::contains(const AABB &other) const
{
    float r1 = m_x + m_w;
    float r2 = other.m_x + other.m_w;
    float b1 = m_y + m_h;
    float b2 = other.m_y + other.m_h;

    if (m_x < r2 && r1 > other.m_x && m_y < b2 && b1 > other.m_y)
    {
        return true;
    }
    return false;
}

bool AABB::contains(const Vector2 &point) const
{
    if (point.x >= m_x && point.x <= m_x + m_w && point.y >= m_y && point.y <= m_y + m_h)
    {
        return true;
    }
    return false;
}

bool AABB::intersects(const AABB &other) const
{
    float r1 = m_x + m_w;
    float r2 = other.m_x + other.m_w;
    float b1 = m_y + m_h;
    float b2 = other.m_y + other.m_h;

    if (m_x < r2 && r1 > other.m_x && m_y < b2 && b1 > other.m_y)
    {
        return true;
    }
    return false;
}

bool AABB::intersects(const Vector2 &circleCenter, float radius)
{
    float deltaX = circleCenter.x - std::max(m_x, std::min(circleCenter.x, m_x + m_w));
    float deltaY = circleCenter.y - std::max(m_y, std::min(circleCenter.y, m_y + m_h));
    return (deltaX * deltaX + deltaY * deltaY) <= (radius * radius);
}
    bool AABB::intersects(const Rectangle &rect)
{
    float r1 = m_x + m_w;
    float r2 = rect.x + rect.width;
    float b1 = m_y + m_h;
    float b2 = rect.y + rect.height;

    if (m_x < r2 && r1 > rect.x && m_y < b2 && b1 > rect.y)
    {
        return true;
    }
    return false;
}



bool AABB::IntersectsCircle(const AABB &aabb, const Vector2 &circleCenter, float radius)
{
    float deltaX = circleCenter.x - std::max(aabb.m_x, std::min(circleCenter.x, aabb.m_x + aabb.m_w));
    float deltaY = circleCenter.y - std::max(aabb.m_y, std::min(circleCenter.y, aabb.m_y + aabb.m_h));
    return (deltaX * deltaX + deltaY * deltaY) <= (radius * radius);
}

bool AABB::IntersectsRectangle(const AABB &aabb, const Rectangle &rect)
{
    float r1 = aabb.m_x + aabb.m_w;
    float r2 = rect.x + rect.width;
    float b1 = aabb.m_y + aabb.m_h;
    float b2 = rect.y + rect.height;

    if (aabb.m_x < r2 && r1 > rect.x && aabb.m_y < b2 && b1 > rect.y)
    {
        return true;
    }
    return false;
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

Vector2 Matrix2D::TransformCoords(Vector2 point)
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

Vector2 Matrix2D::TransformCoords()
{

    Vector2 v;

    v.x = this->a * 0 + this->c * 0 + this->tx;
    v.y = this->d * 0 + this->b * 0 + this->ty;

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
    return *this;
}

void View::setViewPort(float x, float y ,float width, float height)
{
    viewport.x = x;
    viewport.y = y;
    viewport.width = width;
    viewport.height = height;
}

Rectangle View::getViewPort() const
{
    return viewport;
}


void View::begin()
{
    camera.offset = offset;
    camera.target = center;
    camera.rotation = rotation;
    camera.zoom = zoom;
    int     viewportTop = screenHeight - (viewport.y + viewport.height);
    rlViewport(viewport.x, viewportTop, viewport.width,viewport.height);
    BeginScissorMode(viewport.x, viewport.y, viewport.width,viewport.height);
    BeginMode2D(camera);
}

void View::end()
{
    EndMode2D();
    EndScissorMode();
}

void View::setOffset(Vector2 offset)
{
    this->offset = offset;

}

void View::setOffset(float x, float y)
{
    this->offset.x = x;
    this->offset.y = y;
}

void View::folow(Vector2 v)
{
    center.x = v.x;
    center.y = v.y;


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