#include "Subsystem/VFXSubsystem.h"
#include "NiagaraFunctionLibrary.h"
#include "VFXTableRowBase.h"

void UVFXSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	VFXDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sparta/DataTable/DT_VFX"));
	if (VFXDataTable)
	{
		VFXDataTable->GetAllRows<FVFXTableRowBase>(TEXT(""), VFXDataTableRows);
		UE_LOG(LogTemp, Log, TEXT("VFX 참조 성공했습니다."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VFX 참조 실패했습니다."));
	}
}

void UVFXSubsystem::SpawnVFX(EVFX VFX, const FVector SpawnLocation)
{
	if (UNiagaraSystem* NiagaraSystem =	VFXDataTableRows[static_cast<int>(VFX)]->NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, SpawnLocation, FRotator::ZeroRotator);
	}
}
