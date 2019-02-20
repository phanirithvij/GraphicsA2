#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include "texture.hpp"

#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "common/stb_image.h"
// #include "SDL2/SDL_test_images.h"
// #include "SDL2/SDL_image.h"
// #include "SDL2_image-2.0.4/SDL_image.h"

Texture::Texture(const std::string& path){

	// SDL_Surface* Surface = IMG_Load(path.c_str());
	
	std::cout << "SHIT RAN on " << path << std::endl;

	stbi_set_flip_vertically_on_load(1);
	std::cout << "SHIT2 RAN on " << path << std::endl;

	local_buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	std::cout << "SHIT3 RAN on " << path << std::endl;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	std::cout << "SHIT4 RAN on " << path << std::endl;
 
	// int Mode = GL_RGBA;
	// std::cout << "SHIT5 RAN on " << path << std::endl;

	// if(Surface->format->BytesPerPixel == 4) {
	// 	std::cout << "Bhai 4 hain" << std::endl;
	// 	Mode = GL_RGBA;
	// }
	std::cout << "SHIT6 RAN on " << path << std::endl;
 
	// glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	std::cout << "SHIT7 RAN on " << path << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
	std::cout << "SHIT8 RAN on " << path << std::endl;

	// glBindTexture(GL_TEXTURE_2D, 0);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// // ... which requires mipmaps. Generate them automatically.
	// glGenerateMipmap(GL_TEXTURE_2D);
	if(local_buffer){
		std::cout << "Freeing shit" << std::endl;
		stbi_image_free(local_buffer);
	}
}

void Texture::Bind(int slot){
	std::cout << "BINDING SHIT on " << TextureID << std::endl;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, TextureID);
}

void Texture::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture(){
	glDeleteTextures(1, &TextureID);
}

GLuint loadBMP_custom(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath);
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file 1\n");
		fclose(file);
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file 2\n");
		fclose(file);
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file 3\n");    fclose(file); return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file 4\n");    fclose(file); return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file can be closed.
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library,
// or do it yourself (just like loadBMP_custom and loadDDS)
//GLuint loadTGA_glfw(const char * imagepath){
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// "Bind" the newly created texture : all future texture functions will modify this texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	// Read the file, call glTexImage2D with the right parameters
//	glfwLoadTexture2D(imagepath, 0);
//
//	// Nice trilinear filtering.
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Return the ID of the texture we just created
//	return textureID;
//}



#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

	// std::cout << "Reached here 129" << std::endl;

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}
  
	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		std::cout << "wrong format" << std::endl;
		fclose(fp);
		return 0;
	}
	
	// std::cout << "Reached here 151" << std::endl;

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

	// std::cout << "Reached here 162, fourCC = " << fourCC << "MIPMAP=" << mipMapCount << std::endl;

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	// std::cout << "Reached here 173" << std::endl; std::cout << "FOURCC_DXT5=" << FOURCC_DXT5 << std::endl;

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}
	// std::cout << "Reached here 192" << std::endl;

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// std::cout << "Reached here 198" << std::endl;

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	// std::cout << "Reached here 204" << std::endl;
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// std::cout << "Reached here 209" << std::endl;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		// std::cout << "Reached here 214 ina lopopop" << std::endl;

		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
	
		// std::cout << "Reached here 218 inalopop, size=" << size << std::endl;

		// std::cout << "Calling with" << std::endl;
		// std::cout << GL_TEXTURE_2D << ", " << level << ", " << format << ", " << width << ", " << height << ", " << 
			// 0 << ", " << size << std::endl;

		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 
			0, size, buffer + offset); //segfault
		// std::cout << "Value " << value << std::endl;

		// std::cout << "Reached here 228 in aloop" << std::endl;

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer);

	return textureID;

}