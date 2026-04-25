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

using namespace ProjectWorld;

void MyVirtualWorld::init()
{
    const std::string headPath =
        "Model\\Kinger\\kinger_Head.txt";

    const std::string headPiecePath =
        "Model\\Kinger\\kinger_Head_Piece.txt";

    const std::string leftEyePath =
        "Model\\Kinger\\kinger_LeftEye.txt";

    const std::string rightEyePath =
        "Model\\Kinger\\kinger_RightEye.txt";

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

    if (!kinger.loadRightEye(rightEyePath))
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

    // Later:
    // labubu.loadHead(...);
    // battleEnvironment.init();
}

void MyVirtualWorld::draw()
{
    // Later:
    // battleEnvironment.draw();

    kinger.draw();

    // Later:
    // labubu.draw();
}

void MyVirtualWorld::tickTime()
{
    // Later animation / battle logic
}
