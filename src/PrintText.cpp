#include <vector>
#include <iostream>
#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glhfAPI.hpp"
#include "PrintText.hpp"
#include "TextureText.hpp"


unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;
unsigned int Text2Dvao;

int NB_CHAR_W = 0;

void initText2D(const char * texturePath, int nbCharW){
	Text2DTextureID = loadDDS(texturePath);
	NB_CHAR_W = nbCharW;

	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	glhf::Program progText = glhf::Program("TextShader.v.glsl", "TextShader.f.glsl");
	Text2DShaderID = progText.getId();

	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "textTexture");
}

void printText2D(const char * text, int x, int y, int size){
	unsigned int length = strlen(text);

	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> uvs;

	for (unsigned int i = 0; i < length; ++i){
		glm::vec2 vertex_up_left	= glm::vec2(x + i * size, y + size);
		glm::vec2 vertex_up_right	= glm::vec2(x + i * size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
		glm::vec2 vertex_down_left	= glm::vec2(x + i * size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i] - '0';
		float uv_x = (character % NB_CHAR_W) / ((float) NB_CHAR_W);
		float uv_y = (character / NB_CHAR_W) / ((float) NB_CHAR_W);

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
        glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/((float) NB_CHAR_W), uv_y );
        glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/((float) NB_CHAR_W), (uv_y + 1.0f/((float) NB_CHAR_W)) );
        glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/((float) NB_CHAR_W)) );

        uvs.push_back(uv_up_left);
        uvs.push_back(uv_down_left);
        uvs.push_back(uv_up_right);

        uvs.push_back(uv_down_right);
        uvs.push_back(uv_up_right);
        uvs.push_back(uv_down_left);
	}

	glGenVertexArrays(1, &Text2Dvao);
	glBindVertexArray(Text2Dvao);

	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glUseProgram(Text2DShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	glUniform1i(Text2DUniformID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_BLEND);

	glBindVertexArray(0);

	glUseProgram(0);

}

void cleanupText2D(){
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	glDeleteTextures(1, &Text2DTextureID);

	glDeleteProgram(Text2DShaderID);
}
