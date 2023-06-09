// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOver.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOver::NativeConstruct()
{
	if (MainMenuBtn)
	{
		MainMenuBtn->OnClicked.AddDynamic(this, &UGameOver::OnMainMenuClick);
	}

	if (RestartBtn)
	{
		RestartBtn->OnClicked.AddDynamic(this, &UGameOver::OnRestartClick);
	}
}

void UGameOver::OnMainMenuClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UGameplayStatics::OpenLevel(world, TEXT("MainMenu"));
	}
}

void UGameOver::OnRestartClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(world, TEXT("RestartLevel"));
	}
}
