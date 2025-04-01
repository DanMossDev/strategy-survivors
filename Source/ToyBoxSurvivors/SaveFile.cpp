// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFile.h"

USaveFile::USaveFile()
{
	CompletedMilestones = TArray<FGuid>();
	PersistentStats = FSavedStats();
	Volume = 0.5f;
	SFXVolume = 0.5f;
	MusicVolume = 0.5f;
}
