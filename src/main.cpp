#include "RayGot.hpp"
#include "Utils.hpp"
#include <raymath.h>

float skewX = 0.0f;
float skewY = 0.0f;

const int screenWidth =  25*24  *2;
const int screenHeight = 15*24  *2;


Color RandomRGBColor()
{
    Color c;
    c.r =(unsigned char) GetRandomValue(0, 255);
    c.g =(unsigned char) GetRandomValue(0, 255);
    c.b =(unsigned char) GetRandomValue(0, 255);
    c.a = 255;
    return c;
}

int sign (float x)
{
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}


class Bullet : public Node2D
{
    public:
    float angle;
    float life = 0;
    Node *solid{nullptr};

        Bullet(float x, float y, float angle):Node2D("Bullet")
        {
            setSize(16,16);
            origin = {16/2,16/2};
            position = {x,y};
            this->angle = angle;
            Sprite2D *sprite = new Sprite2D("bullet","props");
            shape = new RectangleShape2D(8, 4,5,5);
            shape->setGroups(GROUP_PLAYER_PROJECTILE, MASK_PLAYER_PROJECTILE);
            
            sprite->clip = {16,0,16,16};
            if (angle == 180)
            {
                sprite->flipX = true;
            }
            addChild(sprite);

        }

        void ready()
        {
          // solid = scene->Find("solids");
        }


        void update(double deltaTime)
        {

       

            advance(8,angle);
            life += deltaTime;
            if (life > 4.5)
            {
               done = true;
            }
        }

        void draw()
        {

        // if (Collide( solid))
        // {
        //     DrawText("Collide", 20, 250, 20, RED);
        // } else 
        // {
        //     DrawText("No Collide", 20, 250, 20, RED);
            
        // }
         //   DrawText(TextFormat("Life %f",life), position.x, position.y, 20, RED);
        }

        void collision(Node *n)
        {
             
           
            if (n->name == "solids")
            {
                 Log(LOG_INFO, "Collide %s" ,n->name.c_str());
                 done = true;

                    NodeEmitter2D* emitter = new NodeEmitter2D("emitter","particle", 20, true);
                    emitter->SetFrequency(60);
                    emitter->setSize(50,50);
                    float life = 0.8;

                    emitter->SetLife(life);
                    emitter->SetAlphaStart(255, 0);
                    emitter->SetAlphaEnd(0, life);
                
                    
                    emitter->SetSizeStart(0.5, 0);
                    emitter->SetSizeEnd(1.2f, life);
                    emitter->SetColorStart(55, 55, 55, 0);
                    emitter->SetColorEnd(255, 55, 55, life*2);
                    emitter->SetStartZone(-2, -2, 2, 2);
                    emitter->SetDirection(0, 20);
                    emitter->SetSelocityRange(0.5,2);
                    emitter->position = position;
                    scene->Add(emitter);
            }
            
            
        }
};

  
class Physics :public Node2D
{
    public:
    Vector2 velocity;
    Vector2 acceleration;
    Vector2 friction;
    Vector2 maxVelocity;
    Vector2 gravity;
    Node *solid{nullptr};
    bool onGround;
    bool smoke = false;


    Physics(const std::string &name):Node2D(name)
    {
        velocity = {0,0};
        acceleration = {0,0};
        friction = {0,0};
        maxVelocity = {0,0};
        gravity = {0,0};
        onGround = false;
        
        
    }

    void physics_update(double dt)
    {
        onGround = false;

        velocity.x += acceleration.x ;
        velocity.y += acceleration.y ;
        velocity.y += gravity.y;

   
        moveBy(velocity.x,velocity.y,solid,true);

        if (!onGround)
        {
            smoke = false;
        }

        

		checkMaxVelocity();

    }

    virtual bool moveCollideY(Node *n)
    {
        if (velocity.y * sign(gravity.y) > 0)
		{
			onGround = true;

            if (!smoke)
            {
                     NodeEmitter2D* emitter = new NodeEmitter2D("emitter","particle", 5, true);
                    emitter->SetFrequency(20);
                    emitter->setSize(85,85);
                    float life = 0.8;

                    emitter->SetLife(life);
                    emitter->SetAlphaStart(255, 0);
                    emitter->SetAlphaEnd(0, life);
                
                    
                    emitter->SetSizeStart(0.5, 0);
                    emitter->SetSizeEnd(1.2f, life);
                    emitter->SetColorStart(225, 225, 225, 0);
                    emitter->SetColorEnd(55, 55, 55, life*2);
                    emitter->SetStartZone(-4, -2, 4, 2);
                    emitter->SetDirection(0, 20);
                    emitter->SetSelocityRange(0.2,1.5);
                    emitter->position.x = position.x + 5;
                    emitter->position.y = position.y + 20;
                    scene->Add(emitter);
                    smoke = true;
            }
		}
		velocity.y = 0;

		velocity.x *= friction.x;
		if (abs(velocity.x) < 0.5) velocity.x = 0;
		
        return true;
    }
    virtual bool moveCollideX(Node *n)
    {
        velocity.x = 0;
  		velocity.y *= friction.y;
		if (abs(velocity.y) < 1) velocity.y = 0;
		
        return true;
    }
    
    void checkMaxVelocity()
    {
        if (maxVelocity.x > 0 && abs(velocity.x) > maxVelocity.x)
		{
			velocity.x = maxVelocity.x * sign(velocity.x);
		}

		if (maxVelocity.y > 0 && abs(velocity.y) > maxVelocity.y)
		{
			velocity.y = maxVelocity.y * sign(velocity.y);
		}
    }
  
};


class  Player : public Physics
{
    public:

    float kMoveSpeed = 0.8f;
	int   kJumpForce= 19;
    AnimatedSprite2D *sprite;
    Sprite2D *muzzle;
    RectangleShape2D *collider;
    bool crouch = false;
    bool shot=false;
    float coolDown = 0;
    float shootTimer = 0;
    

    Player():Physics("player")
    {
        
     

        
        
    
    }
    void ready()
    {
         solid = scene->Find("solids");
        setSize(30,35);
        origin = {20,20};
        

        maxVelocity.y = kJumpForce;
        maxVelocity.x = kMoveSpeed * 4;
        gravity.y = 1.0f;
        friction.x = 0.82f; // floor friction
        friction.y = 0.99f; // wall friction
        

        this->shape = new RectangleShape2D(20, 35,0,0);
        collider=static_cast<RectangleShape2D*>(this->shape);
    

       sprite = new AnimatedSprite2D("idle");
       sprite->addFrame("idle", "red_idle", 6, 48, 48);
       sprite->addFrame("run", "red_run", 9, 288/6, 48);
       sprite->addFrame("jump", "red_jump", 8, 48, 48);
       sprite->addFrame("crouch", "red_crouch", 8, 48, 48);
       sprite->play("idle", true);

        addChild(sprite);

        muzzle = new Sprite2D("flash","flash");
        addChild(muzzle);
        muzzle->position.x = 40;
        muzzle->position.y = 17;
        muzzle->scale = {1.2,1.2};
        muzzle->color.a = 0;


        position = {300,250};
        coolDown = 0.2f;
        


    }
    void update(double deltaTime)
    {

       acceleration.x = acceleration.y = 0;

        shootTimer += deltaTime;


       if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && shootTimer >= coolDown)
       {
              float angle = (sprite->flipX )?180:0;
              Vector2 pos={2,GetRandomValue(4,6)} ;
              pos = muzzle->getWorldPosition(pos);
              
              scene->Add(new Bullet(pos.x,pos.y,angle));
              muzzle->color=WHITE;
              shot=true;
              shootTimer = 0.0f;
              
       }

        if (shot)
        {
            muzzle->color.a -= 25;
            if (muzzle->color.a <= 25)
            {
                shot=false;
                muzzle->color.a = 0;
                
            }
        }

        // if (IsKeyDown(KEY_A)) 
        // {
        //    acceleration.x = -kMoveSpeed;
        
        // }
        
        //  if (IsKeyDown(KEY_D)) 
        //  {
        //     acceleration.x = kMoveSpeed;
        
        //  }


        //  if (IsKeyDown(KEY_W)) 
        // {
        //    acceleration.y = -kMoveSpeed;
        
        // }
        
        //  if (IsKeyDown(KEY_S)) 
        //  {
        //     acceleration.y = kMoveSpeed;
        
        //  }

        //  position.x += acceleration.x;
        //  position.y += acceleration.y;

      
        if (IsKeyDown(KEY_A)) 
        {
           acceleration.x = -kMoveSpeed;
           crouch=false;
        }
        
         if (IsKeyDown(KEY_D)) 
         {
            acceleration.x = kMoveSpeed;
            crouch=false;
         }

        if (IsKeyDown(KEY_W) && onGround) 
        {
           acceleration.y = -sign(gravity.y) * kJumpForce;
           crouch=false;
        }

        if (IsKeyDown(KEY_S) && onGround && !crouch) 
        {
            	crouch=true;
        }

        if (crouch)
        {
           if (sprite->IsStopped("crouch"))
           {
            shape->offset.y = -10;
            muzzle->position.y = 20;
           }
            
        } else 
        {
            
            shape->offset.y = 0;
             muzzle->position.y = 17;
        }

  


        this->physics_update(deltaTime );


        if (velocity.x < 0)
		{
			sprite->flipX = true; // left
            muzzle->position.x = 0;
            muzzle->flipX = true;
		}
		else if (velocity.x > 0)
		{
			sprite->flipX = false; // right
            muzzle->position.x = 40;
            muzzle->flipX = false;
		}

        if (onGround)
		{
			if (velocity.x == 0)
			{
				
                if (crouch)
                {
                        sprite->play("crouch",false);
                        
                                           
                } else 
                {
                    sprite->play("idle", true);
                }
			}
			else
			{
				sprite->play("run", true);
			}
		}
		else
		{
			sprite->play("jump", false);
		}

        

    }
    void draw()
    {
     
      //   moveBy(velocity.x, velocity.y,solid,true);

        // if (shape->collide(position.x,position.y, solid->shape))
        // {
        //     DrawText("Collide", 20, 250, 20, RED);
        // } else 
        // {
        //     DrawText("No Collide", 20, 250, 20, RED);
        // }
        // if (Collide(position.x,position.y, solid))
        // {
        //     DrawText("Collide", 20, 250, 20, RED);
        // } else 
        // {
        //     DrawText("No Collide", 20, 250, 20, RED);
        // }
        

    //      moveBy(acceleration.x, acceleration.y,solid,true);


          DrawText(TextFormat("Groudn %d ",onGround), 20, 200, 20, RED);

          
    }

};




class MainState : public State 
{
    public:
    Scene *scene;
 
 //   TileMap *map;   
    Vector2 scroll; 
    Player *player;

    MainState():State("MainState")
    {
       
    }
    void ready()
    {
  
         scene = new Scene();    
         Game::Instance().setClearColor({0,0,45,255});


        scene->ImportTileMap("assets/shooter.tmx", "solids");

        TileMap *map = static_cast<TileMap*>(scene->Find("layer"));
        map->SetColor({155,155,155,255});
    




        player = new Player();

        scene->Add(player);

        AnimatedSprite2D *sprite = new AnimatedSprite2D("burn");
        sprite->addFrame("burn", "burn", 8, 64/2, 64);
        sprite->play("burn", true);
        sprite->position = {337,395};
        scene->Add(sprite);

        
        View* view = getDefaultView();
        float size =10;
       // view->setViewPort(size, size, getWidth()-size, getHeight()-size);
        view->setOffset(view->getWidth()/2, getHeight()/2);
        view->setZoom(2.0f);

        
        

    }
    void update()
    {
        scene->Update();
      

    


        
    }
    void render()
    {
        View* view = getDefaultView();
 

        scroll = Vector2Lerp(scroll, player->position, 0.1f);
        if (scroll.x >= 418)
            scroll.x = 418;
        if (scroll.y >= 300)
            scroll.y = 300;
        if (scroll.x <= 300)
            scroll.x = 300;
        if (scroll.y <= 185)
            scroll.y = 185;


        
       view->folow(scroll); 


        view->begin();
        scene->Render(view);   
        scene->Collisions();
 
        view->end();
        //DrawText(TextFormat(" %f %f",scroll.x,scroll.y), getWidth()/2, 20, 20, RED);



 
     
     
   // int tile = map->GetWorldTile(GetMouseX(), GetMouseY());  
   // / DrawText(TextFormat("Objects: %i/%d", scene->Count(),scene->RenderedObjects()),20,20,18, LIME);
   // DrawText(TextFormat("Tile: %i", tile),20,40,18, LIME);
        
    }
    void destroy()
    {
        delete scene;
    }

};




int main()
{
    Game &game = Game::Instance();
    game.Init(screenWidth, screenHeight, "RayGot");

    Assets::Instance().loadGraph("particle", "fire.png");
    Assets::Instance().loadGraph("tile", "tiles.png");
    Assets::Instance().loadGraph("props", "props_16x16.png");
    Assets::Instance().loadGraph("bullet", "SpongeBullet.png");
    Assets::Instance().loadGraph("flash", "MuzzleFlash.png");
    Assets::Instance().loadGraph("burn", "burn.png");


    Assets::Instance().loadGraph("red_idle", "Gunner_Red_Idle.png");
    Assets::Instance().loadGraph("red_run", "Gunner_Red_Run.png");
    Assets::Instance().loadGraph("red_jump", "Gunner_Red_Jump.png");
    Assets::Instance().loadGraph("red_crouch", "Gunner_Red_Crouch.png");



    game.AddState(new MainState());
    game.ChangeState("MainState");
    while (game.IsRunning())
    {
        game.Update();
        game.Render();
    }
    game.Quit();


}