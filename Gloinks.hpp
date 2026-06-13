#ifndef GLOINKS_HPP
#define GLOINKS_HPP

#include "ObjModel.hpp"
#include <string>

namespace ProjectGloinks
{

class Gloinks
{
private:
    ObjModel gloinksBowlingPinModel;
    ObjModel gloinksCircleModel;
    ObjModel gloinksCubeModel;
    ObjModel gloinksMoonModel;
    ObjModel gloinksStarModel;
    ObjModel gloinksTriangularModel;


    //ObjModel Model;

    bool gloinksBowlingPinLoaded;
    bool gloinksCircleLoaded;
    bool gloinksCubeLoaded;
    bool gloinksMoonLoaded;
    bool gloinksStarLoaded;
    bool gloinksTriangularLoaded;

    //bool Loaded;

public:
    Gloinks();

    GLuint BowlingPinTextureID;
    GLuint CircleTextureID;
    GLuint CubeTextureID;
    GLuint MoonTextureID;
    GLuint StarTextureID;
    GLuint TriangularTextureID;

    bool loadGloinksBowlingPin(const std::string& filePath);
    bool loadGloinksCircle(const std::string& filePath);
    bool loadGloinksCube(const std::string& filePath);
    bool loadGloinksMoon(const std::string& filePath);
    bool loadGloinksStar(const std::string& filePath);
    bool loadGloinksTriangular(const std::string& filePath);

    //bool load(const std::string& filePath);

    void drawGloinksBowlingPin() const;
    void drawGloinksCirle() const;
    void drawGloinksCube() const;
    void drawGloinksMoon() const;
    void drawGloinksStar() const;
    void drawGloinksTriangular() const;
    void draw() const;


};

} // namespace ProjectGloinks

#endif

