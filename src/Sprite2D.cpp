#include "Sprite2D.hpp"
#include <raymath.h>
#include "Math.hpp"

void RenderQuad(const rQuad *quad)
{

    rlCheckRenderBatchLimit(4); // Make sure there is enough free space on the batch buffer
    rlSetTexture(quad->tex.id);

    rlBegin(RL_QUADS);

    Color a = quad->v[1].col;
    Color b = quad->v[0].col;
    Color c = quad->v[3].col;
    Color d = quad->v[2].col;

    rlNormal3f(0.0f, 0.0f, 1.0f);

    rlColor4ub(a.r, a.g, a.b, a.a);
    rlTexCoord2f(quad->v[1].tx, quad->v[1].ty);
    rlVertex3f(quad->v[1].x, quad->v[1].y, quad->v[1].z);

    rlColor4ub(b.r, b.g, b.b, b.a);
    rlTexCoord2f(quad->v[0].tx, quad->v[0].ty);
    rlVertex3f(quad->v[0].x, quad->v[0].y, quad->v[0].z);

    rlColor4ub(c.r, c.g, c.b, c.a);
    rlTexCoord2f(quad->v[3].tx, quad->v[3].ty);
    rlVertex3f(quad->v[3].x, quad->v[3].y, quad->v[3].z);

    rlColor4ub(d.r, d.g, d.b, d.a);
    rlTexCoord2f(quad->v[2].tx, quad->v[2].ty);
    rlVertex3f(quad->v[2].x, quad->v[2].y, quad->v[2].z);

    rlEnd();
}

void RenderTransform(Texture2D texture, const Matrix2D &matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    float u = 0.0f;
    float v = 0.0f;
    float u2 = 1.0f;
    float v2 = 1.0f;

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = texture.width;
    float TempY2 = texture.height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = u;
    quad.v[1].ty = v;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = u;
    quad.v[0].ty = v2;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = u2;
    quad.v[3].ty = v2;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = u2;
    quad.v[2].ty = v;

    for (int i = 0; i < 4; i++)
    {
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix.a * x + matrix.c * y + matrix.tx;
        quad.v[i].y = matrix.d * y + matrix.b * x + matrix.ty;

        // Vector3 point = {quad.v[i].x, quad.v[i].y, 0.0f};
        
        // point = Vector3Transform(point, matrix);

        // quad.v[i].x = point.x;
        // quad.v[i].y = point.y;
    }

    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = WHITE;

    RenderQuad(&quad);
}

void RenderTransformFlipClip(Texture2D texture, int width, int height, Rectangle clip, bool flipX, bool flipY, Color color, const Matrix2D &matrix, int blend)
{

    rQuad quad;
    quad.tex = texture;
    quad.blend = blend;

    int widthTex = texture.width;
    int heightTex = texture.height;

    float left;
    float right;
    float top;
    float bottom;

    if (FIX_ARTIFACTS_BY_STRECHING_TEXEL)
    {
        left = (2 * clip.x + 1) / (2 * widthTex);
        right = left + (clip.width * 2 - 2) / (2 * widthTex);
        top = (2 * clip.y + 1) / (2 * heightTex);
        bottom = top + (clip.height * 2 - 2) / (2 * heightTex);
    }
    else
    {
        left = clip.x / widthTex;
        right = (clip.x + clip.width) / widthTex;
        top = clip.y / heightTex;
        bottom = (clip.y + clip.height) / heightTex;
    }

    if (flipX)
    {
        float tmp = left;
        left = right;
        right = tmp;
    }

    if (flipY)
    {
        float tmp = top;
        top = bottom;
        bottom = tmp;
    }

    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    quad.v[1].x = TempX1;
    quad.v[1].y = TempY1;
    quad.v[1].tx = left;
    quad.v[1].ty = top;

    quad.v[0].x = TempX1;
    quad.v[0].y = TempY2;
    quad.v[0].tx = left;
    quad.v[0].ty = bottom;

    quad.v[3].x = TempX2;
    quad.v[3].y = TempY2;
    quad.v[3].tx = right;
    quad.v[3].ty = bottom;

    quad.v[2].x = TempX2;
    quad.v[2].y = TempY1;
    quad.v[2].tx = right;
    quad.v[2].ty = top;

    
    for (int i = 0; i < 4; i++)
    {
        // Vector3 point = {quad.v[i].x, quad.v[i].y, 0.0f};
        
        // point = Vector3Transform(point, matrix);

        // quad.v[i].x = point.x;
        // quad.v[i].y = point.y;
        float x = quad.v[i].x;
        float y = quad.v[i].y;
        quad.v[i].x = matrix.a * x + matrix.c * y + matrix.tx;
        quad.v[i].y = matrix.d * y + matrix.b * x + matrix.ty;
    }


    quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.0f;
    quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color;

    RenderQuad(&quad);
}


Sprite2D::Sprite2D(): Node2D()
{
    this->graph = nullptr;
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    type = SPRITE2D;
}

Sprite2D::Sprite2D(const std::string &name): Node2D(name)
{
    this->graph = nullptr;
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    type = SPRITE2D;
}

Sprite2D::Sprite2D(const std::string &name, const std::string &graphName): Node2D(name)
{
    this->graph = Assets::Instance().getGraph(graphName);
    if (this->graph == nullptr)
    {
        clip.width  = graph->width;
        clip.height = graph->height;
    }
    this->clip = {0.0f, 0.0f, 0.0f, 0.0f};
    this->color = WHITE;
    this->flipX = false;
    this->flipY = false;
    type = SPRITE2D;
}

Sprite2D::~Sprite2D()
{
}


void Sprite2D::OnDraw()
{
    Node2D::OnDraw();
    if (graph != nullptr)
    {
        if (clip.width == 0.0f && clip.height == 0.0f)
        {
            clip.width = graph->width;
            clip.height = graph->height;
        }
      //  RenderMatrixFlipClip(graph->texture, clip.width, clip.height, clip, flipX, flipY, color, getWorldTransform(), BLEND_ALPHA);
        RenderTransformFlipClip(graph->texture, clip.width, clip.height, clip, flipX, flipY, color, getWorldTransform(), BLEND_ALPHA);
    }
}
