#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/common.hpp>
#include <glimac/Image.hpp>
#include <glm/gtc/random.hpp>
#include <vector>
#include <glimac/FreeflyCamera.hpp>
#include <glimac/Cone.hpp>
#include <src/stb_image.h>

using namespace glimac;

int window_width = 800;
int window_height = 800;
const GLuint VERTEX_ATTR_POSITION = 0;
const GLuint VERTEX_ATTR_NORMAL = 1;
const GLuint VERTEX_ATTR_TEXTURE = 2;
const GLuint VERTEX_ATTR_COLOR = 3;
auto freeflyCamera = FreeflyCamera();
bool rightClick = false;
double lastX = 0;
double lastY = 0;
bool z = false;
bool q = false;
bool s = false;
bool d = false;
bool inLineMode = false;

struct ConeProgram
{
    Program m_Program;

    GLint uMVPMatrixLocation;
    GLint uMVMatrixLocation;
    GLint uNormalMatrixLocation;
    GLint uTextureLocation;
    GLint uKdLocation;
    GLint uKsLocation;
    GLint uShininessLocation;
    GLint uLightPos1_vsLocation;
    GLint uLightIntensity1Location;
    GLint uLightPos2_vsLocation;
    GLint uLightIntensity2Location;

    ConeProgram(const FilePath &applicationPath) : m_Program(loadProgram(applicationPath.dirPath() + "/shaders/3D.vs.glsl",
                                                                         applicationPath.dirPath() + "/shaders/lightR1.fs.glsl"))
    {
        uMVPMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTextureLocation = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uKdLocation = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKsLocation = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininessLocation = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightPos1_vsLocation = glGetUniformLocation(m_Program.getGLId(), "uLightPos1_vs");
        uLightIntensity1Location = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity1");
        uLightPos2_vsLocation = glGetUniformLocation(m_Program.getGLId(), "uLightPos2_vs");
        uLightIntensity2Location = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity2");
    }
};

struct SnowProgram
{
    Program m_Program;

    GLint uMVPMatrixLocation;
    GLint uMVMatrixLocation;
    GLint uNormalMatrixLocation;
    GLint uTextureLocation;
    GLint uKdLocation;
    GLint uKsLocation;
    GLint uShininessLocation;
    GLint uLightPos1_vsLocation;
    GLint uLightIntensity1Location;
    GLint uLightPos2_vsLocation;
    GLint uLightIntensity2Location;

    SnowProgram(const FilePath &applicationPath) : m_Program(loadProgram(applicationPath.dirPath() + "/shaders/3D.vs.glsl",
                                                                         applicationPath.dirPath() + "/shaders/lightR1.fs.glsl"))
    {
        uMVPMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrixLocation = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTextureLocation = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        uKdLocation = glGetUniformLocation(m_Program.getGLId(), "uKd");
        uKsLocation = glGetUniformLocation(m_Program.getGLId(), "uKs");
        uShininessLocation = glGetUniformLocation(m_Program.getGLId(), "uShininess");
        uLightPos1_vsLocation = glGetUniformLocation(m_Program.getGLId(), "uLightPos1_vs");
        uLightIntensity1Location = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity1");
        uLightPos2_vsLocation = glGetUniformLocation(m_Program.getGLId(), "uLightPos2_vs");
        uLightIntensity2Location = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity2");
    }
};

struct Seethrough
{
};

std::vector<ShapeVertex> generateCube(float sizeX, float sizeY, float sizeZ, float r, float g, float b)
{
    std::vector<ShapeVertex> vertices;
    // FRONT
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 0, 1), glm::vec2(0, 1), glm::vec3(r, g, b)));
    // BACK
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(r, g, b)));
    // LEFT
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(-1, 0, 0), glm::vec2(0, 1), glm::vec3(r, g, b)));
    // RIGHT
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(r, g, b)));
    // TOP
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, -sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, sizeY / 2, sizeZ / 2), glm::vec3(0, 1, 0), glm::vec2(0, 1), glm::vec3(r, g, b)));
    // BOTTOM
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(1, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, -sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(0, 0), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(1, 1), glm::vec3(r, g, b)));
    vertices.push_back(ShapeVertex(glm::vec3(-sizeX / 2, -sizeY / 2, sizeZ / 2), glm::vec3(0, -1, 0), glm::vec2(0, 1), glm::vec3(r, g, b)));

    return vertices;
}

std::vector<ShapeVertex> generateRooms()
{
    std::vector<ShapeVertex> vertices;
    // SALLE 1 ################################################################
    // DERIERE
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    // DEVANT
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    // GAUCHE
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(10, 2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    // DROITE
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -2), glm::vec3(1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, -2), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, -2), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 2), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 2), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, 2), glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // ########################################################################
    // COULOIR ################################################################
    // DEVANT
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 2), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, 2), glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // DERIERE
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, -2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-10, 2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, -2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -2), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-11, 2, -2), glm::vec3(0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // ########################################################################
    // SALLE 2 ################################################################
    // DEVANT
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, 12), glm::vec3(0, 0, -1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 12), glm::vec3(0, 0, -1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // DERIERE
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, -12), glm::vec3(0, 0, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -12), glm::vec3(0, 0, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // GAUCHE
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -2), glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -2), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -2), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, -12), glm::vec3(-1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));

    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 2), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, -2, 2), glm::vec3(-1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 12), glm::vec3(-1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-12, 2, 2), glm::vec3(-1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // DROITE
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 0), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, 12), glm::vec3(1, 0, 0), glm::vec2(1, 1), glm::vec3(0, 0, 1)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, 2, -12), glm::vec3(1, 0, 0), glm::vec2(0, 1), glm::vec3(0, 0, 1)));
    // ########################################################################
    // SOL
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, -12), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(0.2, 0.68, 0.2)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, -12), glm::vec3(0, 1, 0), glm::vec2(1, 0), glm::vec3(0.2, 0.68, 0.2)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, 12), glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(0.2, 0.68, 0.2)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, -12), glm::vec3(0, 1, 0), glm::vec2(0, 0), glm::vec3(0.2, 0.68, 0.2)));
    vertices.push_back(ShapeVertex(glm::vec3(10, -2, 12), glm::vec3(0, 1, 0), glm::vec2(1, 1), glm::vec3(0.2, 0.68, 0.2)));
    vertices.push_back(ShapeVertex(glm::vec3(-32, -2, 12), glm::vec3(0, 1, 0), glm::vec2(0, 1), glm::vec3(0.2, 0.68, 0.2)));

    return vertices;
}

static void key_callback(GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_L)
    {
        inLineMode = !inLineMode;
        if (!inLineMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_W)
    {
        z = true;
    }
    else if (action == GLFW_RELEASE && key == GLFW_KEY_W)
    {
        z = false;
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_A)
    {
        q = true;
    }
    else if (action == GLFW_RELEASE && key == GLFW_KEY_A)
    {
        q = false;
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_S)
    {
        s = true;
    }
    else if (action == GLFW_RELEASE && key == GLFW_KEY_S)
    {
        s = false;
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_D)
    {
        d = true;
    }
    else if (action == GLFW_RELEASE && key == GLFW_KEY_D)
    {
        d = false;
    }
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int /*mods*/)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightClick = true;
        glfwGetCursorPos(window, &lastX, &lastY);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightClick = false;
    }
}

static void scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow * /*window*/, double xpos, double ypos)
{
    if (rightClick)
    {
        freeflyCamera.rotateLeft(-0.1f * (xpos - lastX));
        freeflyCamera.rotateUp(-0.1f * (ypos - lastY));
        lastX = xpos;
        lastY = ypos;
    }
}

static void size_callback(GLFWwindow * /*window*/, int width, int height)
{
    window_width = width;
    window_height = height;
}

GLuint loadSkybox(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "skybox failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main(int argc, char **argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "2 salles 2 ambiances", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    FilePath applicationPath(argv[0]);

    Program programColorR1 = loadProgram(applicationPath.dirPath() + "/shaders/3D.vs.glsl",
                                         applicationPath.dirPath() + "/shaders/colorR1.fs.glsl");

    GLint MVPMatrixColor = glGetUniformLocation(programColorR1.getGLId(), "uMVPMatrix");
    GLint MvMatrixColor = glGetUniformLocation(programColorR1.getGLId(), "uMVMatrix");
    GLint NormalsMatrixColor = glGetUniformLocation(programColorR1.getGLId(), "uNormalMatrix");
    GLint uKdLocationColor = glGetUniformLocation(programColorR1.getGLId(), "uKd");
    GLint uKsLocationColor = glGetUniformLocation(programColorR1.getGLId(), "uKs");
    GLint uShininessLocationColor = glGetUniformLocation(programColorR1.getGLId(), "uShininess");
    GLint uLightPos1_vsLocationColor = glGetUniformLocation(programColorR1.getGLId(), "uLightPos1_vs");
    GLint uLightIntensity1LocationColor = glGetUniformLocation(programColorR1.getGLId(), "uLightIntensity1");
    GLint uLightPos2_vsLocationColor = glGetUniformLocation(programColorR1.getGLId(), "uLightPos2_vs");
    GLint uLightIntensity2LocationColor = glGetUniformLocation(programColorR1.getGLId(), "uLightIntensity2");

    Program programColorR2 = loadProgram(applicationPath.dirPath() + "/shaders/3D.vs.glsl",
                                         applicationPath.dirPath() + "/shaders/colorR2.fs.glsl");

    GLint MVPMatrixColorR2 = glGetUniformLocation(programColorR2.getGLId(), "uMVPMatrix");
    GLint MvMatrixColorR2 = glGetUniformLocation(programColorR2.getGLId(), "uMVMatrix");
    GLint NormalsMatrixColorR2 = glGetUniformLocation(programColorR2.getGLId(), "uNormalMatrix");

    Program skyboxProgram = loadProgram(applicationPath.dirPath() + "/shaders/skybox.vs.glsl",
                                        applicationPath.dirPath() + "/shaders/skybox.fs.glsl");

    Program programST = loadProgram(applicationPath.dirPath() + "/shaders/3DST.vs.glsl",
                                    applicationPath.dirPath() + "/shaders/colorR2ST.fs.glsl");

    GLint MVPMatrixColorR2ST = glGetUniformLocation(programColorR2.getGLId(), "uMVPMatrix");
    GLint MvMatrixColorR2ST = glGetUniformLocation(programColorR2.getGLId(), "uMVMatrix");
    GLint NormalsMatrixColorR2ST = glGetUniformLocation(programColorR2.getGLId(), "uNormalMatrix");

    ConeProgram coneProgram(applicationPath);
    SnowProgram snowProgram(applicationPath);

    glEnable(GL_DEPTH_TEST);

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // SNOW SPHERE ########################################################
    Sphere sphere(1, 32, 16);

    // Textures
    auto snowTex = loadImage(applicationPath.dirPath() + "/assets/textures/neige.png");
    if (snowTex == nullptr)
    {
        std::cerr << "Error loading image" << std::endl;
        return -1;
    }
    GLuint snowTexture;
    glGenTextures(1, &snowTexture);
    glBindTexture(GL_TEXTURE_2D, snowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, snowTex->getWidth(), snowTex->getHeight(), 0, GL_RGBA, GL_FLOAT, snowTex->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################

    // MURS ####################################################################

    // VBO
    GLuint vbo2;
    glGenBuffers(1, &vbo2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);

    std::vector<ShapeVertex> vertices = generateRooms();

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao2;
    glGenVertexArrays(1, &vao2);

    glBindVertexArray(vao2);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################
    // STELE ####################################################################
    // VBO
    GLuint vbo3;
    glGenBuffers(1, &vbo3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo3);

    std::vector<ShapeVertex> vertices2 = generateCube(1, 1.4, 1, 1, 0, 0);

    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(ShapeVertex), vertices2.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao3;
    glGenVertexArrays(1, &vao3);

    glBindVertexArray(vao3);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo3);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################
    // SNOWMAN ################################################################
    Cone cone(0.5, 0.5, 32, 16);
    auto carotTex = loadImage(applicationPath.dirPath() + "/assets/textures/carotte.png");
    if (carotTex == nullptr)
    {
        std::cerr << "Error loading image" << std::endl;
        return -1;
    }
    GLuint carotTexture;
    glGenTextures(1, &carotTexture);
    glBindTexture(GL_TEXTURE_2D, carotTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, carotTex->getWidth(), carotTex->getHeight(), 0, GL_RGBA, GL_FLOAT, carotTex->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // VBO
    GLuint vbo4;
    glGenBuffers(1, &vbo4);

    glBindBuffer(GL_ARRAY_BUFFER, vbo4);

    glBufferData(GL_ARRAY_BUFFER, cone.getVertexCount() * sizeof(ShapeVertex), cone.getDataPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao4;
    glGenVertexArrays(1, &vao4);

    glBindVertexArray(vao4);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo4);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################
    // SNOW FLAKE ############################################################
    // VBO
    GLuint vbo5;
    glGenBuffers(1, &vbo5);

    glBindBuffer(GL_ARRAY_BUFFER, vbo5);

    std::vector<ShapeVertex> vertices3 = generateCube(2, 2, 2, 1, 1, 1);

    glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(ShapeVertex), vertices3.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao5;
    glGenVertexArrays(1, &vao5);

    glBindVertexArray(vao5);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo5);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################
    // TREE ####################################################################
    // VBO
    GLuint vbo6;
    glGenBuffers(1, &vbo6);

    glBindBuffer(GL_ARRAY_BUFFER, vbo6);

    std::vector<ShapeVertex> vertices4 = generateCube(0.5, 0.6, 0.5, 0.5, 0.25, 0);

    glBufferData(GL_ARRAY_BUFFER, vertices4.size() * sizeof(ShapeVertex), vertices4.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao6;
    glGenVertexArrays(1, &vao6);

    glBindVertexArray(vao6);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo6);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    Cone cone2(0.5, 0.5, 32, 16);
    auto treeTex = loadImage(applicationPath.dirPath() + "/assets/textures/sapin.png");
    if (treeTex == nullptr)
    {
        std::cerr << "Error loading image" << std::endl;
        return -1;
    }
    GLuint treeTexture;
    glGenTextures(1, &treeTexture);
    glBindTexture(GL_TEXTURE_2D, treeTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, treeTex->getWidth(), treeTex->getHeight(), 0, GL_RGBA, GL_FLOAT, treeTex->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // VBO
    GLuint vbo7;
    glGenBuffers(1, &vbo7);

    glBindBuffer(GL_ARRAY_BUFFER, vbo7);

    glBufferData(GL_ARRAY_BUFFER, cone2.getVertexCount() * sizeof(ShapeVertex), cone2.getDataPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao7;
    glGenVertexArrays(1, &vao7);

    glBindVertexArray(vao7);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbo7);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################
    // SKYBOX ##################################################################
    std::vector<std::string> faces{
        applicationPath.dirPath() + "/assets/textures/right.jpg",
        applicationPath.dirPath() + "/assets/textures/left.jpg",
        applicationPath.dirPath() + "/assets/textures/top.jpg",
        applicationPath.dirPath() + "/assets/textures/bot.jpg",
        applicationPath.dirPath() + "/assets/textures/front.jpg",
        applicationPath.dirPath() + "/assets/textures/back.jpg"};

    GLuint skyboxTexture = loadSkybox(faces);

    // VBO
    GLuint vbo8;
    glGenBuffers(1, &vbo8);

    glBindBuffer(GL_ARRAY_BUFFER, vbo8);

    std::vector<GLfloat> skyboxVertices = {
        -1.0f, 1.0f, -1.0f, // Face arrière
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f, // Face avant
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f, // Face droite
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f, // Face gauche
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f, // Face du haut
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f, // Face du bas
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao8;
    glGenVertexArrays(1, &vao8);

    glBindVertexArray(vao8);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, vbo8);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // ########################################################################

    // VERRE ###################################################################
    // VBO
    GLuint vbo9;
    glGenBuffers(1, &vbo9);

    glBindBuffer(GL_ARRAY_BUFFER, vbo9);

    std::vector<ShapeVertex> vertices5 = generateCube(0.7, 0.8, 0.05, 1.0, 0.412, 0.706);

    glBufferData(GL_ARRAY_BUFFER, vertices5.size() * sizeof(ShapeVertex), vertices5.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // VAO
    GLuint vao9;
    glGenVertexArrays(1, &vao9);

    glBindVertexArray(vao9);

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo9);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid *)offsetof(ShapeVertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // ########################################################################

    glm::mat4 defaultProjMatrix = glm::perspective(glm::radians(70.f), (float)window_width / window_height, 0.1f, 100.f);
    glm::mat4 defaultMVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));

    glm::vec3 globKd = glm::vec3(glm::linearRand(0.5f, 1.f), glm::linearRand(0.5f, 1.f), glm::linearRand(0.5f, 1.f));
    glm::vec3 globKs = glm::vec3(glm::linearRand(0.5f, 1.f), glm::linearRand(0.5f, 1.f), glm::linearRand(0.5f, 1.f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        if (z)
        {
            freeflyCamera.moveFront(0.1f);
        }
        if (q)
        {
            freeflyCamera.moveLeft(0.1f);
        }
        if (s)
        {
            freeflyCamera.moveFront(-0.1f);
        }
        if (d)
        {
            freeflyCamera.moveLeft(-0.1f);
        }

        auto cameraViewMatrix = freeflyCamera.getViewMatrix();
        glm::mat4 ProjMatrix = defaultProjMatrix;
        glm::mat4 MVMatrix = defaultMVMatrix * cameraViewMatrix;
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glm::vec3 lightPosR12 = glm::mat3(cameraViewMatrix) * glm::vec3(0, 0, 0);
        glm::vec3 lightPosR1 = glm::vec3(MVMatrix * glm::vec4(0, 0, 0, 1));

        glClearColor(0.3f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        glDepthFunc(GL_LEQUAL);
        skyboxProgram.use();
        glm::mat4 view = glm::mat4(glm::mat3(freeflyCamera.getViewMatrix()));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window_width / window_height, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram.getGLId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram.getGLId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao8);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, skyboxVertices.size());
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        if (freeflyCamera.getPosition().x > -11)
        {
            programColorR1.use();
            glUniform3f(uKdLocationColor, globKd.x, globKd.y, globKd.z);
            glUniform3f(uKsLocationColor, globKs.x, globKs.y, globKs.z);
            glUniform1f(uShininessLocationColor, 32.f);
            glUniform3f(uLightPos1_vsLocationColor, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(uLightIntensity1LocationColor, 0.3, 0.3, 0.7);
            glUniform3f(uLightPos2_vsLocationColor, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(uLightIntensity2LocationColor, 1, 1, 1);
        }
        else
        {
            programColorR2.use();
            glUniformMatrix4fv(MVPMatrixColorR2, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        }
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);

        if (freeflyCamera.getPosition().x < -11)
        {
            glUniform3f(uKdLocationColor, globKd.x, globKd.y, globKd.z);
            glUniform3f(uKsLocationColor, globKs.x, globKs.y, globKs.z);
            glUniform1f(uShininessLocationColor, 32.f);
            glUniform3f(uLightPos1_vsLocationColor, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(uLightIntensity1LocationColor, 0, 0, 0);
            glUniform3f(uLightPos2_vsLocationColor, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(uLightIntensity2LocationColor, 0, 0, 0);

            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0, 0, 0);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 0, 0, 0);

            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0, 0, 0);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 0, 0, 0);
        }
        else
        {
            glUniform3f(uKdLocationColor, globKd.x, globKd.y, globKd.z);
            glUniform3f(uKsLocationColor, globKs.x, globKs.y, globKs.z);
            glUniform1f(uShininessLocationColor, 32.f);
            glUniform3f(uLightPos1_vsLocationColor, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(uLightIntensity1LocationColor, 0.3, 0.3, 0.7);
            glUniform3f(uLightPos2_vsLocationColor, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(uLightIntensity2LocationColor, 1, 1, 1);
        }
        // STELE SALLE 2 ##################################################################
        {
            programColorR2.use();
            glBindVertexArray(vao3);
            // Placer le cube au centre de la salle 2
            MVMatrix = glm::translate(MVMatrix, glm::vec3(-22, -1.3, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

            glBindVertexArray(0);

            float front = (freeflyCamera.getPosition().x < -21) ? 1 : -1;
            programST.use();
            glBindVertexArray(vao9);
            // Placer le verre sur la stele
            MVMatrix = glm::translate(MVMatrix, glm::vec3(front * 0.3, 1, 0));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(0, 1, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, 0, vertices5.size());
            glDisable(GL_BLEND);

            MVMatrix = glm::rotate(MVMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(front * -0.3, 0, 0));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(0, 1, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, 0, vertices5.size());
            glDisable(GL_BLEND);

            MVMatrix = glm::rotate(MVMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(front * -0.3, 0, 0));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(0, 1, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_TRIANGLES, 0, vertices5.size());
            glDisable(GL_BLEND);

            MVMatrix = glm::rotate(MVMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(front * 0.3, -1, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2ST, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glBindVertexArray(0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(22, 1.3, 0));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColorR2, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColorR2, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColorR2, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        }
        // ################################################################################

        // SNOWMAN SALLE 1 ################################################################
        {
            snowProgram.m_Program.use();
            glBindVertexArray(vao);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, snowTexture);
            glUniform1i(snowProgram.uTextureLocation, 0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(-7, -1.5, 7));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.7, 0.7, 0.7));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(snowProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(snowProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(snowProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(snowProgram.uShininessLocation, 32.f);
            glUniform3f(snowProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(snowProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(snowProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(snowProgram.uLightIntensity2Location, 1, 1, 1);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.7, 1 / 0.7, 1 / 0.7));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 0.8, 0));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.5, 0.5, 0.5));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(snowProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(snowProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(snowProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(snowProgram.uShininessLocation, 32.f);
            glUniform3f(snowProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(snowProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(snowProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(snowProgram.uLightIntensity2Location, 1, 1, 1);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(2, 2, 2));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 0.6, 0));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.3, 0.3, 0.3));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(snowProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(snowProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(snowProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(snowProgram.uShininessLocation, 32.f);
            glUniform3f(snowProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(snowProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(snowProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(snowProgram.uLightIntensity2Location, 1, 1, 1);

            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.3, 1 / 0.3, 1 / 0.3));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(7, -0.1, -7));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(snowProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(snowProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(snowProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(snowProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(snowProgram.uShininessLocation, 32.f);
            glUniform3f(snowProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(snowProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(snowProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(snowProgram.uLightIntensity2Location, 1, 1, 1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);

            coneProgram.m_Program.use();
            glBindVertexArray(vao4);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, carotTexture);
            glUniform1i(coneProgram.uTextureLocation, 0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(-7, 0.1, 6.71));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.1, 0.5, 0.1));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(coneProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 1, 1, 1);

            glDrawArrays(GL_TRIANGLES, 0, cone.getVertexCount());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.1, 1 / 0.5, 1 / 0.1));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(90.f), glm::vec3(1, 0, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(7, -0.1, -6.71));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(coneProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 1, 1, 1);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
        // ################################################################################

        // TREE SALLE 1 ###################################################################
        {
            // Placer arbre derriere au centre de la salle 1

            programColorR1.use();
            glBindVertexArray(vao6);
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, -1.5, -9));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices2.size());
            glBindVertexArray(0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 1.5, 9));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            coneProgram.m_Program.use();
            glBindVertexArray(vao7);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, treeTexture);
            glUniform1i(coneProgram.uTextureLocation, 0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, -1.2, -9));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f), glm::vec3(0, 1, 0));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(3, 6, 3));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(coneProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 1, 1, 1);

            glDrawArrays(GL_TRIANGLES, 0, cone.getVertexCount());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindVertexArray(0);

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 3.0, 1 / 6.0, 1 / 3.0));
            MVMatrix = glm::rotate(MVMatrix, glm::radians(-180.f), glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 1.2, 9));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(coneProgram.uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(coneProgram.uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniform3f(coneProgram.uKdLocation, globKd.x, globKd.y, globKd.z);
            glUniform3f(coneProgram.uKsLocation, globKs.x, globKs.y, globKs.z);
            glUniform1f(coneProgram.uShininessLocation, 32.f);
            glUniform3f(coneProgram.uLightPos1_vsLocation, lightPosR1.x, lightPosR1.y, lightPosR1.z);
            glUniform3f(coneProgram.uLightIntensity1Location, 0.3, 0.3, 0.7);
            glUniform3f(coneProgram.uLightPos2_vsLocation, lightPosR12.x, lightPosR12.y, lightPosR12.z);
            glUniform3f(coneProgram.uLightIntensity2Location, 1, 1, 1);
        }
        // ################################################################################

        // SNOW FLAKE #####################################################################
        {
            programColorR1.use();

            glBindVertexArray(vao3);
            // Placer le cube dans le coin en haut à gauche de la salle 1
            MVMatrix = glm::translate(MVMatrix, glm::vec3(7, -1.2, 7));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

            glBindVertexArray(0);

            // Rotation continue du flocon

            MVMatrix = glm::rotate(MVMatrix, (float)glfwGetTime() * 0.8f, glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, sinf((float)glfwGetTime()) * 0.2, 0));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glBindVertexArray(vao5);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, 1.6, 0));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.05, 0.6, 0.01));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.05, 1 / 0.6, 1 / 0.01));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(0.6, 0.05, 0.01));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

            MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.6, 1 / 0.05, 1 / 0.01));

            for (int i = 0; i < 4; i++)
            {
                MVMatrix = glm::translate(MVMatrix, glm::vec3(0.1, 0.35, 0));
                MVMatrix = glm::rotate(MVMatrix, glm::radians(-45.f), glm::vec3(0, 0, 1));
                MVMatrix = glm::scale(MVMatrix, glm::vec3(0.05, 0.16, 0.01));
                NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
                glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
                glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

                glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

                MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.05, 1 / 0.16, 1 / 0.01));
                MVMatrix = glm::rotate(MVMatrix, glm::radians(45.f), glm::vec3(0, 0, 1));
                MVMatrix = glm::translate(MVMatrix, glm::vec3(-0.2, 0, 0));
                MVMatrix = glm::rotate(MVMatrix, glm::radians(45.f), glm::vec3(0, 0, 1));
                MVMatrix = glm::scale(MVMatrix, glm::vec3(0.05, 0.16, 0.01));
                NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
                glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
                glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

                glDrawArrays(GL_TRIANGLES, 0, vertices3.size());

                MVMatrix = glm::scale(MVMatrix, glm::vec3(1 / 0.05, 1 / 0.16, 1 / 0.01));
                MVMatrix = glm::rotate(MVMatrix, glm::radians(45.f), glm::vec3(0, 0, 1));
                MVMatrix = glm::translate(MVMatrix, glm::vec3(-0.35, -0.1, 0));
                NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
                glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
                glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            }
            glBindVertexArray(0);

            MVMatrix = glm::translate(MVMatrix, glm::vec3(0, -sinf((float)glfwGetTime()) * 0.2, 0));
            MVMatrix = glm::rotate(MVMatrix, (float)glfwGetTime() * -0.8f, glm::vec3(0, 1, 0));
            MVMatrix = glm::translate(MVMatrix, glm::vec3(-7, -0.2, -7));
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            glUniformMatrix4fv(MVPMatrixColor, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(MvMatrixColor, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(NormalsMatrixColor, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        }
        // ###############################################################################

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo2);
    glDeleteVertexArrays(1, &vao2);
    glDeleteBuffers(1, &vbo3);
    glDeleteVertexArrays(1, &vao3);
    glDeleteBuffers(1, &vbo4);
    glDeleteVertexArrays(1, &vao4);
    glDeleteBuffers(1, &vbo5);
    glDeleteVertexArrays(1, &vao5);
    glDeleteBuffers(1, &vbo6);
    glDeleteVertexArrays(1, &vao6);
    glDeleteBuffers(1, &vbo7);
    glDeleteVertexArrays(1, &vao7);
    glDeleteBuffers(1, &vbo8);
    glDeleteVertexArrays(1, &vao8);
    glDeleteBuffers(1, &vbo9);
    glDeleteVertexArrays(1, &vao9);

    glfwTerminate();
    return 0;
}