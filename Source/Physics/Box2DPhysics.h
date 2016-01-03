/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#ifndef BOX_2D_PHYSICS
#define BOX_2D_PHYSICS
#include "IPhysics.h"
#include "Box2DDebugDrawer.h"
#include <Box2D/Box2D.h>
#include "../Components/Actor.h"
#include <memory>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <vector>

class Box2DPhysics;
class Box2DContactListener;

struct BodyUserData
{
    Box2DPhysics*       Physics;
    DEFAULT_ACTOR_TYPES Type;
};

class Box2DPhysics : public IPhysics
{
    friend Box2DContactListener;
    // physics world
    b2World* m_World;
    Box2DContactListener* m_ContactListener;
    Box2DDebugDrawer*     m_DebugDrawer;
    // body-actor relations
    std::map<ActorID, b2Body*> m_ActorIDToBody;
    std::map<b2Body*, ActorID> m_BodyToActorID;
    // collisions
	std::list<b2Contact*> m_Collisions;
    void SendCollisionAddEvent(b2Contact* contact);
    void SendCollisionRemoveEvent(b2Contact * contact);
    // removal queue
    std::list<ActorID> m_RemovalQueue;
public:
    Box2DPhysics();
    ~Box2DPhysics();

    // initialization and maintenance of physics world
    void Initialize();
    void Update();
    void SyncVisibleScene();
    void RemoveQueuedItems();
    void Reset();
    // body-actor relations
    b2Body* FindBody(ActorID);
    ActorID FindActorID(b2Body*);
    // integration of physics objects
    void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool isSensor = false);
    void AddBox(std::shared_ptr<Actor> actor, float density, std::string type = "static", bool fixedRotation = true, bool isSensor = false, float hitBoxScale = 1.0f);
    void AddPolygon(std::shared_ptr<Actor> actor, std::vector<glm::vec2> vertices, float density, bool dynamic = false, bool fixedRotation = true);
    void AddCharacter(std::shared_ptr<Actor> actor, float density);
    void RemoveActor(unsigned int ActorID);
    // debugging
    void RenderDiagnostics();
    // physics world modifiers/forces
    void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons);
	// per body physics data in game-world coordinates
	glm::vec2 GetLinearVelocity(unsigned int ActorID);
    void SetLinearVelocity(unsigned ActorID, glm::vec2 velocity);
	float GetBodyMass(unsigned int ActorID);
	// collision check functions
	bool IsBodiesColliding(const b2Body* bodyA, const b2Body* bodyB);
	
};

#endif