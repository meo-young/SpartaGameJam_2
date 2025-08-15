#include "Subsystem/SoundSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TableRowBase/BGMTableRowBase.h"
#include "TableRowBase/SFXTableRowBase.h"

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SFXDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sparta/DataTable/DT_SFX"));
	BGMDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Sparta/DataTable/DT_BGM"));

	if (SFXDataTable)
	{
		SFXDataTable->GetAllRows<FSFXTableRowBase>(TEXT(""), SFXDataRows);
		UE_LOG(LogTemp, Log, TEXT("SFX 참조 성공했습니다."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SFX 참조 실패했습니다."));
	}

	if (BGMDataTable)
	{
		BGMDataTable->GetAllRows<FBGMTableRowBase>(TEXT(""), BGMDataRows);
		UE_LOG(LogTemp, Log, TEXT("BGM 참조 성공했습니다."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM 참조 실패했습니다."));
	}
}

void USoundSubsystem::PlaySFX(ESFX SFX)
{
	// ESFX를 SoundCue로 변환합니다.
	USoundCue* SoundCue = SFXDataRows[static_cast<int>(SFX)]->Cue;

	// 2D로 SFX를 재생합니다.
	UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
}

void USoundSubsystem::PlayBGM(EBGM BGM)
{
	// BGM이 재생 중이라면 중단합니다.
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}

	// EBGM을 SoundCue로 변환합니다.
	USoundCue* SoundCue = BGMDataRows[static_cast<int>(BGM)]->Cue;

	// UGameplayStatics를 사용하여 BGM을 재생합니다.
	AudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCue);
	AudioComponent->Play();
}

void USoundSubsystem::StopBGM()
{
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}
}
