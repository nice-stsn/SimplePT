#include <memory>
#include <iostream>
#include <cassert>
#include "Material/Texutre.h"
#include "MyMath.h"
#include "stb_image.h"

Texture::Texture(const std::string& texturefilename)
{
	int channels;
	m_texture_buffer = stbi_load(texturefilename.c_str(), &m_width, &m_height, &channels, 0);
	if (m_texture_buffer == nullptr)
	{
		std::clog << "Cannot load texture" << std::endl;
		assert(false);
	}

}

Texture:: ~Texture()
{
	stbi_image_free(m_texture_buffer);
	m_texture_buffer = nullptr;
}

Vector3 Texture::MapKdAtUV(double u, double v) const
{
	// u v to pixel coordinate
	// stb_image: left_buttom (0,0), right_top(width - 1, height - 1)
	int x = static_cast<int>(u * m_width);
	int y = static_cast<int>(v * m_height);

    // clamp
    x = std::max(0, std::min(x, m_width - 1));
    y = std::max(0, std::min(y, m_height - 1));
    int idx = (y * m_width + x) * 3;

	// unsigned char[0, 255] -> double[0, 1]
    double r = static_cast<double>(m_texture_buffer[idx + 0]) / 255;
    double g = static_cast<double>(m_texture_buffer[idx + 1]) / 255;
    double b = static_cast<double>(m_texture_buffer[idx + 2]) / 255;

	Vector3 map_kd_uv(r, g, b);
	SimplePT::Gamma2Linear(map_kd_uv);
	return map_kd_uv;
}
