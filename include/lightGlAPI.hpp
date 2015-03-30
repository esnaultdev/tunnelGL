#ifndef __LIGHTGLAPI_HPP__
#     define __LIGHTGLAPI_HPP__
/** @file lightGlAPI.hpp 
 *  @brief A thin object oriented layer on top of OpenGL.
 *
 * This file implements a thin object oriented layer on top of OpenGL.{{{
 * 
 *
 *  !fixme: introduce Sampler Objects to centralize texture sampling parameters
 *  (eg GL_TEXTURE_MIN_FILTER, ...)
 *
 *
 *  @fixme Make a cpp to implement non inline functions (and move the static
 *  Program::attributes there)
 * }}}
 *
 */
/* Headers {{{*/
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>
#include <iostream>
#include <sstream>   
#include <limits>
#include <boost/any.hpp>

// glm: matrix and vectors
// to declare before including glm.hpp, to use the swizzle operators
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

// assimp stuff
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// my own utils
#include "utils/utils.h"
#include "utils/shaders.h"
#include "utils/vao.h"
#include "utils/textures.h"
#include "utils/objloader.h"
#include "utils/vboindexer.h"
#include "utils/tangentspace.h"
/*}}}*/

/* forward declarations {{{ */
namespace ensi { namespace gl { class Vertex; class Program; class Mesh; class Scene; class Renderer; class GLRenderer;} }
void displayMat4(const glm::mat4& m);
std::ostream& operator<<(std::ostream& out, const ensi::gl::Vertex& v);
std::ostream& operator<<(std::ostream& out, const ensi::gl::Program& p);
std::ostream& operator<<(std::ostream& out, const ensi::gl::Mesh& m);/*}}}*/

namespace ensi
{
    namespace gl
    {
        using ::operator<<;
        
        /** @class Vertex {{{
         *  @brief Class for vertex data
         *
         * This class stores all the common data expected at a Vertex: position,
         * normal , tangent, uv, color. 
         * 
         */
        class Vertex
        {
        public:
            Vertex(const glm::vec3& p=glm::vec3(), const glm::vec3& n=glm::vec3(), const glm::vec2& u=glm::vec2(), const glm::vec4& c=glm::vec4(1,1,1,1), const glm::vec3& t=glm::vec3(0,0,0)):
            position(p),
            normal(n),
            tangent(t),
            uv(u),
            color(c) { }

            virtual ~Vertex(){}

        public://I know it's bad!
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec2 uv;
            glm::vec4 color;
        };/*}}}*/

        /** @class Program {{{
         *  @brief  generic GLSL shader program class
         * 
         * Allows to load a GLSL program, compile it and link it. It also tackles,
         * setting uniforms based on the type of the input (int, float, glm
         * vectors, matrices, ...). 
         * Beware automatic cast: setUniform(0) -> 0 is an int or unsigned int?
         *
         *  @note: 
         * Attribute locations are fixed (thanks to glBindAttribLocation)
         * Don't forget to relink the program after fixing the locations.
         */
        class Program
        {
        public:
            Program(const std::string& vname="", const std::string& fname="", const std::string& progname=""):myname(progname){/*{{{*/
                if(vname=="" or fname=="")
                    return;
                progid = make_program(vname.c_str(), fname.c_str());
                for(auto& entry : attributes)
                {
                    const std::string& name=entry.first;
                    GLint& index=entry.second;
                    //!note: glBindAttribLocation forces to associate the location index with the
                    //!      vertex GLSL attribute called 'name'
                    glBindAttribLocation(progid, index, name.c_str());
                }
                // Do not forget to link the program after binding the attributes:
                // http://www.opengl.org/sdk/docs/man/xhtml/glBindAttribLocation.xml
                link_program(progid);
                retrieveUniformNames();
            }/*}}}*/

            Program& operator=(const Program& other)/*{{{*/
            {
                if(this!=&other)
                {
                    uniforms=other.uniforms;
                    myname=other.myname;
                    progid=other.progid;
                    queried_uniforms=other.queried_uniforms;
                }
                return *this;
            }/*}}}*/

            // glUniform Wrappers/*{{{*/
            void setUniform(const std::string& name, unsigned int val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform1i(location, val);
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, int val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform1i(location, val);
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<" uniform was queried but does not exist"<<name<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, float val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform1f(location, val);
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, double val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform1f(location, val);
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, const glm::vec3& val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform3fv(location, 1, glm::value_ptr(val));
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, const glm::vec4& val) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniform4fv(location, 1, glm::value_ptr(val));
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, const glm::mat3& val, bool transpose=false) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(val));
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, const glm::mat4& val, bool transpose=false) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(val));
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;
            }/*}}}*/

            void setUniform(const std::string& name, const boost::any& val) const/*{{{*/
            {
                try
                {
                    setUniform(name, boost::any_cast<unsigned int>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<float>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<double>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<glm::vec3>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<glm::vec4>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<glm::mat3>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                try
                {
                    setUniform(name, boost::any_cast<glm::mat4>(val));
                    return;
                }
                catch(const boost::bad_any_cast &)
                {
                }
                std::cerr << "Something went wrong with uniform: " << name << std::endl;
            }/*}}}*/

            void setUniformSampler(const std::string& name, GLuint textureId, int unit, GLenum target=GL_TEXTURE_2D) const/*{{{*/
            {
                if(hasUniform(name))
                {
                    GLint location=uniforms.at(name);
                    glActiveTexture(GL_TEXTURE0+unit);
                    glBindTexture(target, textureId);
                    glUniform1i(location, unit);
                }
                else if(notYetQueried(name))
                    std::cout<<"====="<<name<<" uniform was queried but does not exist"<<std::endl;

            }/*}}}*/

            /*}}}*/

            static void setAttributeLocations(){/*{{{*/
                unsigned int i=0;
                for(const std::string& name : {"vertexPosition","vertexColor","vertexNormal","vertexTangent","vertexUV"})
                {
                    attributes[name]=i++;
                }
            }/*}}}*/

            virtual ~Program(){}

        /*Private methods{{{*/
        private:
            void retrieveUniformNames()/*{{{*/
            {
                int total = -1;
                glGetProgramiv( progid, GL_ACTIVE_UNIFORMS, &total ); 
                for(int i=0; i<total; ++i)  {
                    int name_len=-1, num=-1;
                    GLenum type = GL_ZERO;
                    char name[100];
                    glGetActiveUniform( progid, GLuint(i), sizeof(name)-1,
                                        &name_len, &num, &type, name );
                    name[name_len] = 0;
                    addUniformName(name);
                }
            }/*}}}*/

            void addUniformName(const std::string& name)/*{{{*/
            {
                    GLint location=glGetUniformLocation(progid, name.c_str());
                    if(location>=0)
                        uniforms[name]=location;
                    else
                        std::cout<<"====="<<name<<" could not be added to program "<<myname<<std::endl;
            }/*}}}*/

            bool hasUniform(const std::string& name) const/*{{{*/
            {
                return uniforms.find(name)!=uniforms.end();
            }/*}}}*/

            bool notYetQueried(const std::string& name) const/*{{{*/
            {
                bool wasQueried= queried_uniforms.find(name)!=queried_uniforms.end();
                if(not wasQueried)
                    queried_uniforms.insert(name);
                return not wasQueried;
            }/*}}}*/
        /*}}}*/

        // Public members/*{{{*/
        public:
            // program OpenGL location
            GLuint progid;  
            // a name for debug purpose
            std::string myname;      
            // Vertex attribute locations (static and imposed thanks to  glBindAttribLocation
            static std::map<std::string, GLint> attributes;
            // Uniforms locations
            std::map<std::string, GLint> uniforms;
            // Non-existing but queried uniforms
            mutable std::set<std::string> queried_uniforms;/*}}}*/
        };
        std::map<std::string, GLint> Program::attributes;
        /*}}}*/

        /** @class GLSLMaterial {{{
         *  @brief  generic GLSL material class
         * 
         * Allows to store values for various types of uniforms, including{{{
         * texture samplers. Also encapsulates a Program.
         *
         *  @note:
         * Heterogeneous uniform types are handled by using boost::any.
         * Beware automatic cast (0 -> int or unsigned int?)
         * }}}
         */
        class GLSLMaterial{
        public:
            GLSLMaterial(){}
            virtual ~GLSLMaterial(){}

            void applyToProgram() const{
                for(auto& entry : uniforms)
                {
                    const std::string& name=entry.first;
                    const boost::any& value=entry.second;
                    program.setUniform(name, value);
                }
                int unit=0;
                for(auto& entry : uniformSamplers)
                {
                    const std::string& name=entry.first;
                    float value=entry.second;
                    GLenum target=GL_TEXTURE_2D;
                    if(samplerTargets.find(name)!=samplerTargets.end())
                        target=samplerTargets.at(name);
                    program.setUniformSampler(name, value, unit, target);
                    unit++;
                }
            }

        public:
            std::map<std::string, boost::any> uniforms;
            std::map<std::string, GLuint> uniformSamplers;
            std::map<std::string, GLenum> samplerTargets;
            Program program;
        };/*}}}*/

        /** @class Sphere {{{
         *  @brief: Sphere descritpion
         *
         *  This class is only useful for Picking, in order to accelerate
         *  computations.
         */
        class Sphere
        {
        public:
            explicit Sphere(const glm::vec3& center=glm::vec3(), float r=-1):center(center),r(r)
            {}

            virtual ~Sphere(){}

        public:
            glm::vec3 center;
            float r;
        };
        /*}}}*/

        /** @class Ray {{{
         *  @brief Simple class to model Rays
         */
        class Ray
        {
        public:
            Ray (const glm::vec4& orig, const glm::vec3& dir, float tMin=0):orig(orig), dir(dir), tMin(tMin){}
            Ray (const glm::vec3& orig, const glm::vec3& dir, float tMin=0):orig(orig,1), dir(dir), tMin(tMin){}

            virtual ~Ray(){}
        
            void changeFrame(const glm::mat4& matrix)/*{{{*/
            {
                /*!todo: Picking Lab Exercise 1: Compute the ray in the new coordinate space {{{
                 * */
                /*}}}*/
            }/*}}}*/
        public:
            glm::vec4 orig;
            glm::vec3 dir;
            float tMin;
        };
        /*}}}*/

        /** @class Camera {{{
         *  @brief Camera control
         *
         */
        class Camera
        {
        public:
            Camera ():rotateX(0),rotateY(0),rotateZ(0),fwd(0), position(0,0,0), rotationPerTick(0.2), near(0.1), far(500.)
            {}

            virtual ~Camera (){}

            glm::mat4 getRotation() const
            {
                glm::mat4 rot;
                rot=glm::rotate(rot, -rotateX, glm::vec3(0,1,0));
                rot=glm::rotate(rot, -rotateY, glm::vec3(1,0,0));
                rot=glm::rotate(rot, -rotateZ, glm::vec3(0,0,1));
                return rot;
            }

            void updateView(glm::mat4& wv) const/*{{{*/
            {
                wv=wv*getRotation();
                wv=glm::translate(wv, position);
            }/*}}}*/
        
        public:/*{{{*/
            float rotateX,
                  rotateY,
                  rotateZ,
                  fwd;
            glm::vec3 position;
            float rotationPerTick;
            float near, far;
            /*}}}*/
        };
        /*}}}*/

        /** @class Controls {{{
         *  @brief One class to rule them all.
         *  It allows to control the openGL rendering, thanks to callbacks. The{{{
         *  control allows to change viewpoint, switch between solid and
         *  wireframe rendering, ...
         *
         *  @note: The design of the coordinate transforms in the form of stack
         *  of matrices, is not always useful. One may think that it is
         *  inherited from the old time. But sometimes it comes in handy, in
         *  order to fine tune the application behaviour.}}}
         */
        class Controls
        {
        public://Methods/*{{{*/
            Controls(unsigned int nbEx): wireframe(false), controlCam(true), nbExercises(nbEx), exercise(0), rotateLY(0), rotateLZ(0), lightdirn(0., 0.8, 0.5), lightcolor(glm::vec3(0.6, 0.4, 0.4)), ambient(glm::vec3(0.4, 0.4, 0.4)), oldtime(0), time(0){/*{{{*/
                modelWorldMatrix.push(glm::mat4(1.));
                worldViewMatrix.push(glm::lookAt(glm::vec3(0,-10,5), glm::vec3(0,0,0), glm::vec3(0,0,1)));
            }/*}}}*/

            virtual ~Controls(){}

            void setTime(float t)/*{{{*/
            {
                oldtime=time;
                time=t;
            }/*}}}*/

            void updateView()/*{{{*/
            {
                glm::mat4 wv=worldViewMatrix.top();
                camera.updateView(wv);
                worldViewMatrix.push(wv);
            }/*}}}*/

            void resetView()/*{{{*/
            {
                worldViewMatrix.pop();
            }/*}}}*/

            std::string help()/*{{{*/
            {
                std::ostringstream out;
                out << "\nAvailable commands:\n"
                    << "press 'h' to print this message again.\n"
                    << "press 'l' to toggle wireframe.\n"
                    << "press 'r' to reset the light or the camera to its initial position.\n"
                    << "press 'w/s' to move forward/backward :\n"
                    << "press 'a/d' to roll camera (rotation around Z):\n"
                    << "press 'Left/Right' to pan camera (rotation around Y):\n"
                    << "press 'Up/Down' to tilt camera (rotation around X):\n"
                    << "press 'e' to switch to the next exercise:\n"
                    << "press 'E' to switch to the previous  exercise.\n"
                    << "press an integer (in 0:"<<nbExercises<<")  to switch to the corresponding  exercise.\n"
                    << "press 'q' or Esc to quit.\n"
                    ;      
                return out.str();
            }/*}}}*/

            void updateProgramUniforms(const Program& prog, const glm::mat4& instanceMW) const/*{{{*/
            {
                glm::mat4 mw=modelWorldMatrix.top();
                mw=mw*instanceMW;
                glm::vec3 lightdirEye=getLightDirEye();
                prog.setUniform("lightdirn", lightdirEye);
                prog.setUniform("lightcolor", lightcolor);
                prog.setUniform("ambient", ambient);
                prog.setUniform("time", time);
                prog.setUniform("projectionMatrix", projectionMatrix);
                prog.setUniform("modelViewMatrix", worldViewMatrix.top()*mw);
                prog.setUniform("modelWorldMatrix", mw);
                prog.setUniform("worldViewMatrix", worldViewMatrix.top());
                glm::mat3 normalMatrix= glm::inverse(glm::transpose(glm::mat3(worldViewMatrix.top()*mw)));
                prog.setUniform("normalMatrix", normalMatrix);
                prog.setUniform("exercise", exercise);
                // object specific properties (defaults)
                prog.setUniform("diffuse", glm::vec3(0.9, 0.9, 0.9));
                prog.setUniform("specular", glm::vec3(0.6, 0.6, 0.6));
                prog.setUniform("shininess", 1.);
            }/*}}}*/

            virtual void keyCallback(int key, int action, int modifiers=0)/*{{{*/
            {
                if(action==GLFW_PRESS)
                {
                    switch(key)
                    {
                    case 'H':
                        std::cout << help() << std::endl;
                        break;
                    case 'C':
                        controlCam=not controlCam;
                        break;
                    case 'L':
                        wireframe=not wireframe;
                        break;
                    case 'E':
                        if (modifiers&GLFW_MOD_SHIFT)
                            exercise-=1;
                        else
                            exercise+=1;
                        exercise=(exercise+nbExercises)%nbExercises;
                        std::cout<<"exercise: "<<exercise<<std::endl;
                        break;
                    case 'R':
                        if(controlCam)
                        {
                            camera.rotateX=camera.rotateY=camera.rotateZ=camera.fwd=0;
                            camera.position=glm::vec3(0,0,0);
                        }
                        else
                            rotateLY=rotateLZ=0;
                        break;
                    default:
                        int i=((int)key)-(int)('0');
                        if(i>=0 && static_cast<unsigned int>(i)<nbExercises)
                        {
                            exercise=i;
                            std::cout<<"exercise: "<<exercise<<std::endl;
                        }
                    }
                }
            }/*}}}*/

            void continuousKeyCallback()/*{{{*/
            {
                double delta_rotate = (time - oldtime) * camera.rotationPerTick * 2*glm::pi<float>();
                if (glfwGetKey(window, 'W') == GLFW_PRESS)
                {
                    camera.fwd += delta_rotate*4;
                    glm::vec3 movement=4*(float)delta_rotate*glm::transpose(glm::mat3(worldViewMatrix.top()))*glm::vec3(0,0,1);
                    camera.position+=movement;
                }
                if (glfwGetKey(window, 'S') == GLFW_PRESS)
                {
                    camera.fwd -= delta_rotate*4;
                    glm::vec3 movement=4*(float)delta_rotate*glm::transpose(glm::mat3(worldViewMatrix.top()))*glm::vec3(0,0,1);
                    camera.position-=movement;
                }
                if (glfwGetKey(window, 'D') == GLFW_PRESS)
                    camera.rotateX += delta_rotate;
                if (glfwGetKey(window, 'A') == GLFW_PRESS)
                    camera.rotateX -= delta_rotate;
                if(controlCam){
                    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                        camera.rotateZ += delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                        camera.rotateZ -= delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                        camera.rotateY += delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                        camera.rotateY -= delta_rotate;
                }
                else
                {
                    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
                        rotateLZ += delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
                        rotateLZ -= delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                        rotateLY += delta_rotate;
                    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                        rotateLY -= delta_rotate;
                }              
            }/*}}}*/

            virtual void mouseButtonCallback(int button, int action);

            virtual void resize(int width, int height)/*{{{*/
            {        
                // set the projection matrix
                projectionMatrix = glm::perspective(45.0f, width/(float)height, camera.near, camera.far);
                viewport=glm::vec4(0, 0, width, height);
                glViewport(0,0, width, height);
                glfwGetWindowSize(window, &width, &height);
                screenToPixel=viewport.zw()/glm::vec2(width, height);
            }/*}}}*/                                               

            glm::vec3 getLightDir() const/*{{{*/
            {
                glm::mat4 rotL=glm::rotate(glm::mat4(), -rotateLY, glm::vec3(1,0,0));
                rotL=glm::rotate(rotL, -rotateLZ, glm::vec3(0,0,1));
                glm::vec3 lightdirn=glm::normalize(glm::mat3(rotL)*this->lightdirn);
                return lightdirn;
            }/*}}}*/

            glm::vec3 getLightDirEye() const/*{{{*/
            {
                glm::vec3 lightdirn=glm::normalize(glm::mat3(worldViewMatrix.top())*getLightDir());
                return lightdirn;
            }/*}}}*/


            /*}}}*/

        public://Members/*{{{*/
            GLFWwindow* window;
            std::stack<glm::mat4> worldViewMatrix;
            std::stack<glm::mat4> modelWorldMatrix;
            glm::mat4 projectionMatrix;
            glm::vec4 viewport;
            glm::vec2 screenToPixel;
            bool wireframe;
            bool controlCam;
            unsigned int nbExercises;
            unsigned int exercise;
            float rotateLY,
                  rotateLZ;
            glm::vec3 lightdirn;
            glm::vec3 lightcolor;
            glm::vec3 ambient;
            float oldtime;
            float time;
            Camera camera;
            /*}}}*/
        };
        /*}}}*/

        /** @class Resources {{{
         *  @brief stores GPU related data
         *  Mainly used to store GPU related location data ( VAOs, IBOs)
         *
         * !fixme: rethink if this class is really necessary (could be placed in
         * the Mesh directly)
         */
        class Resources
        {
        public:
            virtual ~Resources()/*{{{*/
            {
                for(auto& entry : vaos)
                {
                    GLuint vaolocation=entry.second;
                    glDeleteBuffers(1, &vaolocation);
                    checkGLerror();
                }
                for(auto& entry : vbos)
                {
                    std::vector<GLuint>& vbolocations=entry.second;
                    glDeleteBuffers(vbolocations.size(), &vbolocations[0]);
                    checkGLerror();
                }
                for(auto& entry : ibos)
                {
                    GLuint ibolocation=entry.second;
                    glDeleteBuffers(1, &ibolocation);
                    checkGLerror();
                }
            }/*}}}*/

        public:
            std::map<std::string, GLuint> vaos;
            std::map<std::string, std::vector<GLuint> > vbos;
            std::map<std::string, GLuint> ibos;
            std::map<std::string, unsigned int> ibonbtris;
        }; /*}}}*/

        /** @class Renderer {{{
         *  @brief Abstract class for rendering
         */
        class Renderer
        {
        public:
            Renderer ();

            virtual ~Renderer (){}

            virtual void update(const std::string& objectname)=0;
            virtual void renderInstance(const std::string& instancename, const GLSLMaterial& mat=GLSLMaterial(), bool special4picked=true) const=0;
            void setPicked(const std::string& pickedname)
            {
                this->pickedname=pickedname;
            }

            const std::string& getPicked() const
            {
                return pickedname;
            }

        protected:
            const Scene& scene;
            std::string pickedname;
        private:
            
        };
        /*}}}*/

        /** @class SceneObject {{{
         *  @brief A abstract class for objects in the scene
         *
         */
        class SceneObject
        {
        public:
            virtual ~SceneObject (){}
        
            virtual SceneObject* clone() const=0;

            virtual const Mesh& asMesh() const=0;

            virtual void computeBSphere()=0;

            virtual bool intersect(const Ray& ray, float& distHit) const=0;

            bool intersectBoundingSphere(const Ray& ray, float& distHit) const/*{{{*/
            {
                /*!todo: Picking Lab Exercise 3: Implement bounding sphere reject{{{*/
                /*! Reject occurs under any of the following conditions
                 *  1 - The distance from the sphere center to its projection
                 *      P(tProj) on the ray is larger than the sphere radius
                 *  2 - P(tMin) is outside the sphere and further than P(tProj)
                 * */
                /*}}}*/
                return true;
            }
            /*}}}*/

        protected:
            Sphere bSphere;
        };/*}}}*/

        /** @class Mesh {{{
         *  @brief A mesh class
         *  It is designed to encapsulate a c++ representation, made of vertices
         *  and triangles. It also allows to setup a GPU equivalent thanks to
         *  the methods setupVAO
         *
         */
        class Mesh: public SceneObject
        {
        public:
            Mesh() {}
            
            Mesh(const Mesh& other):SceneObject(other), m_verts(other.m_verts), m_tris(other.m_tris)/*{{{*/
            {
                
            }/*}}}*/

            virtual ~Mesh(){}

            SceneObject* clone() const{/*{{{*/
                return new Mesh(*this);
            }/*}}}*/


            void setupVAO(Resources& resources, const std::string& vaoname,  GLenum usage=GL_STATIC_DRAW) const/*{{{*/
            {
                /* Make the cpu buffers{{{*/
                //!note could probably be avoided with interlaced buffers
                std::vector<glm::vec3> positions;
                std::vector<glm::vec3> normals;
                std::vector<glm::vec3> tangents;
                std::vector<glm::vec4> colors;
                std::vector<glm::vec2> uvs;
                for(const Vertex& vertex : m_verts)
                {
                    positions.push_back(vertex.position);
                    normals.push_back(vertex.normal);
                    tangents.push_back(vertex.tangent);
                    colors.push_back(vertex.color);
                    uvs.push_back(vertex.uv);
                }
                /*}}}*/

                /* Set up the VAO{{{*/
                // Bind the vao
                GLuint vao=resources.vaos.at(vaoname);
                glBindVertexArray(vao);      

                std::vector<GLuint> vbos=resources.vbos.at(vaoname);
                GLuint pbuffer = vbos[0];
                GLuint nbuffer = vbos[1]; 
                GLuint tbuffer = vbos[2]; 
                GLuint cbuffer = vbos[3]; 
                GLuint uvbuffer= vbos[4]; 

                unsigned int nbverts=m_verts.size();
                glBindBuffer(GL_ARRAY_BUFFER, pbuffer);
                glBufferData(GL_ARRAY_BUFFER, nbverts*sizeof(glm::vec3), &positions[0], usage);
                glVertexAttribPointer(
                                      Program::attributes["vertexPosition"],  /* attribute */
                                      3,                                /* size of the attribute */
                                      GL_FLOAT,                         /* type */
                                      GL_FALSE,                         /* normalized? */
                                      0,                /* stride */
                                      (void*)0                          /* array buffer offset */
                                     );
                glEnableVertexAttribArray(Program::attributes["vertexPosition"]);

                glBindBuffer(GL_ARRAY_BUFFER, nbuffer);
                glBufferData(GL_ARRAY_BUFFER, nbverts*sizeof(glm::vec3), &normals[0], usage);
                glVertexAttribPointer(
                                      Program::attributes["vertexNormal"],  /* attribute */
                                      3,                                /* size of the attribute */
                                      GL_FLOAT,                         /* type */
                                      GL_FALSE,                         /* normalized? */
                                      0,                /* stride */
                                      (void*)0                          /* array buffer offset */
                                     );
                glEnableVertexAttribArray(Program::attributes["vertexNormal"]);

                glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
                glBufferData(GL_ARRAY_BUFFER, nbverts*sizeof(glm::vec3), &tangents[0], usage);
                glVertexAttribPointer(
                                      Program::attributes["vertexTangent"],  /* attribute */
                                      3,                                /* size of the attribute */
                                      GL_FLOAT,                         /* type */
                                      GL_FALSE,                         /* normalized? */
                                      0,                /* stride */
                                      (void*)0                          /* array buffer offset */
                                     );
                glEnableVertexAttribArray(Program::attributes["vertexTangent"]);

                glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
                glBufferData(GL_ARRAY_BUFFER, nbverts*sizeof(glm::vec4), &colors[0], usage);
                glVertexAttribPointer(
                                      Program::attributes["vertexColor"],  /* attribute */
                                      4,                                /* size of the attribute */
                                      GL_FLOAT,                         /* type */
                                      GL_FALSE,                         /* normalized? */
                                      0,                /* stride */
                                      (void*)0                          /* array buffer offset */
                                     );
                glEnableVertexAttribArray(Program::attributes["vertexColor"]);

                glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
                glBufferData(GL_ARRAY_BUFFER, nbverts*sizeof(glm::vec2), &uvs[0], usage);
                glVertexAttribPointer(
                                      Program::attributes["vertexUV"],  /* attribute */
                                      2,                                /* size of the attribute */
                                      GL_FLOAT,                         /* type */
                                      GL_FALSE,                         /* normalized? */
                                      0,                /* stride */
                                      (void*)0                          /* array buffer offset */
                                     );
                glEnableVertexAttribArray(Program::attributes["vertexUV"]);

                GLuint ibo=resources.ibos.at(vaoname);
                unsigned int nbtris=m_tris.size();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbtris*sizeof(unsigned int), &m_tris[0], usage);
                resources.ibonbtris[vaoname]=nbtris;
                /*}}}*/
            }/*}}}*/

            void recomputeTangents()/*{{{*/
            {
                std::vector<glm::vec3> ps;
                std::vector<glm::vec2> uvs;
                std::vector<glm::vec3> ns;
                std::vector<glm::vec3> ts;
                std::vector<glm::vec3> bs;
                for( unsigned int k : m_tris)
                {
                    const Vertex& v=m_verts[k];
                    ps.push_back(v.position);
                    uvs.push_back(v.uv);
                    ns.push_back(v.normal);
                }
                computeTangentBasis(ps, uvs, ns, ts, bs);
                for( unsigned int k : m_tris)
                {
                    Vertex& v=m_verts[k];
                    v.tangent=ts[k];
                }
            }/*}}}*/

            void loadObj(const std::string& filepath, const glm::vec4& color=glm::vec4(1,1,1,1)){/*{{{*/
                m_verts.clear();
                m_tris.clear();
                std::vector<glm::vec3> ps;
                std::vector<glm::vec2> uvs;
                std::vector<glm::vec3> ns;
                bool res = loadOBJ(filepath.c_str(), ps, uvs, ns);
                if(not res)
                {
                    std::cout<<"Error loading file: "<<filepath<<std::endl;
                    return;
                }
                std::vector<glm::vec3> ts;
                std::vector<glm::vec3> bs;
                computeTangentBasis(ps, uvs, ns, ts, bs);
                std::vector<glm::vec3> ops, ons, ots, obs;
                std::vector<glm::vec2> ouvs;
                indexVBO_TBN(ps, uvs, ns, ts, bs, m_tris, ops, ouvs, ons, ots, obs);
                for(unsigned int i = 0; i < ops.size(); ++i)
                {
                    Vertex v(ops[i], ons[i], ouvs[i], color, ots[i]);
                    m_verts.push_back(v);
                }
                computeBSphere();
            }/*}}}*/

            void makeMeCube(const glm::vec4& color=glm::vec4(1,1,1,1))/*{{{*/
            {
                m_verts.clear();
                m_tris.clear();
                std::vector<glm::vec3> ps, ns, ts, bs;
                std::vector<glm::vec2> uvs;
                //make the front face (y=0)/*{{{*/
                glm::vec3 p0(0,0,0);
                glm::vec3 p1(1,0,0);
                glm::vec3 p2(1,0,1);
                glm::vec3 p3(0,0,1);
                glm::vec3 n0(0,-1,0);
                glm::vec3 n1(0,-1,0);
                glm::vec3 n2(0,-1,0);
                glm::vec3 n3(0,-1,0);  
                glm::vec3 pp0(0,1,0);
                glm::vec3 pp1(0,1,1);
                glm::vec3 pp2(1,1,1);
                glm::vec3 pp3(1,1,0);
                glm::vec3 nn0(0,1,0);
                glm::vec3 nn1(0,1,0);
                glm::vec3 nn2(0,1,0);
                glm::vec3 nn3(0,1,0);  /*}}}*/
                // for each axis create two faces
                int nbverts=0;
                for(int i = 0; i < 3; ++i)/*{{{*/
                {
                    int ix=i, iy=(i+1)%3, iz=(i+2)%3;
                    // first face
                    ps.push_back(glm::vec3(p0[ix]-0.5,p0[iy]-0.5,p0[iz]-0.5));
                    ns.push_back(glm::vec3(n0[ix],n0[iy],n0[iz]));
                    uvs.push_back(glm::vec2(p0[0],p0[2]));

                    ps.push_back(glm::vec3(p1[ix]-0.5,p1[iy]-0.5,p1[iz]-0.5));
                    ns.push_back(glm::vec3(n1[ix],n1[iy],n1[iz]));
                    uvs.push_back(glm::vec2(p1[0],p1[2]));

                    ps.push_back(glm::vec3(p2[ix]-0.5,p2[iy]-0.5,p2[iz]-0.5));
                    ns.push_back(glm::vec3(n2[ix],n2[iy],n2[iz]));
                    uvs.push_back(glm::vec2(p2[0],p2[2]));
                    
                    nbverts+=3;

                    ps.push_back(glm::vec3(p0[ix]-0.5,p0[iy]-0.5,p0[iz]-0.5));
                    ns.push_back(glm::vec3(n0[ix],n0[iy],n0[iz]));
                    uvs.push_back(glm::vec2(p0[0],p0[2]));

                    ps.push_back(glm::vec3(p2[ix]-0.5,p2[iy]-0.5,p2[iz]-0.5));
                    ns.push_back(glm::vec3(n2[ix],n2[iy],n2[iz]));
                    uvs.push_back(glm::vec2(p2[0],p2[2]));
                    
                    ps.push_back(glm::vec3(p3[ix]-0.5,p3[iy]-0.5,p3[iz]-0.5));
                    ns.push_back(glm::vec3(n3[ix],n3[iy],n3[iz]));
                    uvs.push_back(glm::vec2(p3[0],p3[2]));

                    nbverts+=3;

                    // second face
                    ps.push_back(glm::vec3(pp0[ix]-0.5,pp0[iy]-0.5,pp0[iz]-0.5));
                    ns.push_back(glm::vec3(nn0[ix],nn0[iy],nn0[iz]));
                    uvs.push_back(glm::vec2(pp0[0],pp0[2]));

                    ps.push_back(glm::vec3(pp1[ix]-0.5,pp1[iy]-0.5,pp1[iz]-0.5));
                    ns.push_back(glm::vec3(nn1[ix],nn1[iy],nn1[iz]));
                    uvs.push_back(glm::vec2(pp1[0],pp1[2]));

                    ps.push_back(glm::vec3(pp2[ix]-0.5,pp2[iy]-0.5,pp2[iz]-0.5));
                    ns.push_back(glm::vec3(nn2[ix],nn2[iy],nn2[iz]));
                    uvs.push_back(glm::vec2(pp2[0],pp2[2]));
                    
                    nbverts+=3;

                    ps.push_back(glm::vec3(pp0[ix]-0.5,pp0[iy]-0.5,pp0[iz]-0.5));
                    ns.push_back(glm::vec3(nn0[ix],nn0[iy],nn0[iz]));
                    uvs.push_back(glm::vec2(pp0[0],pp0[2]));

                    ps.push_back(glm::vec3(pp2[ix]-0.5,pp2[iy]-0.5,pp2[iz]-0.5));
                    ns.push_back(glm::vec3(nn2[ix],nn2[iy],nn2[iz]));
                    uvs.push_back(glm::vec2(pp2[0],pp2[2]));
                    
                    ps.push_back(glm::vec3(pp3[ix]-0.5,pp3[iy]-0.5,pp3[iz]-0.5));
                    ns.push_back(glm::vec3(nn3[ix],nn3[iy],nn3[iz]));
                    uvs.push_back(glm::vec2(pp3[0],pp3[2]));

                    nbverts+=3;
                }/*}}}*/
                computeTangentBasis(ps, uvs, ns, ts, bs);
                std::vector<glm::vec3> ops, ons, ots, obs;
                std::vector<glm::vec2> ouvs;
                indexVBO_TBN(ps, uvs, ns, ts, bs, m_tris, ops, ouvs, ons, ots, obs);
                for(unsigned int i = 0; i < ops.size(); ++i)
                {
                    Vertex v(ops[i], ons[i], ouvs[i], color, ots[i]);
                    m_verts.push_back(v);
                }
                computeBSphere();
            }/*}}}*/

            void makeMeSphere(int nbTheta=10, int nbPhi=-1, const glm::vec4& color=glm::vec4(1,1,1,1))/*{{{*/
            {
                if(nbPhi<0)
                    nbPhi=nbTheta;
                m_verts.clear();
                m_tris.clear();
                float dtheta=M_PI/nbTheta;
                float dphi=2*M_PI/nbPhi;
                for(int i = 0; i < nbTheta; ++i)
                {
                    for(int j = 0; j < nbPhi; ++j)
                    {
                        for(int u = 0; u < 2; ++u)
                        {
                            for(int v = 0; v < 2; ++v)
                            {
                                float phi=(j+u)*dphi;
                                float theta=(i+v)*dtheta;
                                glm::vec3 p(cos(phi)*sin(theta),sin(phi)*sin(theta), cos(theta));
                                glm::vec3 n=glm::normalize(p);
                                glm::vec3 t(-sin(phi),cos(phi),0);
                                glm::vec2 uv(phi/(2*M_PI),theta/M_PI);
                                Vertex vert(p,n,uv,color,t);
                                m_verts.push_back(vert);
                            }
                        }
                        // first triangle uv= (0,0), (0,1), (1,0)
                        m_tris.push_back(4*(i*nbPhi+j));
                        m_tris.push_back(4*(i*nbPhi+j)+1);
                        m_tris.push_back(4*(i*nbPhi+j)+2);
                        // second triangle uv= (1,0), (0,1), (1,1)
                        m_tris.push_back(4*(i*nbPhi+j)+2);
                        m_tris.push_back(4*(i*nbPhi+j)+1);
                        m_tris.push_back(4*(i*nbPhi+j)+3);
                    }
                }
                computeBSphere();
            }/*}}}*/

            void computeBSphere()/*{{{*/
            {
                glm::vec3 center(0,0,0);
                for( const Vertex& vertex : m_verts)
                {
                    const glm::vec3& v=vertex.position;
                    center+=v;
                }
                unsigned int nbverts=m_verts.size();
                center/=nbverts;
                float r2=0;
                for( const Vertex& vertex : m_verts)
                {
                    const glm::vec3& v=vertex.position;
                    float newR2=glm::length2(center-v);
                    if(newR2>=r2)
                        r2=newR2;
                }
                float r=std::sqrt(r2);
                bSphere= Sphere(center, r);
            }
            /*}}}*/

            bool intersect(const Ray& ray, float& distHit) const/*{{{*/
            {
                bool reached=false;
                /*!todo: Picking Lab Exercise 1: Compute the HIT or return false{{{*/
                /*! For each triangle
                 *  1 - Compute the three vertices (v0, v1, v2)
                 *  2 - Call intersectTriangle 
                 *  3 - If there is a hit, check if closest and if not too close
                 *      (tMin)
                 *  4 - Don't forget to UPDATE distHit
                 *  
                 *  Then modify the intersectTriangle method below
                 * */
                /*}}}*/
                return reached;
            }
            /*}}}*/     

            const Mesh& asMesh() const{/*{{{*/
                return *this;
            }/*}}}*/

        protected:
            bool intersectTriangle(const Ray& ray, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& distHit) const/*{{{*/
            {
                /*!todo: Picking Lab Exercise 1: Compute the hit{{{*/
                /*! You will implement the linear system in barycentric coords
                 *  (beta, gamma, t) where t is the location on the ray
                 *  1 - Create the columns of the system
                 *  (c1,c2,c3) (beta, gamma, t)^T = b
                 *  2 - Solve it using Cramer's rule
                 *
                 *  No need to solve all unknowns: return false as soon as you
                 *  can!!
                 * */
                return true;
                /*}}}*/
            }
            /*}}}*/

        public:
            std::vector<Vertex> m_verts;
            std::vector<unsigned int> m_tris;
        };/*}}}*/

        /** @class Scene {{{
         *  @brief Simple scene representation.
         *
         *  A scene is composed of a list of objects, each being labeled by a {{{
         *  name, attached to a vao, and also located in the world via a matrix.
         *  This location can be fine-tuned thanks to the stack capability. The
         *  Scene provides the capability to perform the OpenGL drawing commands
         *  thanks to the drawObject method. When fed with an object name, this
         *  method draw the object at its attached location (but this location
         *  is actually relative to what is already in the
         *  Controls::modelViewMatrix stack.
         *  
         *  Considering, light, only one single directional light is handled by
         *  default.}}}
         */
        class Scene
        {
        public:
            virtual ~Scene()/*{{{*/
            {
                delete renderer;
                for(auto& entry : objects)
                {
                    delete entry.second;
                }
            }/*}}}*/

            static Scene& getInstance(){/*{{{*/
                if(! instance)
                {
                    instance = new Scene();
                }
                return *instance;
            }/*}}}*/

            void addObject(const std::string& objectname, const SceneObject& object, const glm::mat4& mw=glm::mat4(), const std::string& instancename="")/*{{{*/
            {
                objects[objectname]=object.clone();
                addInstance(objectname, mw, instancename);
                renderer->update(objectname);
            }/*}}}*/

            void addInstance(const std::string& objectname, const glm::mat4& mw, std::string instancename)/*{{{*/
            {
                if(instancename=="")
                    instancename=objectname;
                mwMatrices[instancename]=mw;
                objectnames[instancename]=objectname;
            }/*}}}*/

            void drawObject(const std::string& instancename, const std::string& matname, bool special4picked=true) const /*{{{*/
            {
                const GLSLMaterial& mat=materials.at(matname);
                renderer->renderInstance( instancename, mat, special4picked);
            }/*}}}*/
            
            bool intersectInstance(const std::string& instancename, const Ray& ray, float& distHit) const/*{{{*/
            {
                const std::string& objectname=objectnames.at(instancename);
                const SceneObject& object=*objects.at(objectname);
                // compute the ray in Model coords
                glm::mat4 vm=glm::inverse(controls.worldViewMatrix.top()*mwMatrices.at(instancename));

                Ray rayInModel(ray);
                rayInModel.changeFrame(vm);
                bool reachable=object.intersectBoundingSphere(rayInModel, distHit);
                if(not reachable)
                    return false;
                return object.intersect(rayInModel, distHit);
                return false;
            }
            /*}}}*/

            const std::string& performPicking(int x, int y)/*{{{*/
            {
                pickedname="";
                float distNearest=std::numeric_limits<float>::max();
                /*!todo: Picking Lab Exercise 2: Compute the ray in Eye coordinate space {{{*/
                /*! 1 - get the NDC coords of (x,y) at the near plane 
                 *      Beware y is inversed use height-y-1 instead
                 *  2 - Get the cooresponding clipping coords (beware that we deal
                 *      with a vector not a point)
                 *  3 - Get the eye coords of the ray direction (raydirEye)
                 *  4 - Same for the ray origin (rayorigEye)
                 *  5 - provide a consistent value for tMin
                 *  NB: You may place the ray origin at the camera center or at
                 *      the near plane
                 * */
                //! This are the three variable to compute
                glm::vec4 rayorigEye(0,0,0,1);
                glm::vec3 raydirEye(0,0,-1);
                float tMin=0;
                /*}}}*/
                for(auto& entry : objectnames)
                {
                    float distHit=std::numeric_limits<float>::max();
                    const std::string& instancename=entry.first;
                    Ray ray(rayorigEye, raydirEye, tMin);
                    if (not intersectInstance(instancename, ray, distHit))
                        continue;
                    if(distHit<distNearest and instancename.substr(instancename.size()-2,2)!="SV")
                    {
                        distNearest=distHit;
                        pickedname=instancename;
                    }
                }
                return pickedname;
            }
            /*}}}*/

            void setRenderer(Renderer* r)/*{{{*/
            {
                renderer=r;
            }/*}}}*/

            bool containsInstance(const std::string& instancename) const /*{{{*/
            {
                return objectnames.find(instancename)!=objectnames.end();
            }/*}}}*/

            bool containsObject(const std::string& objectname) const /*{{{*/
            {
                return objects.find(objectname)!=objects.end();
            }/*}}}*/

        public:
            /*!note Assimp related: not validated yet{{{*/
            GLuint processTexture(aiTexture* t)/*{{{*/
            {                          
                //!fixme: need to use a texture loading lib like SOIL 
                return 0;
            }/*}}}*/

            GLSLMaterial processMaterial(aiMaterial* m, const std::vector<GLuint>& textures)/*{{{*/
            {
                GLSLMaterial mat;
                //!fixme should be done at some point
                return mat;
            }/*}}}*/

            std::string processMesh(aiMesh* m, const std::vector<std::string> matnames)/*{{{*/
            {
                //!fixme use the materials when ready
                std::string meshname=m->mName.C_Str();
                //!fixme: it seems that meshname is often empty
                std::cout << "Mesh name: " << meshname << std::endl;
                Mesh mesh;
                for (GLuint i = 0; i < m->mNumVertices; ++i)
                {
                    Vertex v;
                    // Process vertex attributes
                    if(m->HasPositions()) 
                    {
                        v.position.x = m->mVertices[i].x;
                        v.position.y = m->mVertices[i].y;
                        v.position.z = m->mVertices[i].z;
                    }
                    if(m->HasNormals()) 
                    {
                        v.normal.x = m->mNormals[i].x;
                        v.normal.y = m->mNormals[i].y;
                        v.normal.z = m->mNormals[i].z;
                    }
                    if(m->HasTangentsAndBitangents())
                    {
                        v.tangent.x = m->mTangents[i].x;
                        v.tangent.y = m->mTangents[i].y;
                        v.tangent.z = m->mTangents[i].z;
                    }
                    if(m->HasVertexColors(0))
                    {
                        v.color.r = m->mColors[0][i].r; 
                        v.color.g = m->mColors[0][i].g;
                        v.color.b = m->mColors[0][i].b; 
                        v.color.a = m->mColors[0][i].a;
                    }
                    if(m->HasTextureCoords(0)) // Does the mesh contain texture coordinates?
                    {
                        v.uv.x = m->mTextureCoords[0][i].x; 
                        v.uv.y = m->mTextureCoords[0][i].y;
                    }
                    // Append the vertex
                    mesh.m_verts.push_back(v);
                }
                for(GLuint i = 0; i < m->mNumFaces; i++)
                {
                    aiFace face = m->mFaces[i];
                    //normally we only get triangles thanks to aiProcess_Triangulate
                    if(face.mNumIndices!=3)
                        std::cout << "oh my god!!!!!!!!!!!" << std::endl;
                    for(GLuint j = 0; j < face.mNumIndices; j++)
                        mesh.m_tris.push_back(face.mIndices[j]);
                }  
                //mesh.recomputeTangents();
                mesh.computeBSphere();
                this->addObject(meshname, mesh);
                return meshname;
            }/*}}}*/

            glm::mat4 processTransform(aiMatrix4x4 t)/*{{{*/
            {
                //!fixme implement this function
                glm::mat4 mw;
                return mw;
            }/*}}}*/

            //!fixme: needs to be polished and tested
            void loadFromFile(const std::string& filename, const glm::mat4& mw=glm::mat4())/*{{{*/
            {
                Assimp::Importer importer;
                const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_FixInfacingNormals);
                if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE or !scene->mRootNode) /*{{{*/
                {
                    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                    return;
                }    /*}}}*/
                // Process all the scene's meshes and materials
                std::vector<std::string> meshnames;
                std::vector<std::string> matnames;
                std::vector<GLuint> textures;
                for(GLuint i = 0; i < scene->mNumTextures; i++)
                {
                    aiTexture* texture = scene-> mTextures[i];
                    textures.push_back(processTexture(texture));
                }
                for(GLuint i = 0; i < scene->mNumMaterials; i++)
                {
                    aiMaterial* mat = scene->mMaterials[i];
                    aiString matname;
                    mat->Get(AI_MATKEY_NAME, matname);
                    std::string strname=matname.C_Str();
                    materials[strname]=processMaterial(mat, textures);
                    matnames.push_back(strname);
                }
                for(GLuint i = 0; i < scene->mNumMeshes; i++)
                {
                    aiMesh* mesh = scene->mMeshes[i];
                    meshnames.push_back(processMesh(mesh, matnames));         
                }
                processNode(scene->mRootNode, mw, meshnames);
            }/*}}}*/

            void processNode(aiNode* node, glm::mat4 mw, const std::vector<std::string>& meshnames)/*{{{*/
            {
                std::string nodename=node->mName.C_Str();
                std::cout << "Node name: " << nodename << std::endl;
                mw=mw*processTransform(node->mTransformation);
                // Process all the node's meshes (if any)
                for(GLuint i = 0; i < node->mNumMeshes; i++)
                {
                    GLuint meshidx=node->mMeshes[i];
                    const std::string& meshname=meshnames[meshidx]; 
                    std::string instancename=nodename+meshname;
                    std::cout << "Instance name: " << instancename << std::endl;
                    addInstance(meshname, mw, instancename);
                }
                // Then do the same for each of its children
                for(GLuint i = 0; i < node->mNumChildren; i++)
                {
                    aiNode* child = node->mChildren[i];
                    processNode(child, mw, meshnames);
                }
            }/*}}}*/
            /*}}}*/


        private:
            Scene():controls(0)/*{{{*/
            {
                Program::setAttributeLocations();
            }/*}}}*/

            explicit Scene(const Scene& other);

            static Scene* instance;

        public:
            std::map<std::string, SceneObject*> objects;
            std::map<std::string, GLSLMaterial> materials;
            std::map<std::string, glm::mat4> mwMatrices;
            std::map<std::string, std::string> objectnames;
            std::string pickedname;
            Renderer* renderer;
            Controls controls;
        };
        Scene* Scene::instance=NULL;
        /*}}}*/

        /** @class GLRenderer {{{
         *  @brief Concrete class for OpenGL rendering
         */
        class GLRenderer: public Renderer
        {
        public:
            GLRenderer (): Renderer(){}

            virtual ~GLRenderer (){}

            void update(const std::string& objectname)/*{{{*/
            {
                const SceneObject& object = *scene.objects.at(objectname);
                const Mesh& mesh = object.asMesh();
                GLuint vao=make_vao();
                resources.vbos[objectname].assign(5, 0);
                glGenBuffers(5, &resources.vbos[objectname][0]);
                resources.vaos[objectname]=vao;
                GLenum usage=GL_STATIC_DRAW;
                GLuint ibo;
                glGenBuffers(1, &ibo);
                resources.ibos[objectname]=ibo;
                mesh.setupVAO(resources, objectname, usage);
            }/*}}}*/

            void renderInstance(const std::string& instancename, const GLSLMaterial& mat=GLSLMaterial(), bool special4picked=true) const/*{{{*/
            {
                if(not scene.containsInstance(instancename))
                    return;
                // program
                const ensi::gl::Program& prog=mat.program;
                glUseProgram(prog.progid);
                // common properties and instance matrix
                scene.controls.updateProgramUniforms(prog,  scene.mwMatrices.at(instancename));
                // object specific properties (material specs)
                mat.applyToProgram();
                special4picked = special4picked and (instancename==pickedname);
                mySolidObject(scene.objectnames.at(instancename), special4picked);
                // reset
                glUseProgram(0);
            }/*}}}*/

        private:
            void mySolidObject(const std::string& vaoname, bool special4picked=true) const/*{{{*/
            {
                /* Activate the object's vao and draw it in solid mode*/
                glBindVertexArray(resources.vaos.at(vaoname));
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.ibos.at(vaoname));
                GLenum type=GL_TRIANGLES;
                if(scene.controls.wireframe)
                    type=GL_LINES;
                /*!todo: Picking Lab Exercise 4: Highlight selected object {{{*/
                /*! Right now the selected object is displayed in wireframe; You
                 *  may find a more fancy way to put it in emphasis, maybe a
                 *  nice dedicated shader
                 * */
                if(special4picked)
                {
                    if(type==GL_TRIANGLES)
                        type=GL_LINES;
                    else
                        type=GL_TRIANGLES;
                }
                /*}}}*/
                glDrawElements(
                               type,  /* mode */
                               resources.ibonbtris.at(vaoname),/* count  (not memory size!)*/
                               GL_UNSIGNED_INT,  /* type */
                               (void*)0            /* element array buffer offset */
                              );

                glBindVertexArray(0);
            }/*}}}*/


        public:
            Resources resources;
            
        };
        /*}}}*/

        inline Renderer::Renderer ():scene(Scene::getInstance())/*{{{*/
        {
        }/*}}}*/

        inline void Controls::mouseButtonCallback(int button, int action)/*{{{*/
        {
            if(button==GLFW_MOUSE_BUTTON_LEFT)
            {
                if(action==GLFW_PRESS){
                    double x, y;
                    glfwGetCursorPos(window, &x, &y);
                    x*=screenToPixel[0];
                    y*=screenToPixel[1];
                    Scene& scene=Scene::getInstance();
                    std::string pickedname=scene.performPicking(x,  y);
                    scene.renderer->setPicked(pickedname);
                    std::cout << pickedname  << " was picked!" << std::endl;
                }
            }
        }
        /*}}}*/                                                                               

    }
}


std::ostream& operator<<(std::ostream& out, const ensi::gl::Vertex& v){/*{{{*/
    out<<"position :"<<v.position.x<<", "<<v.position.y<<", "<<v.position.z<<std::endl;
    out<<"normal :"<<v.normal.x<<", "<<v.normal.y<<", "<<v.normal.z<<std::endl;
    out<<"tangent :"<<v.tangent.x<<", "<<v.tangent.y<<", "<<v.tangent.z<<std::endl;
    out<<"color :"<<v.color.r<<", "<<v.color.g<<", "<<v.color.b<<", "<<v.color.a<<std::endl;
    out<<"uv :"<<v.uv.x<<", "<<v.uv.y;
    return out;
}/*}}}*/

std::ostream& operator<<(std::ostream& out, const ensi::gl::Mesh& m){/*{{{*/
    unsigned int i = 0;
    for(const ensi::gl::Vertex& v : m.m_verts)
    {
        out<<"Vertex "<<i++<<std::endl<<v<<std::endl;
    }
    for(unsigned int i = 0; i < m.m_tris.size(); i+=3)
    {
        out<<"Triangle "<<i/3<<": "<<std::endl
            <<m.m_tris[i]<<" "<<m.m_tris[i+1]<<" "<<m.m_tris[i+2]<<std::endl;
    }
    return out;
}/*}}}*/

std::ostream& operator<<(std::ostream& out, const ensi::gl::Program& p){/*{{{*/
    std::cout<<"Attributes:"<<std::endl;
    for(auto& entry : p.attributes)
    {
        const std::string& name=entry.first;
        GLint& index=entry.second;
        out<<"\t"<<name<<" -> "<<index<<std::endl;
    }
    return out;
}/*}}}*/

void displayMat4(const glm::mat4& m)/*{{{*/
{
    std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl 
              << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl 
              << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl 
              << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}/*}}}*/
#endif /* __LIGHTGLAPI_HPP__ */

// vim: set foldmethod=marker :

