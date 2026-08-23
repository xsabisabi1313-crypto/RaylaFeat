
#include "BaseBuilding.h"
#include "GameManager.h"                // GameManagerの中身（winnerやEWinner）を使うため
#include "Kismet/GameplayStatics.h"     // UGameplayStaticsを使うため
#include "Engine/World.h"

// Sets default values
ABaseBuilding::ABaseBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance.
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseBuilding::TakeBuildingDamage(int32 DamageAmount)
{
	HP -= DamageAmount;

    if (HP <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("拠点が破壊されました！"));
        //Destroy(); // 必要に応じてコメントアウトを解除

        AGameManager* GameManager = Cast<AGameManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
        );
        if (!GameManager) return;

        // プレイヤー側が壊されたということは、敵の勝ち（あるいはその逆）
        // ※ どちらの拠点が破壊されたかによって勝者を判定します
        if (PlayerSide == EPlayerSide::Player)
        {
            GameManager->winner = EWinner::Enemy; // プレイヤーの拠点HP0＝敵の勝ち
        }
        else
        {
            GameManager->winner = EWinner::Player; // 敵の拠点HP0＝プレイヤーの勝ち
        }
    }
}