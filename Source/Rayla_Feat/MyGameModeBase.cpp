#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase()
{
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

bool AMyGameModeBase::CheckCollision(ABP_UnitBase* UnitA, ABP_UnitBase* UnitB) const
{
	if (!UnitA || !UnitB) return false;
	return (UnitA->GridX == UnitB->GridX) && (UnitA->GridY == UnitB->GridY);
}

ABP_UnitBase* AMyGameModeBase::FindRangeTarget(ABP_UnitBase* Unit, const TArray<AActor*>& AllUnits) const
{
	if (!Unit) return nullptr;

	// AttackRangePriority‚ÍŠù‚É—Dæ‡ˆÊ‡‚É•À‚ñ‚Å‚¢‚é‚Ì‚ÅAæ“ª‚©‚çŒ©‚ÄÅ‰‚É“G‚ªŒ©‚Â‚©‚Á‚½“_‚Å•Ô‚·
	for (const FIntPoint& Pos : Unit->AttackRangePriority)
	{
		for (AActor* OtherActor : AllUnits)
		{
			ABP_UnitBase* Other = Cast<ABP_UnitBase>(OtherActor);
			if (!Other || Other == Unit) continue;

			// ©w‰c‚ÍUŒ‚‘ÎÛ‚É‚µ‚È‚¢
			if (Other->PlayerSide == Unit->PlayerSide) continue;

			if (Other->GridX == Pos.X && Other->GridY == Pos.Y)
			{
				return Other;
			}
		}
	}
	return nullptr;
}

TArray<FBattlePair> AMyGameModeBase::DetermineBattlePairs(
	AActor* P1MovedUnitActor,
	AActor* P2MovedUnitActor,
	const TArray<AActor*>& AllUnits
)
{
	TArray<FBattlePair> Result;

	ABP_UnitBase* P1Unit = Cast<ABP_UnitBase>(P1MovedUnitActor);
	ABP_UnitBase* P2Unit = Cast<ABP_UnitBase>(P2MovedUnitActor);

	// ƒP[ƒXA:—¼•ûˆÚ“®‚µ‚½ê‡
	if (P1Unit && P2Unit)
	{
		// ‡@Õ“Ë”»’è
		if (CheckCollision(P1Unit, P2Unit))
		{
			FBattlePair Pair;
			Pair.UnitA = P1Unit;
			Pair.UnitB = P2Unit;
			Result.Add(Pair);
			return Result; // Õ“Ë‚ªÅ—DæB‚±‚ê‚ÅŠm’è
		}

		// ‡AUŒ‚”ÍˆÍ”»’è
		ABP_UnitBase* P1Target = FindRangeTarget(P1Unit, AllUnits);
		ABP_UnitBase* P2Target = FindRangeTarget(P2Unit, AllUnits);

		bool bP1TargetsP2 = (P1Target == P2Unit);
		bool bP2TargetsP1 = (P2Target == P1Unit);

		// ‚¨Œİ‚¢‚ğ‘_‚¢‡‚Á‚Ä‚¢‚é‚¾‚¯‚È‚çB–†‚³‚È‚µ
		if (bP1TargetsP2 && bP2TargetsP1)
		{
			FBattlePair Pair;
			Pair.UnitA = P1Unit;
			Pair.UnitB = P2Unit;
			Result.Add(Pair);
			return Result;
		}

		// ‡BB–†”»’è:P1‚ª“G‚ÌˆÚ“®Unit‚ğ‘_‚¢AP2‚ÍŒ³‚©‚ç‚¢‚½Unit‚ğ‘_‚Á‚Ä‚¢‚é(‚©‚ÂA‚»‚ÌŒ³‚©‚ç‚¢‚½Unit‚ÌUŒ‚”ÍˆÍ‚É‚àP1ˆÚ“®Unit‚ª“ü‚Á‚Ä‚¢‚é)ƒP[ƒX
		bool bAmbiguous = false;
		if (P1Target && P2Target && P1Target != P2Unit && P2Target != P1Unit)
		{
			// P1Target‚Ì‹“_‚Å‚àAP1Unit‚âP2Unit‚ğŒ©‚Ä‚¢‚é‚©Šm”F(‘ŠŒİ‚É—‚İ‡‚Á‚Ä‚¢‚é‚©)
			ABP_UnitBase* P1TargetCounter = FindRangeTarget(P1Target, AllUnits);
			ABP_UnitBase* P2TargetCounter = FindRangeTarget(P2Target, AllUnits);

			if (P1TargetCounter == P1Unit && P2TargetCounter == P2Unit)
			{
				bAmbiguous = true;
			}
		}

		if (bAmbiguous)
		{
			// ƒ‰ƒ“ƒ_ƒ€‚Å‚Ç‚¿‚ç‚Ì‘g‚İ‡‚í‚¹‚É‚·‚é‚©Œˆ’è
			if (FMath::RandBool())
			{
				FBattlePair Pair;
				Pair.UnitA = P1Unit;
				Pair.UnitB = P2Unit;
				Result.Add(Pair);
			}
			else
			{
				FBattlePair Pair1;
				Pair1.UnitA = P1Unit;
				Pair1.UnitB = P2Target;
				Result.Add(Pair1);

				FBattlePair Pair2;
				Pair2.UnitA = P2Unit;
				Pair2.UnitB = P1Target;
				Result.Add(Pair2);
			}
			return Result;
		}

		// B–†‚Å‚È‚¯‚ê‚ÎA‚»‚ê‚¼‚ê‚ÌŒ‹‰Ê‚ğ‚»‚Ì‚Ü‚Ü”½‰f
		if (P1Target)
		{
			FBattlePair Pair;
			Pair.UnitA = P1Unit;
			Pair.UnitB = P1Target;
			Result.Add(Pair);
		}
		if (P2Target)
		{
			FBattlePair Pair;
			Pair.UnitA = P2Unit;
			Pair.UnitB = P2Target;
			Result.Add(Pair);
		}
		return Result;
	}

	// ƒP[ƒXB:•Ğ•û‚¾‚¯ˆÚ“®‚µ‚½ê‡
	if (P1Unit)
	{
		ABP_UnitBase* Target = FindRangeTarget(P1Unit, AllUnits);
		if (Target)
		{
			FBattlePair Pair;
			Pair.UnitA = P1Unit;
			Pair.UnitB = Target;
			Result.Add(Pair);
		}
	}
	if (P2Unit)
	{
		ABP_UnitBase* Target = FindRangeTarget(P2Unit, AllUnits);
		if (Target)
		{
			FBattlePair Pair;
			Pair.UnitA = P2Unit;
			Pair.UnitB = Target;
			Result.Add(Pair);
		}
	}

	return Result;
}