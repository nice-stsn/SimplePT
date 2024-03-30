#pragma once

#include <string>
#include <memory>
#include "Vector.h"

class Texture
{
public:
    Texture(const std::string& texturefilename);
    ~Texture();
    Vector3 MapKdAtUV(double u, double v) const;

private:
    unsigned char* m_texture_buffer = nullptr; 
    int m_width = 0;
    int m_height = 0;
};
