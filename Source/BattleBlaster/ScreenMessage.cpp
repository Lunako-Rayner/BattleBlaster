// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	if (MessageTextBlock)
	{
		//in order to convert FStrig to FText(which uses different data type)
		MessageTextBlock->SetText(FText::FromString(Message));
	}
	if (HealthTextBlock)
	{
		
		HealthTextBlock->SetText(FText::FromString(Message));
	}
}
