// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentData.h"

#include "Milestone.h"

#if WITH_EDITOR
void UPersistentData::RefreshAllMilestoneGuids()
{
	for (auto milestone : Milestones)
		milestone->RefreshGUID();
}

#endif