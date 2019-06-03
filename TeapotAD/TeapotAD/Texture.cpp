/*
 tdogl::Texture
 
 Copyright 2012 Thomas Dalling - http://tomdalling.com/
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "stdafx.h"

#include "Texture.h"
#include <stdexcept>



static GLenum TextureFormatForBitmapFormat(Bitmap::Format format)
{
    switch (format) {
        case Bitmap::Format_RGB: return gl::RGB;
        case Bitmap::Format_RGBA: return gl::RGBA;
        default: throw std::runtime_error("Unrecognised Bitmap::Format");
    }
}

Texture::Texture(const Bitmap& bitmap, GLint minMagFiler, GLint wrapMode) :
    _originalWidth((GLfloat)bitmap.width()),
    _originalHeight((GLfloat)bitmap.height())
{
    gl::GenTextures(1, &_object);
    gl::BindTexture(gl::TEXTURE_2D, _object);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, minMagFiler);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, minMagFiler);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, wrapMode);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, wrapMode);
    gl::TexImage2D(gl::TEXTURE_2D,
                 0, 
                 TextureFormatForBitmapFormat(bitmap.format()),
                 (GLsizei)bitmap.width(), 
                 (GLsizei)bitmap.height(),
                 0, 
                 TextureFormatForBitmapFormat(bitmap.format()), 
                 gl::UNSIGNED_BYTE, 
                 bitmap.pixelBuffer());
    gl::BindTexture(gl::TEXTURE_2D, 0);
}

Texture::Texture(std::vector<Bitmap> &bitmaps, std::vector<unsigned int> orientation, GLint minMagFiler, GLint wrapMode)
{
	gl::GenTextures(1, &_object);
	gl::BindTexture(gl::TEXTURE_CUBE_MAP, _object);

	for (GLuint i = 0; i < bitmaps.size(); i++)
	{
		gl::TexImage2D(orientation[i], 0,
			TextureFormatForBitmapFormat(bitmaps[i].format()),
			(GLsizei)bitmaps[i].width(),
			(GLsizei)bitmaps[i].height(),
			0,
			TextureFormatForBitmapFormat(bitmaps[i].format()),
			gl::UNSIGNED_BYTE,
			bitmaps[i].pixelBuffer());
	}
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, minMagFiler);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, minMagFiler);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, wrapMode);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, wrapMode);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_R, wrapMode);

	gl::BindTexture(gl::TEXTURE_2D, 0);

}

Texture::~Texture()
{
    gl::DeleteTextures(1, &_object);
}

GLuint Texture::object() const
{
    return _object;
}

GLfloat Texture::originalWidth() const
{
    return _originalWidth;
}

GLfloat Texture::originalHeight() const
{
    return _originalHeight;
}