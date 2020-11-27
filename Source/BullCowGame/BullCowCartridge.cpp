// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();

    Isograms = GetValidWords(HiddenWords);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input)
{    
    if(bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);        
    }
}

void UBullCowCartridge::SetupGame()
{
    // Initialize variables
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    // Print a welcome message with instruction
    PrintLine(TEXT("Welcome to Bulls and Cows Game!"));
    PrintLine(TEXT("Press Tab to access or leave the Terminal"));
    PrintLine(TEXT("Try to guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type your guess and hit Enter"));
    
    // Debug purposes
    // PrintLine(TEXT("The hidden word is '%s'"), *HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;

    PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    // Check if the player guesses the word right
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("Your guess is right! You win!"));

        EndGame();

        return;
    }

    // Check how many lives the player have
    if(--Lives > 0)
    {
        PrintLine(TEXT("You have %i more attempt(s)"), Lives);
    }
    else
    {
        PrintLine(TEXT("You don't have any lives left! You lost!"));
        PrintLine(TEXT("The hidden word was '%s'"), *HiddenWord);

        EndGame();

        return;
    }

    // Check if the entered word's length maches hidden word's
    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters long!"), HiddenWord.Len());

        return;
    }

    // Check if the guessed word is isometric or not
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("Please, type an isogram word!"));
        
        return;
    }

    FBullCowCounter Score = GetBullCow(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

FBullCowCounter UBullCowCartridge::GetBullCow(const FString& Guess) const
{
    FBullCowCounter Counter;

    for(int32 Index = 0; Index < Guess.Len(); Index++)
    {
        if(Guess[Index] == HiddenWord[Index])
        {
            ++Counter.Bulls;

            continue;
        }
        for(int32 Comparison = 0; Comparison < Guess.Len(); Comparison++)
            if(Guess[Index] == HiddenWord[Comparison])
            {
                ++Counter.Cows;

                break;
            }
    }

    return Counter;
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for(int32 Index = 0; Index < Word.Len(); Index++)
        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
            if(Word[Index] == Word[Comparison]) return false;
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const
{
    TArray<FString> ValidWords;

    for(FString Word : Words)
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) ValidWords.Emplace(Word);
    
    return ValidWords;
}