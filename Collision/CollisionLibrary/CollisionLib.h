#pragma once
#include <math.h>
#include <DirectXMath.h>

using namespace DirectX;

/************************************************
* Filename:      CollisionLib.h
* Date:          06/08/2015
* Mod. Date:     08/05/2015
* Mod. Initials: MM
* Author:        Mitchel Mishiev
* Purpose:       To hold all collisions functions
*************************************************/

struct tPlane
{
	XMFLOAT3 m_tNormal;
	float m_fOffset;
};

struct tAABB
{
	XMFLOAT3 m_tMin;
	XMFLOAT3 m_tMax;
};

struct tSegment
{
	XMFLOAT3 m_tStart;
	XMFLOAT3 m_tEnd;
};

struct tSphere
{
	XMFLOAT3 m_tCenter;
	float m_fRadius;
};

struct tFrustum
{
	tPlane m_tPlanes[6];
	XMFLOAT3 m_tCorners[8];
};

struct tCapsule
{
	tSegment m_tsegment;
	float m_fRadius;
};

struct tRay
{
	XMFLOAT3 m_tStartPoint;
	XMFLOAT3 m_tRayDirection;
};

enum eFrustumCorners { FTL = 0, FBL, FBR, FTR, NTL, NTR, NBR, NBL };
enum eFrustumPlanes { PNEAR = 0, PFAR, PLEFT, PRIGHT, PTOP, PBOTTOM };

void CalculatePlane( tPlane &_tplane, const XMFLOAT3& _tpointA, const XMFLOAT3& _tpointB, const XMFLOAT3& _tpointC);

int IdentifyPointToPlane( const tPlane& _tplane, const XMFLOAT3& _tpoint);

int IdentifySphereToPlane( const tPlane& _tplane, const tSphere& _tsphere);

int IdentifyAABBtoPlane( const tPlane& _tplane, const tAABB& _taabb );

void BuildFrustum( tFrustum& _tfrustum, float _fFov, float _fNearDist, float _fFarDist, float _fRatio, const XMFLOAT4X4& _tcamXform);

bool FrustumToSphere( const tFrustum& _tfrustum, const tSphere& _tsphere);

bool FrustumToAABB(const tFrustum& _tfrustum, const tAABB& _taabb);

bool AABBtoAABB(const tAABB& _taabbA, const tAABB& _taabbB);

bool SphereToSphere(const tSphere& _tsphereA, const tSphere& _tsphereB);

bool SphereToAABB(const tSphere& _tsphere, const tAABB& _taabb);

bool CapsuleToSphere(const tCapsule& _tcapsule, const tSphere& _tsphere);

bool CapsuleToAABB(const tCapsule& _tcapsule, const tAABB& _taabb);

bool CapsuleToCapsule(const tCapsule& _tcapsuleA, const tCapsule& _tcapsuleB);

bool RayToSphere(const tRay& _tray, const tSphere& _tsphere);

bool RayToAABB(const tRay& _tray, const tAABB& _taabb);

void ClosestPointOnAABB(XMFLOAT3& _tClosestPoint, const XMFLOAT3& _tPointA, const tAABB& _taabb);

XMFLOAT3 ClosestPointLineSegment(tSegment _tsegment, XMFLOAT3 _tTestPoint);

XMFLOAT3 ClosestPointOnRay(tRay _tray, XMFLOAT3 _tTestPoint);

float ClosestPointSegmentSegment(tSegment _tSegmentA, tSegment _tSegmentB, float& _fDistanceA, float& _fDistanceB, XMFLOAT3& _tClosestPointA, XMFLOAT3& _tClosestPointB);

float Clamp(float _fN, float _fMin, float _fMax);

bool RayPlaneIntersect(float _fPlaneMin, float _fPlaneMax, float _fStart, float _fEnd, float& tbenter, float& tbexit);

bool SegmentToAABB(const tAABB& Box, const XMFLOAT3& _xmStart, const XMFLOAT3& _xmEnd, float& _fThisDist, float _fPriorDist);

