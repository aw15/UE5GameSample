// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"

#include "DcGameInstance.h"
#include "Engine/DataTable.h"

UDataManager::UDataManager()
{
}

DEFINE_DC_GET(UDataManager)
{
	DC_GET_RET(UDcGameInstance, GameInstance, WorldContextObject, nullptr)
	return GameInstance->DataManager;
}

UDataTable* UDataManager::GetDataTable(const EDataType DataType)
{
	return DataTables[static_cast<int>(DataType)];
}
