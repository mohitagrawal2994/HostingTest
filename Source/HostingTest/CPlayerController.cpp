// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
//#include "Kismet/GameplayStatics.h"

ACPlayerController::ACPlayerController()
{
	MyCusGameInstance = NULL;
	MenuWidgetOpen = false;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//MyCusGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));
	MyCusGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	AddWidget();
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	PlayerInputComponent = NewObject<UInputComponent>(this, TEXT("PC_InputComponent0"));
	PlayerInputComponent->RegisterComponent();

	PlayerInputComponent->BindAction("OpenMenu", IE_Pressed, this, &ACPlayerController::OpenMenu);
}

void ACPlayerController::AddWidget()
{
	if (wMyMainMenu)
	{
		MyMainMenu = CreateWidget<UUserWidget>(this, wMyMainMenu);
		if (MyMainMenu)
		{
			MyMainMenu->AddToViewport();
			MyMainMenu->SetVisibility(ESlateVisibility::Hidden);
			MyCusGameInstance->SetMenuWidget(MyMainMenu);
		}
	}

}

void ACPlayerController::OpenMenu()
{
	if (MenuWidgetOpen && (MyMainMenu != NULL))
	{
		MyMainMenu->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		MenuWidgetOpen = false;
		SetInputMode(FInputModeGameOnly());

	}
	else
	{
		MyMainMenu->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
		MenuWidgetOpen = true;
		SetInputMode(FInputModeGameAndUI());
	}
}
