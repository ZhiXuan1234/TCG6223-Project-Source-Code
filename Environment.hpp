#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "ObjModel.hpp"
#include <string>
#include <GL/glut.h>

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

        // Environment Animations
        float animationTime;

        // Environment Object Loader Checker
        bool skyBoxLoaded;
        bool groundLoaded;
        bool castleWallLoaded;
        bool cubeLoaded;
        bool cubeGroupedLoaded;
        bool irregularCubeLoaded;
        bool pillarLoaded;
        bool roofLoaded;
        bool sphereLoaded;

        // Render State Trackers
        mutable bool isCubeDrawn;
        mutable bool isPillarDrawn;
        mutable bool isIrregularCubeDrawn;
        mutable bool isCastleWallDrawn;

        // Texture IDs
        GLuint skyBoxTexture;
        GLuint groundTexture;
        GLuint roofTexture;
        GLuint castleWallTexture;
        GLuint cube1Texture;
        GLuint cube2Texture;
        GLuint cubeGroupedTexture;
        GLuint circusObject1Texture;
        GLuint circusObject2Texture;
        GLuint pillarTexture;
        GLuint skyTexture;

        // Animation Texture IDs
        GLuint glitchTexture;

    public:
        Environment();

        // Environment Animation
        void tickTime();
        float getAnimationTime() const { return animationTime; }

        // Texture Loading
        bool loadTextures();

        // Object Construction
        bool loadSkyBox(const std::string& filePath);
        bool loadGround(const std::string& filePath);
        bool loadRoof(const std::string& filePath);
        bool loadCastleWall(const std::string& filePath);
        bool loadCube(const std::string& filePath);
        bool loadCubeGrouped(const std::string& filePath);
        bool loadIrregularCube(const std::string& filePath);
        bool loadPillar(const std::string& filePath);
        bool loadSphere(const std::string& filePath);

        void drawSkyBox() const;
        void drawGround() const;
        void drawRoof() const;
        void drawCastleWall() const;
        void drawCube() const;
        void drawCubeGrouped() const;
        void drawIrregularCube() const;
        void drawPillar() const;
        void drawSphere() const;

        // Glitch Animation
        void drawDigitalEffect() const;
        void drawGlitchPanelEffect(float x = 0.0f, float y = 0.0f, float z = 0.0f, float width = 22.0f, float height = 8.0f, float uShift = 0.0f, float alpha = 1.0f, float rotationAngle = 0.0f) const;
        void drawThinGlitchLineEffect(float x = 0.0f, float y = 0.0f, float z = 0.0f, float length = 35.0f, float alpha = 1.0f, float dirX = 1.0f, float dirY = 0.057f, float dirZ = 0.0f) const;

        // Bounding Box Collision
        bool checkWallCollision(float playerX, float playerZ, float radius, float& outNewX, float& outNewZ) const;
        bool checkObstacleCollision(float playerX, float playerZ, float playerY, float radius, float& outNewX, float& outNewZ, float& outGroundY, bool isGloink = false) const;
        void getSkyBoxBounds(Vec3& minB, Vec3& maxB) const;

        //Main Draw Function
        void draw() const;
    };
}

#endif
