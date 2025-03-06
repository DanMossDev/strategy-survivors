// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMesh.h"

#include "PoolableComponent.h"

ATileMesh::ATileMesh()
{
	Poolable = CreateDefaultSubobject<UPoolableComponent>("PoolableComponent");
}
