// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	if (StartBtn)
	{
		StartBtn->OnClicked.AddDynamic(this, &UMainMenu::OnStartClick);
	}

	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenu::OnQuitClick);
	}
}

void UMainMenu::OnStartClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UGameplayStatics::OpenLevel(world, TEXT("MainLevel"));
	}
}

void UMainMenu::OnQuitClick()
{
	UWorld* world = GetWorld();

	if (world)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(world, TEXT("quit"));
	}
}
