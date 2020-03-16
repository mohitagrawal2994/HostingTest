// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PanelWidget.h"
#include "GenericPlatform/GenericPlatformMisc.h"

//A FName used to set key during Session Creation so can be used to find sessions later with name
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UMyGameInstance::UMyGameInstance()
{
	SessionFiringStatus = false;
	LobbyMapName = "Level01";
	GameMapName = "Level02";
	CurrentDeviceID = "";
	CurrentMainMenu = NULL;
	ServerListWidgetName = "ServerList";
}

void UMyGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init Func"));

	//Find the type of subsystem we are interacting with NULL or Steam, etc
	MyOnlineSub = IOnlineSubsystem::Get();
	Sessions = MyOnlineSub->GetSessionInterface();

	//If interface is found then do binding of functions that get called when Creation,Finding, Joining And Deletion of Session happens
	if (Sessions.IsValid())
	{
		Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::CreatingSessionSuccessful);
		Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::SearchingSessionSuccessful);
		//Sessions->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::JoiningSessionSuccessful);
		Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyGameInstance::DestroyingSessionSucessful);

		CurrentDeviceID = FGenericPlatformMisc::GetDeviceId();

	}
}

void UMyGameInstance::CreateGameSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Lobby"));
	//Check is session with same name exists, if true then destroy that session
	if((Sessions->GetNamedSession(FName(*CurrentDeviceID))) != NULL)
	{
		DestroyGameSession(CurrentDeviceID);
	}

	CustomSessionSettings = new FOnlineSessionSettings();
	CustomSessionSettings->bIsLANMatch = true;
	CustomSessionSettings->NumPublicConnections = 5;
	CustomSessionSettings->bShouldAdvertise = true;

	//CustomSessionSettings->bUsesPresence = true;
	//CustomSessionSettings->NumPrivateConnections = 0;
	//CustomSessionSettings->bAllowInvites = true;
	//CustomSessionSettings->bAllowJoinInProgress = true;
	//CustomSessionSettings->bAllowJoinViaPresence = true;
	//CustomSessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

	CustomSessionSettings->Set(SERVER_NAME_SETTINGS_KEY, CurrentDeviceID, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionFiringStatus = Sessions->CreateSession(0, FName(*CurrentDeviceID), *CustomSessionSettings);
}


void UMyGameInstance::SearchGameSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Finding Lobby"));

	ListOfSessions = MakeShareable(new FOnlineSessionSearch());
	if (ListOfSessions.IsValid())
	{
		ListOfSessions->MaxSearchResults = 10;
		SessionFiringStatus = Sessions->FindSessions(0, ListOfSessions.ToSharedRef());
	}
}

void UMyGameInstance::JoinGameSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Joining Lobby"));

	//Join the lobby selected by client
}

void UMyGameInstance::ExitGameSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Exiting Lobby"));

	//Find the current game session the game belongs to
	//Destroy the session
	//Client travel to main menu
}

void UMyGameInstance::DestroyGameSession(FString UserName)
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying Lobby"));
	if ((Sessions->GetNamedSession(FName(*UserName))) != NULL)
	{
		SessionFiringStatus = Sessions->DestroySession(FName(*UserName));
		UE_LOG(LogTemp, Warning, TEXT("Destroying Lobby %d"), SessionFiringStatus);
	}
	
}

void UMyGameInstance::CreatingSessionSuccessful(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Was Created with name %s"), *SessionName.ToString());
		UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
	}
}

void UMyGameInstance::SearchingSessionSuccessful(bool Success)
{
	if (Success && ListOfSessions.IsValid())
	{
		ServerNamesList.Empty();
		for (const FOnlineSessionSearchResult &SearchResult : ListOfSessions->SearchResults)
		{
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found session with name %s"), *ServerName);
				ServerNamesList.Add(ServerName);
			}
		}
		SetServerList();
	}

}

//void UMyGameInstance::JoiningSessionSuccessful(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
//{
//}


void UMyGameInstance::DestroyingSessionSucessful(FName SessionName, bool Success)
{
}

void UMyGameInstance::SetMenuWidget(UUserWidget* MainMenu)
{
	CurrentMainMenu = MainMenu;
	ServerList = Cast<UPanelWidget>(CurrentMainMenu->WidgetTree->FindWidget(ServerListWidgetName));
	
}

void UMyGameInstance::SetServerList()
{
	UE_LOG(LogTemp, Warning, TEXT("Adding Name to server List"));
	if (ServerList)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got the area "));
	}
}
