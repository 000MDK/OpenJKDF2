#ifndef _ENGINE_SITHPHYSICS_H
#define _ENGINE_SITHPHYSICS_H

#include "types.h"
#include "globals.h"

#define sithPhysics_FindFloor_ADDR (0x004F5550)
#define sithPhysics_ThingPhysIdk_inlined_ADDR (0x004F5870)
#define sithPhysics_ThingTick_ADDR (0x004F5900)
#define sithPhysics_ThingApplyForce_ADDR (0x004F59B0)
#define sithPhysics_ThingSetLook_ADDR (0x004F5A80)
#define sithPhysics_ApplyDrag_ADDR (0x004F5D50)
#define sithPhysics_LoadThingParams_ADDR (0x004F5EC0)
#define sithPhysics_ThingStop_ADDR (0x004F61A0)
#define sithPhysics_ThingGetInsertOffsetZ_ADDR (0x004F6210)
#define sithPhysics_ThingPhysGeneral_ADDR (0x004F6270)
#define sithPhysics_ThingPhysPlayer_ADDR (0x004F6860)
#define sithPhysics_ThingPhysUnderwater_ADDR (0x004F6D80)
#define sithPhysics_ThingPhysAttached_ADDR (0x004F7430)

void sithPhysics_FindFloor(sithThing *thing, int a3);
void sithPhysics_ThingTick(sithThing *thing, float force);
void sithPhysics_ThingApplyForce(sithThing *thing, rdVector3 *forceVec);
void sithPhysics_ThingSetLook(sithThing *thing, const rdVector3 *look, float a3);
void sithPhysics_ApplyDrag(rdVector3 *vec, float drag, float mag, float dragCoef);
int sithPhysics_LoadThingParams(stdConffileArg *arg, sithThing *thing, int param);
void sithPhysics_ThingStop(sithThing *thing);
float sithPhysics_ThingGetInsertOffsetZ(sithThing *thing);
void sithPhysics_ThingPhysGeneral(sithThing *thing, float deltaSeconds);
void sithPhysics_ThingPhysPlayer(sithThing *player, float deltaSeconds);
void sithPhysics_ThingPhysUnderwater(sithThing *thing, float deltaSeconds);
void sithPhysics_ThingPhysAttached(sithThing *thing, float deltaSeconds);

static void (*_sithPhysics_ThingPhysAttached)(sithThing *thing, float deltaSeconds) = (void*)sithPhysics_ThingPhysAttached_ADDR;

#endif // _ENGINE_SITHPHYSICS_H