/*
 TCG6223 Computer Graphics
 FIST, Multimedia University

 File: CNAworld.cpp
 Objective: show environment / World & declare model file path

 Reference code from:
 Copyright (C) by Ya-Ping Wong <ypwong@mmu.edu.my>

 Game and 3d model by
 Group C&A

*/

#include <GL/glut.h>
#include <iostream>
#include "CNAWorld.hpp"
#include "TextureLoader.hpp"

using namespace ProjectWorld;

// =======================Whole environment lighting====================== //
void setupEnvironmentLighting()
{
    // Enable OpenGL lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Smooth shading
    glShadeModel(GL_SMOOTH);

    // Needed because many objects use glScalef()
    glEnable(GL_NORMALIZE);

    // Allow glColor3ub() to work with lighting
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Let both front and back faces receive lighting
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Make textures react to lighting
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Darker overall scene
    GLfloat globalAmbient[] = { 0.12f, 0.12f, 0.12f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // Main light position
    GLfloat lightPosition[] = { -180.0f, 220.0f, 120.0f, 1.0f };

    // Neutral dim white light
    GLfloat lightAmbient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.55f, 0.55f, 0.55f, 1.0f };
    GLfloat lightSpecular[] = { 0.25f, 0.25f, 0.25f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Less shiny, more dark/matte
    GLfloat materialSpecular[] = { 0.12f, 0.12f, 0.12f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 16.0f);
}
// ==================================================================== //

void MyVirtualWorld::init()
{
    //#//#//#//#//#//#//#//#//#//#//#//#Model#//#//#//#//#//#//#//#//#//#//#//

    //////////////////////////////////Kinger//////////////////////////////////
    const std::string headPath =
        "Model\\Kinger\\kinger_Head.txt";

    const std::string headPiecePath =
        "Model\\Kinger\\kinger_Head_Piece.txt";

    const std::string leftEyePath =
        "Model\\Kinger\\kinger_LeftEye.txt";

    const std::string rightEyeNPath =
        "Model\\Kinger\\kinger_RightEyeN.txt";

    const std::string bodyPath =
        "Model\\Kinger\\kinger_Body.txt";

    const std::string clothPath =
        "Model\\Kinger\\kinger_Cloth.txt";

    const std::string leftHandPath =
        "Model\\Kinger\\kinger_LeftHand.txt";

    const std::string rightHandwGunPath =
        "Model\\Kinger\\kinger_RightHandwGun.txt";

    const std::string bucketPath =
        "Model\\Kinger\\kinger_Bucket.txt";

    const std::string bucketHandlePath =
        "Model\\Kinger\\kinger_BucketHandle.txt";


    //////////////////////////////////Gloinks//////////////////////////////////

    const std::string gloinksBowlingPinPath =
        "Model\\Gloinks\\gloinks_BowlingPin.txt";

    const std::string gloinksCirclePath =
        "Model\\Gloinks\\gloinks_Circle.txt";

    const std::string gloinksCubePath =
        "Model\\Gloinks\\gloinks_Cube.txt";

    const std::string gloinksMoonPath =
        "Model\\Gloinks\\gloinks_Moon.txt";

    const std::string gloinksStarPath =
        "Model\\Gloinks\\gloinks_Star.txt";

    const std::string gloinksTriangularPath =
        "Model\\Gloinks\\gloinks_Triangular.txt";


    //////////////////////////////////Caine//////////////////////////////////
    const std::string caineHatPath =
        "Model\\Caine\\caine_Hat.txt";

    const std::string caineLeftHandPath =
        "Model\\Caine\\caine_LeftHand.txt";

    const std::string caineLeftLegPath =
        "Model\\Caine\\caine_LeftLeg.txt";

    const std::string caineLeftPalmPath =
        "Model\\Caine\\caine_LeftPalm.txt";

    const std::string caineLowerJawPath =
        "Model\\Caine\\caine_LowerJaw.txt";

    const std::string caineRightHandPath =
        "Model\\Caine\\caine_RightHand.txt";

    const std::string caineRightLegPath =
        "Model\\Caine\\caine_RightLeg.txt";

    const std::string caineRightPalmPath =
        "Model\\Caine\\caine_RightPalm.txt";

    const std::string caineStaffPath =
        "Model\\Caine\\caine_Staff.txt";

    const std::string caineTonguePath =
        "Model\\Caine\\caine_Tongue.txt";

    const std::string caineTursoPath =
        "Model\\Caine\\caine_Turso.txt";

    const std::string caineUpperJawPath =
        "Model\\Caine\\caine_UpperJaw.txt";

    const std::string caineLeftEyePath =
        "Model\\Caine\\caine_LeftEye.txt";

    const std::string caineRightEyePath =
        "Model\\Caine\\caine_RightEye.txt";
    ////////////////////////////////Environment//////////////////////////////
    const std::string skyBoxPath =
        "Model\\Environment\\Square SkyBox.txt";

    const std::string groundPath =
        "Model\\Environment\\Ground.txt";

    const std::string castleWallPath =
        "Model\\Environment\\CastleWall.txt";

    const std::string cubePath =
        "Model\\Environment\\Cube.txt";

    const std::string cubeGroupedPath =
        "Model\\Environment\\CubeGrouped.txt";

    const std::string irregularCubePath =
        "Model\\Environment\\IrregularCube.txt";

    const std::string pillarPath =
        "Model\\Environment\\Pillar.txt";

    const std::string roofPath =
        "Model\\Environment\\Roof.txt";

    const std::string spherePath =
        "Model\\Environment\\Sphere.txt";

    //#//#//#//#//#//#//#//#//#//#Condition Checking#//#//#//#//#//#//#//#//#//

    //////////////////////////////////Kinger//////////////////////////////////
    if (!kinger.loadHead(headPath))
    {
        std::cerr << "Kinger head failed to load.\n";
    }

    if (!kinger.loadHeadPiece(headPiecePath))
    {
        std::cerr << "Kinger head piece failed to load.\n";
    }

    if (!kinger.loadLeftEye(leftEyePath))
    {
        std::cerr << "Kinger left eye failed to load.\n";
    }

    if (!kinger.loadRightEyeN(rightEyeNPath))
    {
        std::cerr << "Kinger right eye failed to load.\n";
    }

    if (!kinger.loadBody(bodyPath))
    {
        std::cerr << "Kinger body failed to load.\n";
    }

    if (!kinger.loadCloth(clothPath))
    {
        std::cerr << "Kinger cloth failed to load.\n";
    }

    if (!kinger.loadLeftHand(leftHandPath))
    {
        std::cerr << "Kinger left hand failed to load.\n";
    }

    if (!kinger.loadRightHandwGun(rightHandwGunPath))
    {
        std::cerr << "Kinger right hand with Gun failed to load.\n";
    }

    if (!kinger.loadBucket(bucketPath))
    {
        std::cerr << "Kinger body failed to load.\n";
    }

    if (!kinger.loadBucketHandle(bucketHandlePath))
    {
        std::cerr << "Kinger cloth failed to load.\n";
    }

    //////////////////////////////////Gloinks//////////////////////////////////
    if (!gloinks.loadGloinksBowlingPin(gloinksBowlingPinPath))
    {
        std::cerr << "Gloinks Bowling Pin failed to load.\n";
    }

    if (!gloinks.loadGloinksCircle(gloinksCirclePath))
    {
        std::cerr << "Gloinks Circle failed to load.\n";
    }

    if (!gloinks.loadGloinksCube(gloinksCubePath))
    {
        std::cerr << "Gloinks Cube failed to load.\n";
    }

    if (!gloinks.loadGloinksMoon(gloinksMoonPath))
    {
        std::cerr << "Gloinks Moon failed to load.\n";
    }

    if (!gloinks.loadGloinksStar(gloinksStarPath))
    {
        std::cerr << "Gloinks Star failed to load.\n";
    }

    if (!gloinks.loadGloinksTriangular(gloinksTriangularPath))
    {
        std::cerr << "Gloinks Triangular failed to load.\n";
    }

    //////////////////////////////////Caine//////////////////////////////////
    if (!caine.loadHat(caineHatPath))
    {
        std::cerr << "Caine hat failed to load.\n";
    }

    if (!caine.loadLeftHand(caineLeftHandPath))
    { std::cerr << "Caine left hand failed to load.\n"; }

    if (!caine.loadLeftLeg(caineLeftLegPath))
    { std::cerr << "Caine left leg failed to load.\n"; }

    if (!caine.loadLeftPalm(caineLeftPalmPath))
    { std::cerr << "Caine left palm failed to load.\n"; }

    if (!caine.loadLowerJaw(caineLowerJawPath))
    { std::cerr << "Caine lower jaw failed to load.\n"; }

    if (!caine.loadRightHand(caineRightHandPath))
    { std::cerr << "Caine right hand failed to load.\n"; }

    if (!caine.loadRightLeg(caineRightLegPath))
    { std::cerr << "Caine right leg failed to load.\n"; }

    if (!caine.loadRightPalm(caineRightPalmPath))
    { std::cerr << "Caine right palm failed to load.\n"; }

    if (!caine.loadStaff(caineStaffPath))
    { std::cerr << "Caine staff failed to load.\n"; }

    if (!caine.loadTongue(caineTonguePath))
    { std::cerr << "Caine tongue failed to load.\n"; }

    if (!caine.loadTurso(caineTursoPath))
    { std::cerr << "Caine turso failed to load.\n"; }

    if (!caine.loadUpperJaw(caineUpperJawPath))
    { std::cerr << "Caine upper jaw failed to load.\n"; }

    if (!caine.loadLeftEye(caineLeftEyePath))
    { std::cerr << "Caine left eye failed to load.\n"; }

    if (!caine.loadRightEye(caineRightEyePath))
    { std::cerr << "Caine right eye failed to load.\n"; }

    ////////////////////////////////Environment//////////////////////////////
    if (!environment.loadSkyBox(skyBoxPath))
    {
        std::cerr << "SkyBox failed to load.\n";
    }

    if (!environment.loadGround(groundPath))
    {
        std::cerr << "Ground failed to load.\n";
    }

    if (!environment.loadCastleWall(castleWallPath))
    {
        std::cerr << "CastleWall failed to load.\n";
    }

    if (!environment.loadCube(cubePath))
    {
        std::cerr << "Cube failed to load.\n";
    }

    if (!environment.loadCubeGrouped(cubeGroupedPath))
    {
        std::cerr << "CubeGrouped failed to load.\n";
    }

    if (!environment.loadIrregularCube(irregularCubePath))
    {
        std::cerr << "IrregularCube failed to load.\n";
    }

    if (!environment.loadPillar(pillarPath))
    {
        std::cerr << "Pillar failed to load.\n";
    }

    if (!environment.loadRoof(roofPath))
    {
        std::cerr << "Roof failed to load.\n";
    }

    if (!environment.loadSphere(spherePath))
    {
        std::cerr << "Sphere failed to load.\n";
    }

    if (!environment.loadTextures())
    {
        std::cerr << "Environment textures failed to load.\n";
    }


    //#//#//#//#//#//#//#//#//#//#//#Texture#//#//#//#//#//#//#//#//#//#//#//#

    //////////////////////////////////Kinger//////////////////////////////////
    kinger.headTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/WoodTexture.jpg"
    );

    kinger.headPieceTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/WoodTexture.jpg"
    );
    kinger.clothTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/Kinger_Gown.png"
    );

    kinger.rightHandwGunTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/RightHandwGun.png"
    );

    kinger.leftEyeTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/LeftEye.png"
    );

    kinger.rightEyeNTextureID = TextureLoader::loadTexture(
    "Model/Kinger/Textures/RightEyeN.png"
    );

    //////////////////////////////////Gloinks//////////////////////////////////
    gloinks.BowlingPinTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_BowlingPin.png"
    );

    gloinks.CircleTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_Circle.png"
    );

    gloinks.CubeTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_Cube.png"
    );

    gloinks.MoonTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_Moon.png"
    );

    gloinks.StarTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_Star.png"
    );

    gloinks.TriangularTextureID = TextureLoader::loadTexture(
    "Model/Gloinks/Textures/Gloinks_Triangular.png"
    );

    //////////////////////////////////Caine//////////////////////////////////
    caine.hatTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_Hat.png");
    caine.leftHandTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_LeftHand.png");
    caine.leftLegTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_LeftLeg.png");
    caine.leftPalmTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_LeftPalm.png");
    caine.lowerJawTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_LowerJaw.png");
    caine.rightHandTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_RightHand.png");
    caine.rightLegTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_RightLeg.png");
    caine.rightPalmTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_RightPalm.png");
    caine.staffTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_Staff.png");
    caine.tongueTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_Tongue.png");
    caine.tursoTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_Turso.png");
    caine.upperJawTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_UpperJaw.png");
    caine.leftEyeTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_LeftEye.png");
    caine.rightEyeTextureID = TextureLoader::loadTexture("Model/Caine/Textures/Caine_RightEye.png");
    // Example Later:
    // battleEnvironment.init();
    // Continue here, please, thanks XD.

    //==================================================================
    // Notes: These two must put add the end of this function.
    /*Whole World Lightning*/
    setupEnvironmentLighting();

    /*Background Music*/
    audioManager.playBackgroundMusic("Audio\\BGM\\[Jigoku Shoujo OST] Ake ni Somaru - The Faustian (128k).wav");
}

void MyVirtualWorld::draw()
{
    /*Environment*/
    //environment.draw();

    /*Characters*/
    //kinger.draw();
    //gloinks.draw();
    caine.draw();
}

void MyVirtualWorld::tickTime()
{
    environment.tickTime();

    // Later animation / battle logic
}
