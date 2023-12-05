#include "RayGot.hpp"
#include "Utils.hpp"
#include <raymath.h>

float skewX = 0.0f;
float skewY = 0.0f;

const int screenWidth = 800;
const int screenHeight = 450;

Scene *scene;// = Scene::Instance();



class Player : public Node2D
{
    public:
      Vector2 speed;
      float width = 32;
      float height = 32;
      float live;

    Player():Node2D("Player")
    {
       addChild(new CircleShape2D(20));

       addChild(new Sprite2D("sprite", "wabbit"));

     //  addChild(new Sprite2D("sprite2", "zazaka"));
     
       live =GetRandomValue(50, 100);  
       width  = 32;
       height = 32;
       origin = {width/2, height/2};
       
    }
    void ready()
    {
         position.x = GetRandomValue(-200, screenWidth+200);
         position.y = GetRandomValue(-200, screenHeight+200);
         speed.x = (float)GetRandomValue(-250, 250)/60.0f;
         speed.y = (float)GetRandomValue(-250, 250)/60.0f;

        Sprite2D *sprite = (Sprite2D*)getChildByName("sprite");
        sprite->color = (Color){ GetRandomValue(50, 240),
                                        GetRandomValue(80, 240),
                                        GetRandomValue(100, 240), 255 };

    }

    void update(double deltaTime)
    {
        live -= 5 * deltaTime *10;//24 fps
       // if (live < 0) done = true;
        // position.x += speed.x;
        // position.y += speed.y;

            if (((position.x + width/2) > GetScreenWidth()) ||
                ((position.x + width/2) < 0)) speed.x *= -1;
            if (((position.y + height/2) > GetScreenHeight()) ||
                ((position.y + height/2 - 40) < 0)) speed.y *= -1;

    }
  
};

class Nave: public Node2D
{
    Sprite2D *main;
    Sprite2D *left;
    Sprite2D *rigth;
    View *view;
    public:
    Nave():Node2D("Nave")
    {
      

      //  origin = {16, 16};
    }
    void ready()
    {
          main   =new Sprite2D("nave", "nave");
        left   =new Sprite2D("left", "cano");
        rigth  =new Sprite2D("right", "cano");
        
         origin = {99/2, 75/2};

        left->move(10, 10);
        rigth->move(70, 10);

        addChild(main);
        addChild(left);
        addChild(rigth);
        addChild(new CircleShape2D(40,99/2,75/2));


        position = {screenWidth/2-200, screenHeight/2};

     view = new View();
     view->setViewPort(0, 0, (float)screenWidth/2, (float)screenHeight);
     view->setOffset((screenWidth/2)/2, screenHeight/2);
     scene->addView(view);


    }
    void update(double deltaTime)
    {
        
       if (IsKeyDown(KEY_A)) rotation += 50 * deltaTime *10;
       if (IsKeyDown(KEY_D)) rotation -= 50 * deltaTime *10;  
       if (IsKeyDown(KEY_W)) advance(50 * deltaTime *10,rotation+90);    
    
       view->folow(position); 
    }
    void draw()
    {
        
      

    }
};

class Enemy : public Node2D
{
    Sprite2D *main;
    Sprite2D *left;
    Sprite2D *rigth;
    View *view;
    public:
    Enemy():Node2D("enemy")
    {
    }
    void ready()
    {
        main   =new Sprite2D("nave", "nave");
        left   =new Sprite2D("left", "cano");
        rigth  =new Sprite2D("right", "cano");
        
        main->color = RED;

        left->move(10, 10);
        rigth->move(70, 10);

        addChild(main);
        main->addChild(left);
        main->addChild(rigth);
        origin = {99/2, 75/2};
        addChild(new RectangleShape2D(99,75,origin.x,origin.y));


        position = {screenWidth/2+200, screenHeight/2};
        

         view = new View();
         view->setViewPort((float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight);
         view->setOffset((screenWidth/2)/2, screenHeight/2);
         scene->addView(view);

        //View view;
       // view.setViewPort({(float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight});
       // scene.addView(&view);
    }
    void update(double deltaTime)
    {
       if (IsKeyDown(KEY_J)) rotation += 50 * deltaTime *10;
       if (IsKeyDown(KEY_L)) rotation -= 50 * deltaTime *10; 
        if (IsKeyDown(KEY_I)) advance(50 * deltaTime *10,rotation+90);    
    
       view->folow(position); 
              
    }
    void draw()
    {
       
        
        
    }
};


void setViewPort(float x, float y, float w, float h)
{

    int     viewportTop = screenHeight - (y + h);
    rlViewport((int)x, viewportTop, (int)w, (int)h);
  
}



int main()
{
    

    InitWindow(screenWidth, screenHeight, "raygot");

    

     Assets::Instance().loadGraph("zazaka", "zazaka.png");
    Assets::Instance().loadGraph("wabbit", "wabbit_alpha.png");
    Assets::Instance().loadGraph("nave", "playerShip1_orange.png");
    Assets::Instance().loadGraph("cano", "gun02.png");

    Node *node = new Node("wabyt");

    scene = new Scene();


     scene->Add(node);    
    
    // scene->Add(new Enemy());
    // scene->Add(new Nave());
    

    // Nave *player = new Nave();
    // player->OnReady();
    // Enemy   *enemy = new Enemy();
    // enemy->OnReady();

    

    

    while (!WindowShouldClose())       
    {
     
        BeginDrawing();

        if (IsMouseButtonDown(1))
        {
            for (int i = 0; i < 100; i++)
            {
               // node->addChild(new Player());
                scene->Add(new Player());
            }
               
        }


        scene->Update();

       

       // View view;
        //view.setView({(float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight});
        //view.setView({0, 0, (float)screenWidth/2, (float)screenHeight});
        //Scene::Instance().setView(view);

        // enemy->OnUpdate(GetFrameTime());
        // player->OnUpdate(GetFrameTime());

        // setViewPort(0, 0, (float)screenWidth/2, (float)screenHeight);
        // BeginScissorMode(0, 0, (float)screenWidth/2, (float)screenHeight);
        // scene.Draw();
        // enemy->OnDraw();
        // player->OnDraw();  
        // EndScissorMode();


        // setViewPort((float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight);
        // BeginScissorMode((float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight);
        // scene.Draw();
        // enemy->OnDraw();
        // player->OnDraw();  
        // EndScissorMode();
           
        //  scene.Draw();
        //  enemy->OnDraw();
        //  player->OnDraw();  

    //    // setViewPort((float)screenWidth/2, 0, (float)screenWidth/2, (float)screenHeight);

         scene->Draw();
    //     enemy->OnDraw();
    //     player->OnDraw();      
    //     End2D();

        
        
         if (IsMouseButtonDown(0))
        {
            Node* node = scene->MousePick();
            if (node!=nullptr)
            {
                TraceLog(LOG_INFO, "node %s", node->getName().c_str());
                node->done = true;
            }
               
        }
       
      
      


        EndDrawing();
     
    }

    // delete player;
    // delete enemy;

    Assets::Instance().clear();
    scene->Clear();
    delete scene;
    CloseWindow();        


    return 0;
}