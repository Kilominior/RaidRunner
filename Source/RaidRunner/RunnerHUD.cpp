// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerHUD.h"

void ARunnerHUD::DrawHUD()
{
    Super::DrawHUD();

    // 绘制十字准星
    if (CrosshairTexture)
    {
        // 获取画布中心点
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // 将十字准星偏移以对齐中心点
        FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

        // 以半透明混合模式绘制十字准星
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}
