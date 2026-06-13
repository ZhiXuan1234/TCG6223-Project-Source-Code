#ifndef BUTTERFLY_HPP
#define BUTTERFLY_HPP

#include <GL/glut.h>
#include <string>
#include "ObjModel.hpp"

namespace ProjectButterfly
{

class Butterfly
{
private:
    ObjModel leftWingModel;
    ObjModel rightWingModel;
    ObjModel tursoModel;

    bool leftWingLoaded;
    bool rightWingLoaded;
    bool tursoLoaded;

public:
    Butterfly();

    GLuint leftWingTextureID;
    GLuint rightWingTextureID;
    GLuint tursoTextureID;

    bool loadLeftWing(const std::string& filePath);
    bool loadRightWing(const std::string& filePath);
    bool loadTurso(const std::string& filePath);

    void drawLeftWing() const;
    void drawRightWing() const;
    void drawTurso() const;

    void draw() const;
};

} // namespace ProjectButterfly

#endif
