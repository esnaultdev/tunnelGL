#ifndef _GLHFAPI
#define _GLHFAPI

#include <GL/glew.h>  // must be included before gl.h
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "utils/utils.h"
#include "utils/shaders.h"

namespace glhf {

	class Program {
		public:
			Program() { }

			Program(const std::string&vertexfilename, const std::string& fragmentfilename){
				programId = make_program(vertexfilename, fragmentfilename);
				link_program(programId);    
			}
			GLuint getId(void){
				return programId;
			}
		private:
			GLuint programId;
	};

	class GLObject {
		public:
			GLObject(void) { }

			GLObject(glhf::Program prog, int vertexCount, int indexCount, std::vector<unsigned int> indices, std::vector<float> position, std::vector<float> color, std::vector<float> normal, std::vector<float> uv){
				this->prog = prog;
				
				this->vertexCount = vertexCount;
				this->indexCount = indexCount;
				this->indices = indices;
				this->color = color;
				this->position = position;
				this->normal = normal;
				this->uv = uv;

				mode = GL_TRIANGLES;
			}

			void setMode(GLenum mode) {
				this->mode = mode;
			}

			bool initVao(){
				indexCount = vertexCount * 6;

				//---------GPU side version
				glGenBuffers(1, &positionBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
				glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				
				glGenBuffers(1, &normalBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), normal.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glGenBuffers(1, &colorBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
				glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);    
				
				glGenBuffers(1, &uvBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
				glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float), uv.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);    

				glGenBuffers(1, &indexBuffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				//--------- Creation and activation
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
				GLint positionIndex = glGetAttribLocation(prog.getId(), "position");
				glEnableVertexAttribArray(positionIndex);
				glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
				glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
				GLint colorIndex = glGetAttribLocation(prog.getId(), "vertexColor");
				glEnableVertexAttribArray(colorIndex);
				glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
				glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
				GLint normalIndex = glGetAttribLocation(prog.getId(), "normal");
				glEnableVertexAttribArray(normalIndex);
				glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
				glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
				GLint uvIndex = glGetAttribLocation(prog.getId(), "vertexUV");
				glEnableVertexAttribArray(uvIndex);
				glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

				////--------- Deactivation (clean OpenGl state!!)
				glBindVertexArray(0);

				//Texturing
				//textureId = loadTGATexture("zebra.tga", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, true);            
			}

			void draw(){
				glBindVertexArray(vao);
				glDrawElements(mode, indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}



		private:
			GLuint textureId;
			GLuint vao;
			int indexCount;
			int vertexCount;
			GLuint positionBuffer;
			GLuint colorBuffer;
			GLuint normalBuffer; 
			GLuint indexBuffer;
			GLuint uvBuffer;

			GLenum mode;
				
			std::vector<float> color;
			std::vector<float> position;
			std::vector<float> normal;
			std::vector<float> uv;
			std::vector<unsigned int> indices;
			glhf::Program prog;
	};

}

#endif
