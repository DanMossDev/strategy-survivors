// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectionDisplayer.h"

ACharacterSelectionDisplayer::ACharacterSelectionDisplayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterSelectionDisplayer::ShowNewCharacter(UPlayableCharacter* Character)
{
	SetAllChildrenVisible(CharacterMeshLookup[CurrentlySelectedCharacter], false);
	CurrentlySelectedCharacter = Character;
	SetAllChildrenVisible(CharacterMeshLookup[CurrentlySelectedCharacter], true);
}

void ACharacterSelectionDisplayer::AddToCharacterMeshLookup(UPlayableCharacter* Character, UStaticMeshComponent* MeshComponent)
{
	CharacterMeshLookup.Add(Character, MeshComponent);

	SetAllChildrenVisible(MeshComponent, Character == CurrentlySelectedCharacter);
}

void ACharacterSelectionDisplayer::SetAllChildrenVisible(UStaticMeshComponent* MeshComponent, bool Visible)
{
	MeshComponent->SetHiddenInGame(!Visible);

	TArray<USceneComponent*> children;
	MeshComponent->GetChildrenComponents(true, children);

	for (auto child : children)
		child->SetHiddenInGame(!Visible);
}
