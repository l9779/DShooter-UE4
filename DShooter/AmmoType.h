#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_PistolBullet UMETA(DisplayName = "Pistol Bullet"),
	EAT_RifleBullet UMETA(DisplayName = "Rifle Bullet"),
	EAT_ShotgunBullet UMETA(DisplayName = "Shotgun Bullet"),

	EAT_MAX UMETA(DisplayName = "DefaultMax")
};