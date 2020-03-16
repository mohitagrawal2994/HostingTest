// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HOSTINGTEST_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

	//Widget Related Variables
	class UUserWidget* MyMainMenu;
	bool MenuWidgetOpen;

protected:
	virtual void SetupInputComponent();

	class UInputComponent* PlayerInputComponent;

	class UMyGameInstance* MyCusGameInstance;


public:
	//Public Variable to set the current main menu
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> wMyMainMenu;

	ACPlayerController();

	virtual void BeginPlay();

	//Widget Functions
	UFUNCTION()
	void AddWidget();

	UFUNCTION()
	void OpenMenu();

};
