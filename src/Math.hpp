#pragma once
#include "Core.hpp"
#include "Utils.hpp"


class API_EXPORT AABB
{
public:
    AABB()
        : x1(0), y1(0), x2(1), y2(1), is_clean(false)
    {
    }

    AABB(float x, float y, float w, float h)
        : x1(x),y1(y), x2(w), y2(h), is_clean(false)
    {
    }

    void Set(const float _x1, const float _y1, const float _x2, const float _y2) ;
    void Set(const float x, const float y, const float r) ;
    void Init(float x, float y, float width, float height);

    void Clear() ;
    

    void render() ;
    const Rectangle getRectangle() const;

    void Encapsulate(const float x, const float y);
    void Encapsulate(const AABB &other);
    void Encapsulate(const Vector2 &point);

    
    
    bool Contains(const float x, const float y) const ;
    bool Contains(const AABB &other) const;
    bool Contains(const Vector2 &point) const;
    
    

    bool Intersect(const AABB* rect) const;
    bool Intersect(const Rectangle* rect) const;
    bool Intersect(const AABB* rect, AABB* intersection) const;
    bool Intersect(const Rectangle* rect, Rectangle* intersection) const;
    bool Intersect(const float x, const float y, const float r) const ;


    static AABB* Transform(const Vector2 &pos, const Vector2 &pivot, const Vector2 &scale, float rot, float width, float height, AABB* rect) ;


    float x1, y1;
    float x2, y2;
private:
    bool is_clean;
};

class API_EXPORT Matrix2D
{
public:
    Matrix2D();
    virtual ~Matrix2D();
    void Identity();
    void Set(float a, float b, float c, float d, float tx, float ty);
    void Transform(const Vector2 &center,const Vector2 &origin, const Vector2 &scale, float rotation);
    void Concat(const Matrix2D &m);
    AABB Transform(const AABB& aabb) ;
    Vector2 TransformCoords(const Vector2 &point);
    Vector2 TransformCoords(float x, float y);
    
    
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

Matrix2D Matrix2DMult(const Matrix2D curr, const Matrix2D m);


class API_EXPORT View 
{
public:
    View();
    virtual ~View();
    View(const View &);
    View &operator=(const View &) ;

    void setViewPort(float x, float y ,float width, float height);
    
    Rectangle getViewPort() ;
    Rectangle getArea() ;

    void update();

    void begin();
    void end();

    bool operator==(const View &right);
    bool operator!=(const View &right);

    void folow(Vector2 position);
    void setOffset(Vector2 offset);
    void setOffset(float x, float y);
    void setZoom(float zoom);
    void setRotation(float rotation);

    int getWidth() const;
    int getHeight() const;
    


private:
    friend class Scene;
    
    Rectangle viewport;
    Rectangle area;
    float zoom;
    float rotation;
    Vector2 offset;
    Vector2 center;
    Camera2D camera;
    int screenHeight;
};

