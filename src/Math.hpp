#pragma once
#include "Core.hpp"
#include "Utils.hpp"


class AABB
{
public:
    AABB(float x, float y, float w, float h)
        : m_x(x), m_y(y), m_w(w), m_h(h)
    {
    }

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return m_w; }
    float GetHeight() const { return m_h; }

    bool contains(const AABB &other) const;
    bool contains(const Vector2 &point) const;
    bool intersects(const AABB &other) const;
    bool intersects(const Vector2 &circleCenter, float radius);
    bool intersects(const Rectangle &rect);
    static bool IntersectsCircle(const AABB &aabb, const Vector2 &circleCenter, float radius);
    static bool IntersectsRectangle(const AABB &aabb, const Rectangle &rect);
 



    float m_x, m_y;
    float m_w, m_h;
};

class API_EXPORT Matrix2D
{
public:
    Matrix2D();
    virtual ~Matrix2D();
    void Identity();
    void Set(float a, float b, float c, float d, float tx, float ty);
    void Concat(const Matrix2D &m);
    Vector2 TransformCoords(Vector2 point);
    Vector2 TransformCoords(float x, float y);
    Vector2 TransformCoords();
    Matrix2D Mult(const Matrix2D &m);
    void Rotate(float angle);
    void Scale(float x, float y);
    void Translate(float x, float y);
    void Skew(float skewX, float skewY);

    const float* getMatrix() const;

public:
    float a;
    float b;
    float c;
    float d;
    float tx;
    float ty;

private:
    float m_matrix[16]{1.f, 0.f, 0.f, 0.f,
                       0.f, 1.f, 0.f, 0.f,
                       0.f, 0.f, 1.f, 0.f,
                       0.f, 0.f, 0.f, 1.f};
    void UpdateMatrix();
};




class API_EXPORT View 
{
public:
    View();
    virtual ~View();
    View(const View &);
    View &operator=(const View &) ;

    void setViewPort(float x, float y ,float width, float height);
    Rectangle getViewPort() const;


    void begin();
    void end();

    bool operator==(const View &right);
    bool operator!=(const View &right);

    void folow(Vector2 position);
    void setOffset(Vector2 offset);
    void setOffset(float x, float y);


private:
    friend class Scene;
    Rectangle viewport;
    float zoom;
    float rotation;
    Vector2 offset;
    Vector2 center;
    Camera2D camera;
    int screenHeight;
};

