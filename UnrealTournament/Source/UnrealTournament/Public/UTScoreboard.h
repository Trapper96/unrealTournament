// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 

#include "UTHUDWidget.h"
#include "UTScoreboard.generated.h"

USTRUCT()
struct FSelectionObject
{
	GENERATED_USTRUCT_BODY()

	// Holds a reference to PS that is under this score element
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scoreboard")
	TWeakObjectPtr<AUTPlayerState> ScoreOwner;

	// Holds the X1/Y1/X2/Y2 bounds of this score element.  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Scoreboard")
	FVector4 ScoreBounds;

	FSelectionObject()
	{
		ScoreOwner.Reset();
		ScoreBounds = FVector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	FSelectionObject(TWeakObjectPtr<AUTPlayerState> inScoreOwner, const FVector4& inScoreBounds)
	{
		ScoreOwner = inScoreOwner;
		ScoreBounds = inScoreBounds;
	}

};

USTRUCT()
struct FStatsFontInfo
{
	GENERATED_USTRUCT_BODY()

	FFontRenderInfo TextRenderInfo;
	float TextHeight;
	UFont* TextFont;
};

UCLASS()
class UNREALTOURNAMENT_API UUTScoreboard : public UUTHUDWidget
{
	GENERATED_UCLASS_BODY()

public:
	// The main drawing stub
	virtual void Draw_Implementation(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnHeaderPlayerX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnHeaderScoreX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnHeaderDeathsX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnHeaderPingX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnHeaderY;

	// The offset of text data within the cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnY;

	// The total Height of a given cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float CellHeight;

	// How much space in between each column
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float CenterBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float ColumnMedalX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float FooterPosY;

	// Where to draw the minimap center
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FVector2D MinimapCenter;

	// Where to draw the flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	float FlagX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scoreboard")
		USoundBase* ScoreUpdateSound;

	UPROPERTY()
		float ScaledEdgeSize;

	UPROPERTY()
		float ScaledCellWidth;

	virtual void PreDraw(float DeltaTime, AUTHUD* InUTHUDOwner, UCanvas* InCanvas, FVector2D InCanvasCenter);

	virtual bool ShouldDrawScoringStats() { return false; };

	/** Show current 2 pages of scoring stats breakdowns. */
	virtual void DrawScoringStats(float RenderDelta, float& YOffset);

	virtual void DrawStatsLeft(float DeltaTime, float& YPos, float XOffset, float ScoreWidth, float PageBottom);
	virtual void DrawStatsRight(float DeltaTime, float& YPos, float XOffset, float ScoreWidth, float PageBottom);

	virtual FLinearColor GetPlayerColorFor(AUTPlayerState* InPS) const;
	virtual FString GetPlayerNameFor(AUTPlayerState* InPS) const
	{
		return InPS ? InPS->PlayerName : "";
	};

	UPROPERTY()
		TArray<FText> StatsPageTitles;

	UPROPERTY()
		TArray<FText> StatsPageFooters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText GameMessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText ScoreMessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Kills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Deaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Ping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText CH_Ready;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText OneSpectatorWatchingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText SpectatorsWatchingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText PingFormatText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText PositionFormatText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText TeamSwapText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText ReadyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText NotReadyText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText ArrowKeysText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		FText MinimapToggleText;

	FInputActionKeyMapping MinimapBinding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		bool bDrawMinimapInScoreboard;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Scoreboard")
	int32 ActualPlayerCount;

	virtual AUTPlayerState* GetNextScoringPlayer(int32 dir, int32& PSIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
	UTexture2D* FlagAtlas;

	virtual bool ShouldDraw_Implementation(bool bShowScores)
	{
		return bShowScores;
	}

	virtual void DrawGamePanel(float RenderDelta, float& YOffset);
	virtual void DrawGameOptions(float RenderDelta, float& YOffset);

	virtual void DrawTeamPanel(float RenderDelta, float& YOffset);

	virtual void DrawScorePanel(float RenderDelta, float& YOffset);
	virtual void DrawScoreHeaders(float RenderDelta, float& DrawY);
	virtual void DrawPlayerScores(float RenderDelta, float& DrawY);
	virtual void DrawPlayer(int32 Index, AUTPlayerState* PlayerState, float RenderDelta, float XOffset, float YOffset);
	virtual void DrawPlayerScore(AUTPlayerState* PlayerState, float XOffset, float YOffset, float Width, FLinearColor DrawColor);
	virtual void DrawReadyText(AUTPlayerState* PlayerState, float XOffset, float YOffset, float Width);

	virtual void DrawServerPanel(float RenderDelta, float YOffset);

	/** Color to display ready text. */
	FLinearColor ReadyColor;

	/** Scale to display ready text. */
	float ReadyScale;

public:
	virtual bool IsInteractive() { return bIsInteractive; };
	virtual TWeakObjectPtr<AUTPlayerState> GetSelectedPlayer();
protected:

	// Will be true when the scoreboard is interactive.  This will cause the
	// scoreboard to track the SelectionStack and display selected items.
	bool bIsInteractive;

	// Holds a list of selectable objects on the scoreboard.  NOTE: this is regenerated every frame
	TArray<FSelectionObject> SelectionStack;

	int32 SelectionHitTest(FVector2D Position);

	FVector2D CursorPosition;

	TWeakObjectPtr<AUTPlayerState> SelectedPlayer;

	//-------------------------------------
	// Scoreboard stats breakdown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float ValueColumn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float ScoreColumn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float KillsColumn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float DeathsColumn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float ShotsColumn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard")
		float AccuracyColumn;

	UPROPERTY()
		bool bHighlightStatsLineTopValue;

	/** List of default weapons to display stats for. */
	UPROPERTY()
	TArray<AUTWeapon *> StatsWeapons;

	/** Index of current top weapon (in kills). */
	UPROPERTY()
		int32 BestWeaponIndex;

	/** Draw one line of scoring breakdown. */
	virtual void DrawStatsLine(FText StatsName, int32 StatValue, int32 ScoreValue, float DeltaTime, float XOffset, float& YPos, const FStatsFontInfo& StatsFontInfo, float ScoreWidth);

	/** Draw one line of scoring breakdown where values are string instead of int32 */
	virtual void DrawTextStatsLine(FText StatsName, FString StatValue, FString ScoreValue, float DeltaTime, float XOffset, float& YPos, const FStatsFontInfo& StatsFontInfo, float ScoreWidth, int32 HighlightIndex);

	/** Draw one line of scoring breakdown where values are PlayerStates instead of int32 */
	virtual void DrawPlayerStatsLine(FText StatsName, AUTPlayerState* FirstPlayer, AUTPlayerState* SecondPlayer, float DeltaTime, float XOffset, float& YPos, const FStatsFontInfo& StatsFontInfo, float ScoreWidth, int32 HighlightIndex);

	//-------------------------------------

public:

	void BecomeInteractive();
	void BecomeNonInteractive();

	// Each time a mouse is moved, then 
	virtual void TrackMouseMovement(FVector2D NewMousePosition);

	// Attempts to select an item in the stack.  Returns true and flags the PS if it succeeds or false and clears all selections if fails.
	virtual bool AttemptSelection(FVector2D SelectionPosition);

	// Clears any selected PS
	virtual void ClearSelection();

	// Finds next selected PRI given an offset
	virtual void SelectNext(int32 Offset, bool bDoNoWrap=false);
	virtual void DefaultSelection(AUTGameState* GS, uint8 TeamIndex = 255);

	virtual void SelectionUp();
	virtual void SelectionDown();
	virtual void SelectionLeft();
	virtual void SelectionRight();
	virtual void SelectionClick();

	virtual float GetDrawScaleOverride();

};

