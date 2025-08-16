#include "Subsystem/VFXSubsystem.h"
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
