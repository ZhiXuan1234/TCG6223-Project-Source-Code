/*
 TCG6223 Computer Graphics
 FIST, Multimedia University

 File: CNAworld.hpp
 Objective: Header file of CNAworld.cpp

 Reference code from:
 Copyright (C) by Ya-Ping Wong <ypwong@mmu.edu.my>

 Game and 3d model by
 Group C&A

*/


#ifndef CNAWORLD_HPP
#define CNAWORLD_HPP

#include "CNAmain.hpp"
#include "Kinger.hpp"
#include "Gloinks.hpp"
#include "Caine.hpp"
#include "Environment.hpp"
#include "AudioManager.hpp"

// Example Later you can add:
// #include "BattleEnvironment.hpp"

namespace ProjectWorld
{

class MyVirtualWorld
{
public:
    ProjectKinger::Kinger kinger;
    ProjectGloinks::Gloinks gloinks;
    ProjectCaine::Caine caine;
    ProjectEnvironment::Environment environment;
    // Example Later:
    // BattleEnvironment battleEnvironment;

    /*Audios*/
    AudioManager audioManager;

    void init();
    void draw();
    void tickTime();
};

} // namespace ProjectWorld

#endif
