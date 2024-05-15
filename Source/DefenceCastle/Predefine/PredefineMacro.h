#pragma once

#define TRACE_TYPE_VISIBILITY ECC_Visibility
#define TRACE_TYPE_MOUSE_SELECT ECC_GameTraceChannel1

#define PrintScreenMacro(text) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, text);
#define PrintOnScreenMacroWithKey(key, text) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(key, 10, FColor::Blue, text);
#define PrintScreenMacro_Int(text, num) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("%s %lld"),text, num));
#define PrintScreenMacro_Float(text, num) if(GEngine)\
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("%s %f"),text, num));

//특정 객체
#define DC_GET(className, variable, worldObject) className* variable = className::Get(worldObject);\
	if(!variable) return;
#define DC_GET_RET(className, variable, worldObject, ReturnValue) className* variable = className::Get(worldObject);\
	if(!variable) return ReturnValue;
#define DECLARE_DC_GET(className) static className* Get(const UObject* const WorldContextObject);
#define DEFINE_DC_GET(className) className* className::Get(const UObject* const WorldContextObject)

//인터페이스 객체
#define DC_GET_INTERFACE(interfaceName, className, variable, worldObject) interfaceName* variable = className::Get(worldObject);\
if(!variable) return;
#define DC_GET_INTERFACE_RET(interfaceName, className, worldObject, ReturnValue) className* variable = className::Get(worldObject);\
if(!variable) return ReturnValue;
#define DECLARE_DC_GET_INTERFACE(interfaceName) static interfaceName* Get(const UObject* const WorldContextObject);
#define DEFINE_DC_GET_INTERFACE(interfaceName, className) interfaceName* className::Get(const UObject* const WorldContextObject)

//Streamable
#define DC_GET_STREAMABLE_MANAGER(worldObject) 	DC_GET(UDcGameInstance, DcGameInstance, worldObject)\
	FStreamableManager& StreamableManager = DcGameInstance->StreamableManager;
#define DC_GET_STREAMABLE_MANAGER_RET(worldObject, ReturnValue) DC_GET_RET(UDcGameInstance, DcGameInstance, worldObject, ReturnValue)\
	FStreamableManager& StreamableManager = DcGameInstance->StreamableManager;

#define INVALID_INDEX -1
#define PLAYER_INDEX 0
#define ENEMY_1_INDEX 1

//Unit BlackboardState
#define STATE_NONE 0
#define STATE_ATTACKING 1
#define STATE_MOVE_TO_LOCATION 2