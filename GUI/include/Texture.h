#pragma once

#include <string>

#include "GLFW/glfw3.h"

#define GL_CLAMP_TO_EDGE 0x812F

struct Pixel
{
	uint8_t R = 0;
	uint8_t G = 0;
	uint8_t B = 0;
	uint8_t A = 0;
};

class Texture
{
private:
	GLuint _TextureID = 0;
	int _Width = 0;
	int _Height = 0;

	Pixel* _Data;

public:
	Texture(std::string filename);
	Texture(int width, int height);
	Texture(Pixel* data, int width, int height);
	~Texture();

	void Render();
	void Render(int width, int height);
};

extern bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
extern GLuint LoadTextureData(Pixel* data, int width, int height);