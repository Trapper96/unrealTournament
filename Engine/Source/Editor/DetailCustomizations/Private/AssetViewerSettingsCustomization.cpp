// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AssetViewerSettingsCustomization.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "UObject/UnrealType.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailCategoryBuilder.h"
#include "AssetViewerSettings.h"

#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "AssetViewerSettingsCustomizations"

TSharedRef<IDetailCustomization> FAssetViewerSettingsCustomization::MakeInstance()
{
	return MakeShareable(new FAssetViewerSettingsCustomization);
}

void FAssetViewerSettingsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	ViewerSettings = UAssetViewerSettings::Get();

	// Find profiles array property handle and hide it from settings view
	TSharedPtr<IPropertyHandle> ProfileHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UAssetViewerSettings, Profiles));
	check(ProfileHandle->IsValidHandle());
	ProfileHandle->MarkHiddenByCustomization();

	// Create new category
	IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory("Settings", LOCTEXT("AssetViewerSettingsCategory", "Settings"));

	const UEditorPerProjectUserSettings* PerProjectUserSettings = GetDefault<UEditorPerProjectUserSettings>();
	ProfileIndex = ViewerSettings->Profiles.IsValidIndex(PerProjectUserSettings->AssetViewerProfileIndex) ? PerProjectUserSettings->AssetViewerProfileIndex : 0;

	ensureMsgf(ViewerSettings && ViewerSettings->Profiles.IsValidIndex(PerProjectUserSettings->AssetViewerProfileIndex), TEXT("Invalid default settings pointer or current profile index"));

	// Add current active profile property child nodes (rest of profiles remain hidden)
	TSharedPtr<IPropertyHandle> ProfilePropertyHandle = ProfileHandle->GetChildHandle(ProfileIndex);
	checkf(ProfileHandle->IsValidHandle(), TEXT("Invalid indexing into profiles child properties"));	
	uint32 PropertyCount = 0;
	ProfilePropertyHandle->GetNumChildren(PropertyCount);

	FName NamePropertyName = GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, ProfileName);
	for (uint32 PropertyIndex = 0; PropertyIndex < PropertyCount; ++PropertyIndex)
	{
		TSharedPtr<IPropertyHandle> ProfileProperty = ProfilePropertyHandle->GetChildHandle(PropertyIndex);
				
		if (ProfileProperty->GetProperty()->GetFName() == NamePropertyName)
		{
			NameProperty = ProfileProperty;
			CategoryBuilder.AddCustomRow(LOCTEXT("PreviewSceneProfileDetails_ProfileNameLabel", "Profile Name"))
					.NameContent()
					[
						ProfileProperty->CreatePropertyNameWidget()
					]
					.ValueContent()
					.MaxDesiredWidth(250.0f)
					[
						// PropertyHandle->CreatePropertyValueWidget()
						SAssignNew(NameEditTextBox, SEditableTextBox)
						.Text(this, &FAssetViewerSettingsCustomization::OnGetProfileName)
						//.ToolTip(VarNameTooltip)
						.OnTextChanged(this, &FAssetViewerSettingsCustomization::OnProfileNameChanged)
						.OnTextCommitted(this, &FAssetViewerSettingsCustomization::OnProfileNameCommitted)
						.Font(IDetailLayoutBuilder::GetDetailFont())
					];
		}
		else
		{
			IDetailPropertyRow& Row = CategoryBuilder.AddProperty(ProfileProperty);
		}
	}
}

FText FAssetViewerSettingsCustomization::OnGetProfileName() const
{
	return FText::FromString(ViewerSettings->Profiles[ProfileIndex].ProfileName);
}

void FAssetViewerSettingsCustomization::OnProfileNameChanged(const FText& InNewText)
{	
	bValidProfileName = IsProfileNameValid(InNewText.ToString());
	if (!bValidProfileName)
	{
		NameEditTextBox->SetError(LOCTEXT("PreviewSceneProfileName_NotValid", "This name is already in use"));
	}
	else
	{
		NameEditTextBox->SetError(FText::GetEmpty());
	}
}

void FAssetViewerSettingsCustomization::OnProfileNameCommitted(const FText& InNewText, ETextCommit::Type InTextCommit)
{
	if (bValidProfileName && InTextCommit != ETextCommit::OnCleared)
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameProfile", "Rename Profile"));
		ViewerSettings->Modify();

		ViewerSettings->Profiles[ProfileIndex].ProfileName = InNewText.ToString();		
		FPropertyChangedEvent PropertyEvent(NameProperty->GetProperty());
		ViewerSettings->PostEditChangeProperty(PropertyEvent);
	}

	bValidProfileName = false;
	NameEditTextBox->SetError(FText::GetEmpty());
}

const bool FAssetViewerSettingsCustomization::IsProfileNameValid(const FString& NewName)
{
	for (int32 CheckProfileIndex = 0; CheckProfileIndex < ViewerSettings->Profiles.Num(); ++CheckProfileIndex)
	{
		if (ProfileIndex != CheckProfileIndex && ViewerSettings->Profiles[CheckProfileIndex].ProfileName == NewName)
		{
			return false;
		}
	}

	return true;
}

#undef LOCTEXT_NAMESPACE 
