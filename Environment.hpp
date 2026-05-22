#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "ObjModel.hpp"
#include <string>

namespace ProjectEnvironment
{
    class Environment
    {
    private:
        ObjModel skyBoxModel;
        ObjModel groundModel;

        ObjModel castleWallModel;
        ObjModel cubeModel;
        ObjModel cubeGroupedModel;
        ObjModel irregularCubeModel;
        ObjModel pillarModel;
        ObjModel roofModel;
        ObjModel sphereModel;

        bool skyBoxLoaded;
        bool groundLoaded;

        bool castleWallLoaded;
        bool cubeLoaded;
        bool cubeGroupedLoaded;
        bool irregularCubeLoaded;
        bool pillarLoaded;
        bool roofLoaded;
        bool sphereLoaded;

    public:
        Environment();

        bool loadSkyBox(const std::string& filePath);
        bool loadGround(const std::string& filePath);

        bool loadCastleWall(const std::string& filePath);
        bool loadCube(const std::string& filePath);
        bool loadCubeGrouped(const std::string& filePath);
        bool loadIrregularCube(const std::string& filePath);
        bool loadPillar(const std::string& filePath);
        bool loadRoof(const std::string& filePath);
        bool loadSphere(const std::string& filePath);

        void drawSkyBox() const;
        void drawGround() const;

        void drawCastleWall() const;
        void drawCube() const;
        void drawCubeGrouped() const;
        void drawIrregularCube() const;
        void drawPillar() const;
        void drawRoof() const;
        void drawSphere() const;

        void draw() const;
    };
}

#endif
