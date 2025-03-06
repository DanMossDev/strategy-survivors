// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveFile.h"

USaveFile::USaveFile()
{
	UnlockedItems = TMap<FString, bool>();

	Volume = 0.5f;
	SFXVolume = 0.5f;
	MusicVolume = 0.5f;
}
