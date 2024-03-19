#include <iostream>
#include "PathTracer.h"
#include "Color.h"
#include "stb_image_write.h"

inline bool PathTracer::WritePixelColor(unsigned int x_id, unsigned int y_id, const Color3& col)
{
	if (m_frame_buffer == nullptr)
	{
		std::clog << "frambuffer is nullptr" << std::endl;
		return false;
	}

	unsigned pixel_index = m_cam.GetWidth() * y_id + x_id;
	m_frame_buffer[pixel_index * 3 + 0] = col.R();
	m_frame_buffer[pixel_index * 3 + 1] = col.G();
	m_frame_buffer[pixel_index * 3 + 2] = col.B();

	return true;
}

void PathTracer::Render(int num_samples_per_pixel)
{
	std::clog << "start rendering" << std::endl;
	std::clog << "SPP = " << num_samples_per_pixel << std::endl;
	
	const int width = m_cam.GetWidth();
	const int height = m_cam.GetHeight();

	int index = 0;
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rline: " << j << '/' << height - 1 ;
		for (int i = 0; i < width; ++i)
		{
			float r = (float)i / (float)width;
			float g = (float)j / (float)height;
			float b = 0.2f;
			
			Color3 color(r, g, b);

			/* ray intersection: TODO */

			/* shading: TODO */

			WritePixelColor(i, j, color);
		}
	}

	// if CHANNEL_NUM is 4, you can use alpha channel in png
	stbi_write_png("image/stbpng.png", width, height, CHANNEL_NUM, m_frame_buffer.get(), width * CHANNEL_NUM);

	//// You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
	//stbi_write_jpg("image/stbjpg3.jpg", width, height, 3, m_frame_buffer.get(), 100);
}
