#include "precompiled.h"


efx_api_t efx =
{
	&R_AllocParticle,
	&R_BlobExplosion,
	&R_Blood,
	&R_BloodSprite,
	&R_BloodStream,
	&R_BreakModel,
	&R_Bubbles,
	&R_BubbleTrail,
	&R_BulletImpactParticles,
	&R_EntityParticles,
	&R_Explosion,
	&R_FizzEffect,
	&R_FireField,
	&R_FlickerParticles,
	&R_FunnelSprite,
	&R_Implosion,
	&R_LargeFunnel,
	&R_LavaSplash,
	&R_MultiGunshot,
	&R_MuzzleFlash,
	&R_ParticleBox,
	&R_ParticleBurst,
	&R_ParticleExplosion,
	&R_ParticleExplosion2,
	&R_ParticleLine,
	&R_PlayerSprites,
	&R_Projectile,
	&R_RicochetSound,
	&R_RicochetSprite,
	&R_RocketFlare,
	&R_RocketTrail,
	&R_RunParticleEffect,
	&R_ShowLine,
	&R_SparkEffect,
	&R_SparkShower,
	&R_SparkStreaks,
	&R_Spray,
	&R_Sprite_Explode,
	&R_Sprite_Smoke,
	&R_Sprite_Spray,
	&R_Sprite_Trail,
	&R_Sprite_WallPuff,
	&R_StreakSplash,
	&R_TracerEffect,
	&R_UserTracerParticle,
	&R_TracerParticles,
	&R_TeleportSplash,
	&R_TempSphereModel,
	&R_TempModel,
	&R_DefaultSprite,
	&R_TempSprite,
	&Draw_DecalIndex,
	&Draw_DecalIndexFromName,
	&R_DecalShoot,
	&R_AttachTentToPlayer,
	&R_KillAttachedTents,
	&R_BeamCirclePoints,
	&R_BeamEntPoint,
	&R_BeamEnts,
	&R_BeamFollow,
	&R_BeamKill,
	&R_BeamLightning,
	&R_BeamPoints,
	&R_BeamRing,
	&CL_AllocDlight,
	&CL_AllocElight,
	&CL_TempEntAlloc,
	&CL_TempEntAllocNoModel,
	&CL_TempEntAllocHigh,
	&CL_AllocCustomTempEntity,
	&R_GetPackedColor,
	&R_LookupColor,
	&R_DecalRemoveNonPermanent,
	&R_FireCustomDecal
};