#pragma once
#include "Core.hpp"
#include "Node.hpp"
#include "Node2D.hpp"


#define GROUP_PLAYER 1
#define GROUP_ENEMY 2
#define GROUP_PLAYER_PROJECTILE 4
#define GROUP_ENEMY_PROJECTILE 8
#define GROUP_ITEMS 16
#define GROUP_SOLID 32
#define GROUP_TILE_SOLID 64
#define GROUP_ALL 64

#define MASK_PLAYER (GROUP_ENEMY | GROUP_ENEMY_PROJECTILE | GROUP_SOLID |  GROUP_ITEMS | GROUP_TILE_SOLID)
#define MASK_ENEMY (GROUP_PLAYER | GROUP_PLAYER_PROJECTILE | GROUP_SOLID | GROUP_ITEMS | GROUP_TILE_SOLID)
#define MASK_PLAYER_PROJECTILE (GROUP_ENEMY | GROUP_SOLID)
#define MASK_ENEMY_PROJECTILE (GROUP_PLAYER | GROUP_SOLID)
#define MASK_ITEMS (GROUP_PLAYER | GROUP_ENEMY)
#define MASK_SOLID (GROUP_PLAYER | GROUP_ENEMY | GROUP_PLAYER_PROJECTILE | GROUP_ENEMY_PROJECTILE)
#define MASK_TILE_SOLID (GROUP_PLAYER | GROUP_ENEMY )

enum ShapeType
{

    SHAPE2D=0,
    CIRCLESHAPE2D,
    RECTANGLESHAPE2D,
    POLYSHAPE2D,
    LISTSHAPE2D,

};

class API_EXPORT Shape2D 
{
    public:
    Shape2D();
    virtual ~Shape2D();

    bool collide(Shape2D *shape);
    bool collide(float x,float y, Shape2D *shape);
    virtual void transform(Matrix2D *transform)=0;
    virtual void update()=0;
    
    void setMask(long mask);
    void setGroup(long group);
    void setGroups(long group, long mask);

    bool can(Shape2D *shape);

    void setVisible(bool visible);
    virtual  void render()=0;

    Vector2     offset;
    Vector2     center;
    ShapeType   type;
    Vector2     world;
    bool        show{true};
    bool        manual{false};
    long        group;
    long        mask;

};


class API_EXPORT CircleShape2D : public Shape2D
{
    public:
        friend class Node2D;
        friend class RectangleShape2D;


        CircleShape2D(float radius);
        CircleShape2D(float radius, Vector2 center);
        CircleShape2D(float radius, float x, float y);

        void render() override;
        void update() override;
        void transform(Matrix2D *transform)override;
        bool collide(Vector2 point);
        bool collide(float x, float y);
        bool collide(float x, float y, float radius);
        bool collide(CircleShape2D *circle);
        bool collide(float x, float y, float w, float h);

        bool collide(float x, float y,  CircleShape2D *circle);

        


        float   radius;
        


    

};

class API_EXPORT RectangleShape2D : public Shape2D
{
    public:
        RectangleShape2D(float width, float height);
        RectangleShape2D(float width, float height, Vector2 center);
        RectangleShape2D(float width, float height, float x, float y);
   
        void render() override;
        void update() override;
        void transform(Matrix2D *transform)override;

        bool collide(Vector2 point);
        bool collide(float x, float y);
        bool collide(RectangleShape2D *shape);
        bool collide(CircleShape2D *circle);

        bool collide(float x, float y, RectangleShape2D *shape);
        bool collide(float x, float y, CircleShape2D *circle);


        bool collide(float x, float y, float radius);
        bool collide(float x, float y, float w, float h);


        float   width;
        float   height;


    private:
        Rectangle rectangle;
 
};



class API_EXPORT PolyShape2D : public Shape2D
{
    public:
        PolyShape2D();
        PolyShape2D(std::vector<Vector2> points);
        void render() override;
        void update() override;
        void transform(Matrix2D *transform)override;

    protected:
        std::vector<Vector2> points;
        std::vector<Vector2> world;

};


class API_EXPORT ListShape2D : public Shape2D
{
    public:
        ListShape2D();
        ~ListShape2D();
        
        void render() override;
        void update() override;

        

        void transform(Matrix2D *transform)override;


        int  count();
        
        Shape2D *add(Shape2D *shape);
        
    
        
        void clear();
        
        Shape2D *get(int index);
        

    protected:
        
        std::vector<Shape2D*> shapes;

};