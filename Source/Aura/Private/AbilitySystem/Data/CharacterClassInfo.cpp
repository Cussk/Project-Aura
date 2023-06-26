// Copyright Cuss Programming


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	//return Enum and struct key pair
	return CharacterClassInformation.FindChecked(CharacterClass);
}
