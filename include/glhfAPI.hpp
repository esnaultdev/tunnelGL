#ifndef _GLHFAPI
#define _GLHFAPI

#include <GL/glew.h>  // must be included before gl.h
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "utils/utils.h"
#include "utils/shaders.h"
#include "utils/textures.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


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
			GLObject(void) {
				mode = GL_TRIANGLES;
				_wasInit = false;
			}

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
				_wasInit = false;
			}

			void setMode(GLenum mode) {
				this->mode = mode;
			}

			void setProgram(glhf::Program prog) {
				this->prog = prog;
			}

			void setAttributes(int vertexCount, int indexCount, std::vector<unsigned int> indices, std::vector<float> position, std::vector<float> color, std::vector<float> normal, std::vector<float> uv){
				this->vertexCount = vertexCount;
				this->indexCount = indexCount;
				this->indices = indices;
				this->color = color;
				this->position = position;
				this->normal = normal;
				this->uv = uv;
			}

			void reloadBuffers(){
				if (_wasInit) {
					glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
					glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				
					glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
					glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float), normal.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
					glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(float), color.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);    
					
					glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
					glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float), uv.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);    

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}

			void initVao(){
				_wasInit = true;
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
				GLint uvIndex = glGetAttribLocation(prog.getId(), "uv");
				glEnableVertexAttribArray(uvIndex);
				glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

				glBindVertexArray(0);          
			}

			void draw(){
				glUseProgram(prog.getId());
				glBindVertexArray(vao);
				if (textureId) 
					glBindTexture(GL_TEXTURE_2D, textureId);
				glDrawElements(mode, indexCount, GL_UNSIGNED_INT, 0);
				if (textureId) 
					glDisable(GL_TEXTURE_2D);
				glBindVertexArray(0);
				glUseProgram(0);
			}

			void setTexture(const std::string& imagepath, GLenum wrap_s=GL_REPEAT, GLenum wrap_t=GL_REPEAT, GLenum mag_filter=GL_LINEAR, GLenum min_filter=GL_LINEAR_MIPMAP_LINEAR, bool anisotropy=false) {
				textureId = loadTGATexture(imagepath, wrap_s, wrap_t, mag_filter, min_filter, anisotropy);  
			}

			void load(std::string filename){
  				Assimp::Importer importer;
				
				const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

				if( !scene) {
					std::cout << "Failed to load object: " << importer.GetErrorString() << std::endl;
				}
 
				aiMesh *mesh = scene->mMeshes[0];

				std::vector<unsigned int> indices; 	
				std::vector<float> position;
				std::vector<float> color;
				std::vector<float> normal;
				std::vector<float> uv;

				for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
					const aiFace& face = mesh->mFaces[i];
				 
					for (int j = 0; j < 3; j++) {
						//std::cout << "(i, j) " << i << ", " << j << std::endl;

						if (mesh->HasTextureCoords(0)){
							aiVector3D uvMesh = mesh->mTextureCoords[0][face.mIndices[j]];
							uv.push_back(uvMesh[0]);
							uv.push_back(uvMesh[1]);
						} else {
							uv.push_back(0);
							uv.push_back(0);
						}
						 
						aiVector3D normalMesh = mesh->mNormals[face.mIndices[j]];
						normal.push_back(normalMesh[0]);
						normal.push_back(normalMesh[1]);
						normal.push_back(normalMesh[2]);
						 
						aiVector3D posMesh = mesh->mVertices[face.mIndices[j]];
						//std::cout << "posMesh: " << posMesh[0] << " " << posMesh[1] << " " << posMesh[2] << std::endl;
						position.push_back(posMesh[0]);
						position.push_back(posMesh[1]);
						position.push_back(posMesh[2]+2);

						color.push_back(1);
						color.push_back(1);
						color.push_back(1);

						indices.push_back(i * 3 + j);
					}
				}
				std::cout << "Obj loaded: " << filename << " (" << position.size() / 3 << " vertices, " << indices.size() << " indices)" << std::endl;
				setAttributes(position.size() / 3, indices.size(), indices, position, color, normal, uv);
			}

			void scale(float factor) {
				for (int i = 0; i < position.size(); i++) {
					position.at(i) = position.at(i) * factor;
				}

				if (_wasInit) {
					glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
					glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}


		private:
			GLuint textureId = 0;
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

			bool _wasInit;
	};

}

#endif
