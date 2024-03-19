#include <iostream>
#include "PathTracer.h"
#include "stb_image_write.h"


void PathTracer::Render(int num_samples_per_pixel)
{
	std::clog << "start rendering" << std::endl;
	std::clog << "SPP = " << num_samples_per_pixel << std::endl;
	
	const int width = m_cam.GetWidth();
	const int height = m_cam.GetHeight();

	/*** NOTICE!! You have to use uint8_t array to pass in stb function  ***/
	// Because the size of color is normally 255, 8bit.
	// If you don't use this one, you will get a weird imge.
	unsigned char* pixels = new unsigned char[width * height * CHANNEL_NUM];

	int index = 0;
	//for (int j = height - 1; j >= 0; --j)
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rline: " << j << '/' << height - 1 ;
		for (int i = 0; i < width; ++i)
		{
			float r = (float)i / (float)width;
			float g = (float)j / (float)height;
			float b = 0.2f;
			int ir = int(255.99 * r);
			int ig = int(255.99 * g);
			int ib = int(255.99 * b);

			pixels[index++] = ir;
			pixels[index++] = ig;
			pixels[index++] = ib;
		}
	}

	// if CHANNEL_NUM is 4, you can use alpha channel in png
	stbi_write_png("image/stbpng.png", width, height, CHANNEL_NUM, pixels, width * CHANNEL_NUM);

	// You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
	stbi_write_jpg("image/stbjpg3.jpg", width, height, 3, pixels, 100);
	delete[] pixels;

}
