// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenu::NativeConstruct()
{
	if (ContinueBtn)
	{
		ContinueBtn->OnClicked.AddDynamic(this, &UPauseMenu::OnContinueClick);
	}

	if (RestartBtn) 
	{
		RestartBtn->OnClicked.AddDynamic(this, &UPauseMenu::OnRestartClick);
	}

	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UPauseMenu::OnQuitClick);
	}
}

void UPauseMenu::OnContinueClick()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();
}

void UPauseMenu::OnRestartClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(world, TEXT("RestartLevel"));
	}
}

void UPauseMenu::OnQuitClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(world, TEXT("quit"));
	}
}
