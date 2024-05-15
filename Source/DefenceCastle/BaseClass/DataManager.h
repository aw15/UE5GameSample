#pragma once

#include "CoreMinimal.h"
#include "DefenceCastle/Predefine/PredefineMacro.h"
#include "Engine/DataTable.h"
#include "DataManager.generated.h"
class UDataTable;

enum class EDataType
{
	Unit = 0,
	Skill = 1,
	DrawUnit = 2,
};

UCLASS(Blueprintable)
class UDataManager : public UObject
{
	GENERATED_BODY()
public:
	UDataManager();
	DECLARE_DC_GET(UDataManager)
	UDataTable* GetDataTable(const EDataType DataType);
	template <class T>
	T* FindData(const EDataType DataType, const FName& RowName)
	{
		if(const auto DataTable = GetDataTable(DataType))
		{
			return DataTable->FindRow<T>(RowName, TEXT("Data Find"));	
		}

		return nullptr;
	}


	UPROPERTY(EditAnywhere)
	TArray<UDataTable*> DataTables;
};