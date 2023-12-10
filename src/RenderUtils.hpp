#pragma once

#include "Math.hpp"

void RenderTransformFlipClip(const Texture2D &texture, int width, int height,const  Rectangle &clip, bool flipX, bool flipY, const Color &color, const Matrix2D &matrix, int blend);
void RenderTransform(const Texture2D & texture, const Matrix2D &matrix, const Color &c, int blend);
void  RenderTextureRotateSizeRad(const Texture2D & texture,float x, float y,float spin, float size,  bool flipx, bool flipy, const  Color &c);
void RenderTile(const Texture2D &texture, float x, float y, float width, float height, const Rectangle &clip, bool flipx, bool flipy, const Color &c, int blend);