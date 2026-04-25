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

// Later you can add:
// #include "Labubu.hpp"
// #include "BattleEnvironment.hpp"

namespace ProjectWorld
{

class MyVirtualWorld
{
public:
    ProjectKinger::Kinger kinger;

    // Later:
    // ProjectLabubu::Labubu labubu;
    // BattleEnvironment battleEnvironment;

    void init();
    void draw();
    void tickTime();
};

} // namespace ProjectWorld

#endif
