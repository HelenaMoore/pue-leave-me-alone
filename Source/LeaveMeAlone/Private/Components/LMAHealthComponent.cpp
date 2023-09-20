// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "Components/LMAHealthComponent.h"

ULMAHealthComponent::ULMAHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool ULMAHealthComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool ULMAHealthComponent::AddHealth(float NewHealth)
{
	if (!IsAlive() || IsHealthFull()) return false;
	Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	return true;
}

bool ULMAHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);

	AActor* OwnerComponent = GetOwner();
	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
}

void ULMAHealthComponent::OnTakeAnyDamage(AActor* DamageActor, float Damage, const class UDamageType* DamageType,
										  class AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsAlive()) return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (!IsAlive())
	{
		OnDeath.Broadcast();
	}
}

