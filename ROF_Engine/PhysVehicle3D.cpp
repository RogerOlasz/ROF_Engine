#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet\include\btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
	//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Green;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.Render();

	//--------------------------------------------------------------------------------

	Cube chassis2(1, 1, 2);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis2.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(0, info.chassis_offset.y + 1.7f, -2.5f);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

	chassis2.transform.M[12] += offset2.getX();
	chassis2.transform.M[13] += offset2.getY();
	chassis2.transform.M[14] += offset2.getZ();

	chassis2.Render();

	Cube chassis3(6, 0.2f, 2);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis3.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(0, info.chassis_offset.y + 2.3f, -2.5f);
	offset3 = offset3.rotate(q3.getAxis(), q3.getAngle());

	chassis3.transform.M[12] += offset3.getX();
	chassis3.transform.M[13] += offset3.getY();
	chassis3.transform.M[14] += offset3.getZ();

	chassis3.Render();

	//Front left mud guard
	Cube chassis4(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis4.transform);
	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(2.7f, info.chassis_offset.y + 1, 3.5f);
	offset4 = offset4.rotate(q4.getAxis(), q4.getAngle());

	chassis4.transform.M[12] += offset4.getX();
	chassis4.transform.M[13] += offset4.getY();
	chassis4.transform.M[14] += offset4.getZ();

	chassis4.Render();

	Cube chassis5(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis5.transform);
	btQuaternion q5 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset5(2.7f, info.chassis_offset.y + 0.545f, 4.883f);
	offset5 = offset5.rotate(q5.getAxis(), q5.getAngle());

	chassis5.transform.M[12] += offset5.getX();
	chassis5.transform.M[13] += offset5.getY();
	chassis5.transform.M[14] += offset5.getZ();

	chassis5.Render();

	Cube chassis6(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis6.transform);
	btQuaternion q6 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset6(2.7f, info.chassis_offset.y + 0.545f, 2.116f);
	offset6 = offset6.rotate(q6.getAxis(), q6.getAngle());

	chassis6.transform.M[12] += offset6.getX();
	chassis6.transform.M[13] += offset6.getY();
	chassis6.transform.M[14] += offset6.getZ();

	chassis6.Render();

	//Front right mud guard
	Cube chassis7(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis7.transform);
	btQuaternion q7 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset7(-2.7f, info.chassis_offset.y + 1, 3.5f);
	offset7 = offset7.rotate(q7.getAxis(), q7.getAngle());

	chassis7.transform.M[12] += offset7.getX();
	chassis7.transform.M[13] += offset7.getY();
	chassis7.transform.M[14] += offset7.getZ();

	chassis7.Render();

	Cube chassis8(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis8.transform);
	btQuaternion q8 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset8(-2.7f, info.chassis_offset.y + 0.545f, 4.883f);
	offset8 = offset8.rotate(q8.getAxis(), q8.getAngle());

	chassis8.transform.M[12] += offset8.getX();
	chassis8.transform.M[13] += offset8.getY();
	chassis8.transform.M[14] += offset8.getZ();

	chassis8.Render();

	Cube chassis9(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis9.transform);
	btQuaternion q9 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset9(-2.7f, info.chassis_offset.y + 0.545f, 2.116f);
	offset9 = offset9.rotate(q9.getAxis(), q9.getAngle());

	chassis9.transform.M[12] += offset9.getX();
	chassis9.transform.M[13] += offset9.getY();
	chassis9.transform.M[14] += offset9.getZ();

	chassis9.Render();

	//Back right mud guard
	Cube chassis10(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis10.transform);
	btQuaternion q10 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset10(-2.7f, info.chassis_offset.y + 1, -3.5f);
	offset10 = offset10.rotate(q10.getAxis(), q10.getAngle());

	chassis10.transform.M[12] += offset10.getX();
	chassis10.transform.M[13] += offset10.getY();
	chassis10.transform.M[14] += offset10.getZ();

	chassis10.Render();

	Cube chassis11(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis11.transform);
	btQuaternion q11 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset11(-2.7f, info.chassis_offset.y + 0.545f, -4.883f);
	offset11 = offset11.rotate(q11.getAxis(), q11.getAngle());

	chassis11.transform.M[12] += offset11.getX();
	chassis11.transform.M[13] += offset11.getY();
	chassis11.transform.M[14] += offset11.getZ();

	chassis11.Render();

	Cube chassis12(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis12.transform);
	btQuaternion q12 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset12(-2.7f, info.chassis_offset.y + 0.545f, -2.116f);
	offset12 = offset12.rotate(q12.getAxis(), q12.getAngle());

	chassis12.transform.M[12] += offset12.getX();
	chassis12.transform.M[13] += offset12.getY();
	chassis12.transform.M[14] += offset12.getZ();

	chassis11.Render();

	//Back left mud guard
	Cube chassis13(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis13.transform);
	btQuaternion q13 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset13(2.7f, info.chassis_offset.y + 1, -3.5f);
	offset13 = offset13.rotate(q13.getAxis(), q13.getAngle());

	chassis13.transform.M[12] += offset13.getX();
	chassis13.transform.M[13] += offset13.getY();
	chassis13.transform.M[14] += offset13.getZ();

	chassis13.Render();

	Cube chassis14(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis14.transform);
	btQuaternion q14 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset14(2.7f, info.chassis_offset.y + 0.545f, -4.883f);
	offset14 = offset14.rotate(q14.getAxis(), q14.getAngle());

	chassis14.transform.M[12] += offset14.getX();
	chassis14.transform.M[13] += offset14.getY();
	chassis14.transform.M[14] += offset14.getZ();

	chassis14.Render();

	Cube chassis15(1.2f, 0.2f, 1.6f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis15.transform);
	btQuaternion q15 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset15(2.7f, info.chassis_offset.y + 0.545f, -2.116f);
	offset15 = offset15.rotate(q15.getAxis(), q15.getAngle());

	chassis15.transform.M[12] += offset15.getX();
	chassis15.transform.M[13] += offset15.getY();
	chassis15.transform.M[14] += offset15.getZ();

	chassis15.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
vec3 PhysVehicle3D::GetForwardVector() const
{
	btVector3 h = vehicle->getForwardVector();
	vec3 ret;
	ret.Set(h.getX(), h.getY(), h.getZ());
	return ret;
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::GetRefVehicle() const
{
	vec3 pos;
	btTransform trans = vehicle->getChassisWorldTransform();
	btMatrix3x3 mat = trans.getBasis();
	pos.x = mat.getColumn(0).getX();
	pos.y = mat.getColumn(1).getY();
	pos.z = mat.getColumn(2).getZ();

	return pos;
}

void PhysVehicle3D::Orient(float rot_angle)
{
	float matrix[16];
	memset(matrix, 0.0f, sizeof(matrix));

	vec3 p = GetPos();
	matrix[12] = p.x;
	matrix[13] = p.y;
	matrix[14] = p.z;
	matrix[15] = 1;

	matrix[0] = cos(rot_angle);
	matrix[2] = -sin(rot_angle);
	matrix[5] = 1;
	matrix[8] = sin(rot_angle);
	matrix[10] = cos(rot_angle);

	SetTransform(matrix);

}