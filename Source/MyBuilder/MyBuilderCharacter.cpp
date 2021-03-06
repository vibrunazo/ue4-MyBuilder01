// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "MyBuilderCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
// #include "GameplayTagContainer.h"
// #include "GameplayTags.h"

//////////////////////////////////////////////////////////////////////////
// AMyBuilderCharacter

AMyBuilderCharacter::AMyBuilderCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	// BaseTurnRate = 45.f;
	BaseTurnRate = 180.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Our ability system component.
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSetBase = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSetBase"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_MyDefaultPawn"));
	DefaultPawnClass = PlayerPawnBPClass.Class;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AMyBuilderCharacter::BeginPlay()
{
	if(AbilitySystem)
	{
    	for (auto &&Ability : Abilities)
		{
			GiveAbilityWithInput(Ability.AbilityClass, Ability.Input);
		}
	}
	SetupStatsFromGameInstance();
	Super::BeginPlay();
}

void AMyBuilderCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!InputEnabled()) return;
	if (GetAbilityKeyDown(0)) ActivateAbilityByInput(0);
	// AbilitySystem->TryActivateAbilityByClass(Abilities[0].Ability, true);
}

void AMyBuilderCharacter::SetupStatsFromGameInstance()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (!ensure(GI != nullptr)) return;
	if (GI->bInitialized && IsPlayerControlled())
	{
		AttributeSetBase->SetHealth(GI->PlayerHealth);
	}
	if (!GI->bInitialized && IsPlayerControlled())
	{
		GI->PlayerHealth = AttributeSetBase->GetHealth();
		GI->bInitialized = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyBuilderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyBuilderCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyBuilderCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyBuilderCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyBuilderCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyBuilderCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyBuilderCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyBuilderCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyBuilderCharacter::OnResetVR);

}


void AMyBuilderCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyBuilderCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyBuilderCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyBuilderCharacter::Jump()
{
	// TODO change collision disable from jump to tick, so it can disable collision when falling off ledges without jumping
	if (!GetHasControl()) return;
	Super::Jump();
}

void AMyBuilderCharacter::TurnAtRate(float Rate)
{
	if (!GetHasControl()) return;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyBuilderCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyBuilderCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && GetHasControl())
	{
		// find out which way is forward
		// const FRotator Rotation = Controller->GetControlRotation();
		// const FRotator YawRotation(0, Rotation.Yaw, 0);

		// // get forward vector
		// const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Direction = FVector(1.f, 0.f, 0.f);
		AddMovementInput(Direction, Value);
	}
}

void AMyBuilderCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) & GetHasControl())
	{
		// find out which way is right
		// const FRotator Rotation = Controller->GetControlRotation();
		// const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// // get right vector 
		// const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		const FVector Direction = FVector(0.f, 1.f, 0.f);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

UAbilitySystemComponent* AMyBuilderCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
};

void AMyBuilderCharacter::GiveAbility(TSubclassOf<class UGameplayAbility> Ability)
{
	GiveAbilityWithInput(Ability, 0);
}
void AMyBuilderCharacter::GiveAbilityWithInput(TSubclassOf<class UGameplayAbility> Ability, uint16 InputId=1)
{
	if (HasAuthority() && Ability)
	{
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, InputId));
	}
	AbilitySystem->InitAbilityActorInfo(this, this);
}

void AMyBuilderCharacter::OnDie_Implementation()
{
	// UE_LOG(LogTemp, Warning, TEXT("Died on CPP"));
	OnDied.Broadcast(this);
	FGameplayTag MyTag = FGameplayTag::RequestGameplayTag(TEXT("control"));
	AbilitySystem->RemoveActiveEffectsWithTags(FGameplayTagContainer(MyTag));
	DisableInput(nullptr);
	AController* MyController = GetController();
	// DetachFromControllerPendingDestroy();
	StopAnimMontage();
	AMyPlayerController* Cont = Cast<AMyPlayerController>(MyController);
	if (Cont)
	{
		// FActorSpawnParameters SpawnParams;
		// SpawnParams.Owner = this;
		// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// APawn* MyPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		// Cont->Possess(MyPawn);
		Cont->OnPlayerDead();
	}
	DetachFromControllerPendingDestroy();
	
}

bool AMyBuilderCharacter::IsAlive_Implementation()
{
	if (AttributeSetBase->GetHealth() <= 0)
	{
		return false;
	}
	return true;
}

bool AMyBuilderCharacter::GetHasControl()
{
	FGameplayTag MyTag = FGameplayTag::RequestGameplayTag(TEXT("status.stun"));
	// AbilitySystem->RemoveActiveEffectsWithTags(FGameplayTagContainer(MyTag));
	// TODO this is inneficient, move to a single bool check that changes on event
	if (AbilitySystem->HasAnyMatchingGameplayTags(FGameplayTagContainer(MyTag))) return false;
	// if (AbilitySystem->ComponentHasTag(TEXT("status.stun"))) return false;
	return true;
}

bool AMyBuilderCharacter::GetAbilityKeyDown(uint8 Index)
{
	if (Index >= IsAbilityKeyDown.Num()) return false;
	return IsAbilityKeyDown[Index];
}

void AMyBuilderCharacter::SetAbilityKeyDown(uint8 Index, bool IsKeyDown)
{
	if (Index >= IsAbilityKeyDown.Num()) return;
	IsAbilityKeyDown[Index] = IsKeyDown;
}

void AMyBuilderCharacter::RemoveOneEffect(TSubclassOf<UGameplayEffect> EffectClass)
{
	AbilitySystem->RemoveActiveGameplayEffectBySourceEffect(EffectClass, AbilitySystem, 1);
}

void AMyBuilderCharacter::ActivateAbilityByInput(uint8 Index)
{
	for (auto &&Ability : Abilities)
	{
		if (Ability.Input == Index)
		{
			if ((Ability.CanUseOnAir && GetMovementComponent()->IsFalling())
			|| (Ability.CanUseOnGround && !GetMovementComponent()->IsFalling()))
			{
				AbilitySystem->TryActivateAbilityByClass(Ability.AbilityClass, true);
			}
		}
	}
}