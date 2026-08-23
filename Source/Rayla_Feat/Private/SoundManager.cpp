#include "SoundManager.h"

#include "Kismet/GameplayStatics.h"

void USoundManager::PlaySoundByName(FName SoundRowName)
{
	if (!SoundDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundManager: SoundDataTable is nullptr!"));
		return;
	}

	// データテーブルから、指定された名前（SoundRowName）の行を検索する
	static const FString ContextString(TEXT("SoundContext"));
	FSoundData* FoundRow = SoundDataTable->FindRow<FSoundData>(SoundRowName, ContextString);

	if (FoundRow && FoundRow->SoundAsset)
	{
		// 音が見つかったら再生！
		UGameplayStatics::PlaySound2D(GetWorld(), FoundRow->SoundAsset, FoundRow->VolumeMultiplier);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundManager: Sound not found for row %s"), *SoundRowName.ToString());
	}
}