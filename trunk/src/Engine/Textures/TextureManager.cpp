/***************************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
***************************************************************************************************/
#include "TextureManager.h"
#include <iostream>

std::vector<GLint> OE::Textures::TextureManager::_vLoadedTextures = std::vector<GLint>();

bool OE::Textures::TextureManager::_LoadTGA(const char* path, GLuint Texture)
{
	if(glfwLoadTexture2D(path, GLFW_ORIGIN_UL_BIT))
	{
		std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
			<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;
		_vLoadedTextures.push_back(Texture);
		glBindTexture (GL_TEXTURE_2D, 0);
		return true;
	}
	glBindTexture (GL_TEXTURE_2D, 0);
	return false;
}

bool OE::Textures::TextureManager::_LoadJPG(const char* path, GLuint Texture)
{
	FILE *fp;
	unsigned int fLength, width, height;
	unsigned char* buf;
	struct jdec_private *jpegDecoder;
	unsigned char* components[3];

	fp = fopen(path, "rb");
	if(fp==NULL)
		return false;
	fseek(fp, 0, SEEK_END);
	fLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (unsigned char *)malloc(fLength + 4);
	if(buf == NULL)
		return false;
	fread(buf, fLength, 1, fp);
	fclose(fp);

	jpegDecoder = tinyjpeg_init();

	if(jpegDecoder==NULL)
	{
		free(buf);
		return false;
	}

	if(tinyjpeg_parse_header(jpegDecoder, buf, fLength)<0)
	{
		free(buf);
		return false;
	}

	tinyjpeg_get_size(jpegDecoder, &width, &height);

	if(tinyjpeg_decode(jpegDecoder, TINYJPEG_FMT_RGB24)<0)
	{
		free(buf);
		return false;
	}

	tinyjpeg_get_components(jpegDecoder, components);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, components[0]);

	/*
	int level = 0;
	while(width != 1 && height != 1)
	{
		glTexImage2D(GL_TEXTURE_2D, level, 3, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, components[0]);
		width /= 2;
		height /= 2;
		level++;
	}
	*/

	_vLoadedTextures.push_back(Texture);

	glBindTexture (GL_TEXTURE_2D, 0);
	free(buf);
	return true;
}

bool OE::Textures::TextureManager::_DoesFileExist(const char* path)
{
	FILE *fp;
	fp = fopen(path, "rb");
	if(fp == NULL)
		return false;

	fclose(fp);
	return true;
}

std::string OE::Textures::TextureManager::_GetTexturePath(const char* name)
{
	std::string tName = std::string(TEXTURE_PATH) + name;

	std::string tPath;

	//	Test extensions.
	tPath = tName + ".jpg";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	tPath = tName + ".tga";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	return std::string("");
}

GLint OE::Textures::TextureManager::_LoadTextureFromPath(const char* path)
{
	std::string tPath = std::string(path);
	std::string ext = tPath.substr(tPath.find_last_of('.'), 4);

	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);

	//	Is it a tga?
	if(ext.compare(".tga")==0)
	{
		if(_LoadTGA(path, Texture))
		{
			return Texture;
		}
	}

	//	Is it a jpeg?
	if(ext.compare(".jpg")==0)
	{
		if(_LoadJPG(path, Texture))
		{
			return Texture;
		}
	}

	//	We don't know what it is, free recently allocated texture memory.
	glBindTexture (GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &Texture);
	return -1;
}

GLint OE::Textures::TextureManager::LoadTexture(const char* name)
{
	std::string path = _GetTexturePath(name);
	GLint Texture = -1;

	if(path.length() == 0)
	{
		return Texture;
	}

	if((Texture = _LoadTextureFromPath(path.c_str()))==-1)
	{
		std::cerr << "Error: Failed to load texture \"" << name << "\"" << std::endl;
	}

	return Texture;
}

void OE::Textures::TextureManager::DeleteTexture(const GLuint index)
{
	std::clog << "Texture #" << index << " deleted" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	glDeleteTextures(1, &index);
}

// This is quite hacky and messy..

void OE::Textures::TextureManager::Dispose()
{
	for( int i = 0; i < (int)_vLoadedTextures.size(); i++ )
	{
		DeleteTexture(_vLoadedTextures[i]);
	}
}
