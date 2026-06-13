#ifndef CAINE_HPP
#define CAINE_HPP

#include <GL/glut.h>
#include <string>
#include "ObjModel.hpp"

namespace ProjectCaine
{

class Caine
{
private:
    ObjModel hatModel;
    ObjModel leftHandModel;
    ObjModel leftLegModel;
    ObjModel leftPalmModel;
    ObjModel lowerJawModel;
    ObjModel rightHandModel;
    ObjModel rightLegModel;
    ObjModel rightPalmModel;
    ObjModel staffModel;
    ObjModel tongueModel;
    ObjModel tursoModel;
    ObjModel upperJawModel;
    ObjModel leftEyeModel;
    ObjModel rightEyeModel;

    bool hatLoaded;
    bool leftHandLoaded;
    bool leftLegLoaded;
    bool leftPalmLoaded;
    bool lowerJawLoaded;
    bool rightHandLoaded;
    bool rightLegLoaded;
    bool rightPalmLoaded;
    bool staffLoaded;
    bool tongueLoaded;
    bool tursoLoaded;
    bool upperJawLoaded;
    bool leftEyeLoaded;
    bool rightEyeLoaded;



public:
    Caine();

    GLuint hatTextureID;
    GLuint leftHandTextureID;
    GLuint leftLegTextureID;
    GLuint leftPalmTextureID;
    GLuint lowerJawTextureID;
    GLuint rightHandTextureID;
    GLuint rightLegTextureID;
    GLuint rightPalmTextureID;
    GLuint staffTextureID;
    GLuint tongueTextureID;
    GLuint tursoTextureID;
    GLuint upperJawTextureID;
    GLuint leftEyeTextureID;
    GLuint rightEyeTextureID;

    bool loadHat(const std::string& filePath);
    bool loadLeftHand(const std::string& filePath);
    bool loadLeftLeg(const std::string& filePath);
    bool loadLeftPalm(const std::string& filePath);
    bool loadLowerJaw(const std::string& filePath);
    bool loadRightHand(const std::string& filePath);
    bool loadRightLeg(const std::string& filePath);
    bool loadRightPalm(const std::string& filePath);
    bool loadStaff(const std::string& filePath);
    bool loadTongue(const std::string& filePath);
    bool loadTurso(const std::string& filePath);
    bool loadUpperJaw(const std::string& filePath);
    bool loadRightEye(const std::string& filePath);
    bool loadLeftEye(const std::string& filePath);

    void drawHat() const;
    void drawLeftHand() const;
    void drawLeftLeg() const;
    void drawLeftPalm() const;
    void drawLowerJaw() const;
    void drawRightHand() const;
    void drawRightLeg() const;
    void drawRightPalm() const;
    void drawStaff() const;
    void drawTongue() const;
    void drawTurso() const;
    void drawUpperJaw() const;
    void drawRightEye() const;
    void drawLeftEye() const;

    void draw() const;
};

} // namespace ProjectCaine

#endif
