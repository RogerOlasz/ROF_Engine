#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"

#ifdef _DEBUG
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#endif

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = false;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if (pbodyA && pbodyB)
			{
				p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
				while (item)
				{
					item->data->OnCollision(pbodyA, pbodyB);
					item = item->next;
				}

				item = pbodyB->collision_listeners.getFirst();
				while (item)
				{
					item->data->OnCollision(pbodyB, pbodyA);
					item = item->next;
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (debug == true)
	{
		world->debugDrawWorld();

		// Render vehicles
		p2List_item<PhysVehicle3D*>* item = vehicles.getFirst();
		while (item)
		{
			item->data->Render();
			item = item->next;
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Sphere s(1);
			s.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
			float force = 30.0f;
			AddBody(s)->Push(-(App->camera->Z.x * force), -(App->camera->Z.y * force), -(App->camera->Z.z * force));
		}
	}

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	for (p2List_item<btTypedConstraint*>* item = constraints.getFirst(); item; item = item->next)
	{
		world->removeConstraint(item->data);
		delete item->data;
	}

	constraints.clear();

	for (p2List_item<btDefaultMotionState*>* item = motions.getFirst(); item; item = item->next)
		delete item->data;

	motions.clear();

	for (p2List_item<btCollisionShape*>* item = shapes.getFirst(); item; item = item->next)
		delete item->data;

	shapes.clear();

	for (p2List_item<PhysBody3D*>* item = bodies.getFirst(); item; item = item->next)
		delete item->data;

	bodies.clear();

	for (p2List_item<PhysVehicle3D*>* item = vehicles.getFirst(); item; item = item->next)
		delete item->data;

	vehicles.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
{
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&sphere.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}


// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
	shapes.add(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	world->addRigidBody(body);
	bodies.add(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysVehicle3D* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.add(comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
	shapes.add(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));

	//--------------------------------------------------------------------------

	//Back
	btCollisionShape* colShape2 = new btBoxShape(btVector3(0.5f, 0.5f, 1));
	shapes.add(colShape2);
	btCollisionShape* colShape3 = new btBoxShape(btVector3(3, 0.1f, 1));
	shapes.add(colShape3);

	btTransform trans2;
	trans2.setIdentity();
	trans2.setOrigin(btVector3(0, info.chassis_offset.y + 1.7f, -2.5f));
	btTransform trans3;
	trans3.setIdentity();
	trans3.setOrigin(btVector3(0, info.chassis_offset.y + 2.3f, -2.5f));

	//Mud guard front left
	btCollisionShape* colShape4 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape4);
	btCollisionShape* colShape5 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape5);
	btCollisionShape* colShape6 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape6);

	btTransform trans4;
	trans4.setIdentity();
	trans4.setOrigin(btVector3(2.7f, info.chassis_offset.y + 1, 3.5f));

	btTransform trans5;
	trans5.setIdentity();
	trans5.setOrigin(btVector3(2.7f, info.chassis_offset.y + 0.545f, 4.883f));
	btQuaternion quat;
	quat.setRotation(btVector3(1, 0, 0), M_1_PI * 2);
	trans5.setRotation(quat);

	btTransform trans6;
	trans6.setIdentity();
	trans6.setOrigin(btVector3(2.7f, info.chassis_offset.y + 0.545f, 2.116f));
	btQuaternion quat2;
	quat2.setRotation(btVector3(1, 0, 0), -M_1_PI * 2);
	trans6.setRotation(quat2);

	//Mud guard front right
	btCollisionShape* colShape7 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape7);
	btCollisionShape* colShape8 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape8);
	btCollisionShape* colShape9 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape9);

	btTransform trans7;
	trans7.setIdentity();
	trans7.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 1, 3.5f));

	btTransform trans8;
	trans8.setIdentity();
	trans8.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 0.545f, 4.883f));
	btQuaternion quat3;
	quat3.setRotation(btVector3(1, 0, 0), M_1_PI * 2);
	trans8.setRotation(quat3);

	btTransform trans9;
	trans9.setIdentity();
	trans9.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 0.545f, 2.116f));
	btQuaternion quat4;
	quat4.setRotation(btVector3(1, 0, 0), -M_1_PI * 2);
	trans9.setRotation(quat4);

	//Mud guard back right
	btCollisionShape* colShape10 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape10);
	btCollisionShape* colShape11 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape11);
	btCollisionShape* colShape12 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape12);

	btTransform trans10;
	trans10.setIdentity();
	trans10.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 1, -3.5f));

	btTransform trans11;
	trans11.setIdentity();
	trans11.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 0.545f, -4.883f));
	btQuaternion quat5;
	quat5.setRotation(btVector3(1, 0, 0), -M_1_PI * 2);
	trans11.setRotation(quat5);

	btTransform trans12;
	trans12.setIdentity();
	trans12.setOrigin(btVector3(-2.7f, info.chassis_offset.y + 0.545f, -2.116f));
	btQuaternion quat6;
	quat6.setRotation(btVector3(1, 0, 0), M_1_PI * 2);
	trans12.setRotation(quat6);

	//Mud guard back left
	btCollisionShape* colShape13 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape13);
	btCollisionShape* colShape14 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape14);
	btCollisionShape* colShape15 = new btBoxShape(btVector3(0.6f, 0.1f, 0.8f));
	shapes.add(colShape15);

	btTransform trans13;
	trans13.setIdentity();
	trans13.setOrigin(btVector3(2.7f, info.chassis_offset.y + 1, -3.5f));

	btTransform trans14;
	trans14.setIdentity();
	trans14.setOrigin(btVector3(2.7f, info.chassis_offset.y + 0.545f, -4.883f));
	btQuaternion quat7;
	quat7.setRotation(btVector3(1, 0, 0), -M_1_PI * 2);
	trans14.setRotation(quat7);

	btTransform trans15;
	trans15.setIdentity();
	trans15.setOrigin(btVector3(2.7f, info.chassis_offset.y + 0.545f, -2.116f));
	btQuaternion quat8;
	quat8.setRotation(btVector3(1, 0, 0), M_1_PI * 2);
	trans15.setRotation(quat8);

	comShape->addChildShape(trans, colShape);

	comShape->addChildShape(trans2, colShape2);
	comShape->addChildShape(trans3, colShape3);

	comShape->addChildShape(trans4, colShape4);
	comShape->addChildShape(trans5, colShape5);
	comShape->addChildShape(trans6, colShape6);

	comShape->addChildShape(trans7, colShape7);
	comShape->addChildShape(trans8, colShape8);
	comShape->addChildShape(trans9, colShape9);

	comShape->addChildShape(trans10, colShape10);
	comShape->addChildShape(trans11, colShape11);
	comShape->addChildShape(trans12, colShape12);

	comShape->addChildShape(trans13, colShape13);
	comShape->addChildShape(trans14, colShape14);
	comShape->addChildShape(trans15, colShape15);



	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, -0.5f);
	comShape->calculateLocalInertia(info.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info.frictionSlip;
	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info.suspensionCompression;
	tuning.m_suspensionDamping = info.suspensionDamping;
	tuning.m_suspensionStiffness = info.suspensionStiffness;

	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for (int i = 0; i < info.num_wheels; ++i)
	{
		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
	}
	// ---------------------

	PhysVehicle3D* pvehicle = new PhysVehicle3D(body, vehicle, info);
	world->addVehicle(vehicle);
	vehicles.add(pvehicle);

	return pvehicle;
}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body),
		*(bodyB.body),
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	
	constraints.add(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body),
		*(bodyB.body),
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z),
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.add(hinge);
	hinge->setDbgDrawSize(2.0f);
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes)debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
