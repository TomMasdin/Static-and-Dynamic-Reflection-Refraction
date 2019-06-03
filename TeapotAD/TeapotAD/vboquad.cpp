#include "vboquad.h"

VBOQuad::VBOQuad()
{
	float points[] = 	{
		-1, 1, -1, -1, 1, 1, 1, -1,
	};


	gl::GenVertexArrays(1, &vaoHandle);
	gl::GenBuffers(1, &vboHandle);
	gl::BindVertexArray(vaoHandle);
	gl::BindBuffer(gl::ARRAY_BUFFER, vboHandle);
	gl::BufferData(gl::ARRAY_BUFFER, (sizeof(points) / sizeof(*points)) * sizeof(gl::FLOAT), &points, gl::STATIC_DRAW);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, 0, 0);

}

void VBOQuad::render() const
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	gl::BindVertexArray(vaoHandle);
	gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);
	gl::BindVertexArray(0);
}

void VBOQuad::bindTextureUnits(int programHandle, int textureID)
{
	GLint location = gl::GetUniformLocation(programHandle, "originalTexture");
	gl::Uniform1i(location, 0);

	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, textureID);
}
