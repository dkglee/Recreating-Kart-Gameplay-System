// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventory.h"

#include "Components/Image.h"

void UItemInventory::GetItemUI(class UTexture2D* itemImage)
{
	if (ItemCount == MaxItemCount) return;

	if (ItemCount == 0)
	{
		itemImage0->SetRenderOpacity(1.0f);
		itemImage0->SetBrushFromTexture(itemImage);
		ItemCount++;
	}
	else if (ItemCount == 1)
	{
		itemImage1->SetRenderOpacity(1.0f);
		itemImage1->SetBrushFromTexture(itemImage);
		ItemCount++;
	}
}

void UItemInventory::UseItemUI()
{
	if (ItemCount == 0) return;

	if (ItemCount == 1)
	{
		itemImage0->SetRenderOpacity(0.0f);
		itemImage0->SetBrushFromTexture(nullptr);
		ItemCount--;
	}
	else if (ItemCount == 2)
	{
		itemImage1->SetRenderOpacity(0.0f);
		itemImage0->SetBrush(itemImage1->GetBrush());
		itemImage1->SetBrushFromTexture(nullptr);
		ItemCount--;
	}
}
