// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOSTINGTEST_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

    class IOnlineSubsystem* MyOnlineSub;
    IOnlineSessionPtr Sessions;
    class FOnlineSessionSettings* CustomSessionSettings;
    TSharedPtr<class FOnlineSessionSearch> ListOfSessions;
    bool SessionFiringStatus;

    FString CurrentDeviceID;
    class UUserWidget* CurrentMainMenu;
    class UUserWidget* ServerWidget;

    UPROPERTY(meta = (BindWidget))
    class UPanelWidget* ServerListPanel;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ServerTextPanel;

protected:
    //Array to store names of the current servers hosting a game
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    TArray<FString> ServerNamesList;

public:

    //Public Variable to set the current main menu
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<class UUserWidget> wServerWidget;

    //Storing the name of levels
    UPROPERTY(EditDefaultsOnly, Category = "Level")
    FName LobbyMapName;

    UPROPERTY(EditDefaultsOnly, Category = "Level")
    FName GameMapName;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    FName ServerListWidgetName;

    UMyGameInstance();

    virtual void Init();

    UFUNCTION(BlueprintCallable)
    void CreateGameSession();

    UFUNCTION(BlueprintCallable)
    void SearchGameSession();

    UFUNCTION(BlueprintCallable)
    void JoinGameSession();

    UFUNCTION(BlueprintCallable)
    void ExitGameSession();

    UFUNCTION()
    void DestroyGameSession(FString UserName);

    UFUNCTION()
    void CreatingSessionSuccessful(FName SessionName, bool Success);

    UFUNCTION()
    void SearchingSessionSuccessful(bool Success);

    void JoiningSessionSuccessful(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    UFUNCTION()
    void DestroyingSessionSucessful(FName SessionName, bool Success);

    UFUNCTION()
    void SetMenuWidget(class UUserWidget* MainMenu);

    UFUNCTION()
    void SetServerList();

};
