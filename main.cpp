#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <time.h>
#include "Vector.h"
#include <map>
#include <string>
#include <iostream>


struct Pixel
{
	unsigned char r, g, b;

	unsigned char& operator () (const int index)
	{
		if (index == 0)
			return r;
		if (index == 1)
			return g;
		if (index == 2)
			return b;
	}
};

struct Image
{
	std::vector<Pixel> pixels;
	int W, H;
};

Image loadImage(std::string file_name)
{
	int W, H, C;

	unsigned char* image = stbi_load(file_name.c_str(), &W, &H, &C, STBI_rgb);
	std::cout << W << ", " << H << std::endl;
	std::vector<Pixel> pixels;
	for (int i = 0; i < (W * H * 3) - 3; i+=3)
		pixels.push_back(Pixel{ image[i], image[i + 1], image[i + 2] });


	return Image{ pixels, W, H };
}

void saveImage(Image& img, std::string file_name)
{
	std::vector<unsigned char> pixels;

	for (Pixel& p : img.pixels)
	{
		pixels.push_back(p.r);
		pixels.push_back(p.g);
		pixels.push_back(p.b);
	}

	stbi_write_png(file_name.c_str(), img.W, img.H, 3, &pixels[0], 0);
}

Vector getRandomStraightLine()
{
	double rx = ((double)(rand() % 101) / 100) * 255;
	double ry = ((double)(rand() % 101) / 100) * 255;
	double rz = ((double)(rand() % 101) / 100) * 255;
	Vector droite{ rx, ry, rz };

	return droite;
}

double getPixelProjection(const Pixel& p, const Vector& droite)
{
	Vector v{ p.r, p.g, p.b };
	return dot(v, droite);
}

void addToPixelSafely(Pixel& p, Vector& v)
{
	for (int i = 0; i < 3; i++)
	{
		double temp = p(i) + v(i);

		if (temp < 0)
			p(i) = 0;
		else if (temp > 255)
			p(i) = 255;
		else
			p(i) = temp;
	}
		
}

void slicedWassersteinDistance(double lambda, Image& img1, Image& img2)
{
	std::multimap<double, unsigned int> pixels_img1; // les maps sont triées par clé par défaut
	std::multimap<double, unsigned int> pixels_img2;

	Vector droite = getRandomStraightLine();
	normalize(droite);

	for (int i = 0; i < img1.pixels.size(); i++)
	{
		double cos1 = getPixelProjection(img1.pixels[i], droite);
		double cos2 = getPixelProjection(img2.pixels[i], droite);

		pixels_img1.insert({ cos1, i });
		pixels_img2.insert({ cos2, i });
	}
	// modifier les pixels de img1
	auto it1 = pixels_img1.begin();
	auto it2 = pixels_img2.begin();

	for (; it1 != pixels_img1.end(); it1++, it2++)
	{
		double dif = it2->first - it1->first;

		Vector rgb_to_add = droite * dif * lambda;
		addToPixelSafely(img1.pixels[it1->second], rgb_to_add);
	}
}



int main() {
	srand(time(NULL));

	Image img1 = loadImage("lake.jpg");
	Image img2 = loadImage("fire.jpg");
	int nb_iteration = 10;

	for (int i = 0; i < nb_iteration; i++)
	{
		printf("Iteration %d\n", i);
		slicedWassersteinDistance(1, img1, img2);
	}

	saveImage(img1, "resultat.jpg");

	return 0;
}