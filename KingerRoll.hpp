#ifndef KINGER_ROLL_HPP
#define KINGER_ROLL_HPP

#include <GL/glut.h>
#include <string>
#include "ObjModel.hpp"

namespace ProjectKingerRoll
{

class KingerRoll
{
private:
    ObjModel rollModel;
    bool rollLoaded;

public:
    KingerRoll();

    GLuint rollTextureID;

    bool loadRoll(const std::string& filePath);

    void drawRoll() const;
    void draw() const;
};

} // namespace ProjectKingerRoll

#endif
