////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmpsc311-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMPSC311 at Penn State University.  See the related
//                   assignment page for details.
//
//   Author        : Roman Tuzhilkin
//   Last Modified : ????
//

// Include Files
#include <stdio.h>
#include <time.h>
#include "cmpsc311_util.h"

// Defines
#define NUM_CARDS 52
#define MAX_CARDS 11
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_card
// Description  : print the card from the integer value
//
// Inputs       : card - the card to print
// Outputs      : 0 always

int print_card( int card ) {

    // CODE HERE
    char card_faces[] = "234567891JQKA";
    char *card_suits[] = { SPADE, CLUB, HEART, DIAMOND };
    int suit = card / 13, cardty = card % 13;
    if ( cardty == 8 ) {
        printf( "10%s", card_suits[suit]);
    } else {
        printf( "%c%s", card_faces[cardty], card_suits[suit]);
    }

    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int print_cards( int cards_deck[], int num_cards ) {
  int cnt = 0;

  for (int i = 0; i < num_cards; i++) {
    cnt++;
    print_card( cards_deck[i]);
    printf("%s", " ");
    if (cnt == 13)
    {
      printf("%s\n", "");
      cnt = 0;
    }
  }

  //printf("%s\n", "");

  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : shuffle_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int shuffle_cards( int cards[], int num_cards )
{
  int rand_index = 0;
  int i = 0;
  int temp = 0;

  while (i < num_cards - 2)
  {
    //rand_index = i + rand() % (num_cards - i + 1);
    rand_index = getRandomValue( i, num_cards - 1);

    temp = cards[i];
    cards[i] = cards[rand_index];
    cards[rand_index] = temp;

    i++;
  }

  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : hand_value
// Description  : return the value of the hand
//
// Inputs       : cards - the array of cards in the hand
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int hand_value( int cards[], int num_cards )
{
  int sum = 0;
  int Aces_flag = 0;
  int temp = 0;

  for (int i = 0; i < num_cards; i++)
  {
    if (cards[i] <= 12)
    {
      temp = cards[i];
    }
    else if (cards[i] <= 25)
    {
      temp = cards[i] - 13;
    }
    else if (cards[i] <= 38)
    {
      temp = cards[i] - 26;
    }
    else
    {
      temp = cards[i] - 39;
    }

    if (temp <=8)
    {
      sum = sum + temp + 2;
    }
    else if (temp <= 11)
    {
      sum = sum + 10;
    }
    else
    {
      Aces_flag++;
    }
  }

  if ((Aces_flag != 0) && (sum != 21))
  {
    int Aces__points_to_add = 11; //Available aces point to add 1-11
    while ((sum != 21) && (Aces__points_to_add > 0))
    {
      sum += 1;
      Aces__points_to_add--;
    }
  }

  return sum;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sort_cards
// Description  : sort a collection of cards
//
// Inputs       : hand - the cards to sort
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int sort_cards( int hand[], int num_cards ) {
  int flag = 0;
  int temp = 0;

  do {
    flag = 1;
    for (int i = 0; i < num_cards - 1; i++) {
      if (hand[i] > hand[i + 1])
      {
        temp = hand[i];
        hand[i] = hand[i + 1];
        hand[i + 1] = temp;
        flag = 0;
      }
    }
  } while(flag != 1);

  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : dealer_play
// Description  : dealer decision to hit or stand (hit on anything less
//                than 17)
//
// Inputs       : hand - cards dealer has
//                num_cards - the number of cards in player hand
// Outputs      : 0 = stand, 1 = hit

int dealer_play( int hand[], int num_cards ) {

  int sum = hand_value(hand, num_cards);

  if (((sum >= 16) && (sum <= 21)) || (num_cards >= MAX_CARDS))
  {
    return 0;
  }
  else if (sum < 16)
  {
    return 1;
  }
  else
  {
    return 2; //Signals that player has over 21
  }

  return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : player_play
// Description  : player decision to hit or stand
//
// Inputs       : hand - cards player has
//                num_cards - the number of cards in player hand
//                dealer_card - the dealers face up card
// Outputs      : 0 = stand, 1 = hit

int player_play( int hand[], int num_cards, int dealer_card )
{
  int sum = hand_value(hand, num_cards);

  if (((sum >= 16) && (sum <= 21)) || (num_cards >= MAX_CARDS))
  {
    return 0;
  }
  else if (sum < 16)
  {
    return 1;
  }
  else
  {
    return 2; //Signals that player has over 21
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : play_hand
// Description  : play a hand of black jack
//
// Inputs       : deck - the whole deck of cards
//                num_cards - the number of cards in the deck
//                player_money - pointer to player amount of money
// Outputs      : 1 if player wins, 0 if dealer wins

int play_hand( int deck[], int num_cards, float *player_money ) {
  int dealer_cards[MAX_CARDS];
  int player_cards[MAX_CARDS];
  int final_hand_dealer = 0;               //The final hand value the guy stands with
  int final_hand_player = 0;               //The final hand value the guy stands with
  int number_player_cards = 2;             //Always at least two cards to start
  int number_dealer_cards = 2;             //Always at least two cards to start
  int i = 4;                               //index of deck exclusive first 4 played cards
  int player_play_response = 0;            //Strategy response to hit/stand

  printf("%s\n", "---- New hand -----\n");

  shuffle_cards(deck, num_cards);

  //////////////////////////First cards being handed out

  dealer_cards[0] = deck[0];
  dealer_cards[1] = deck[1];
  player_cards[0] = deck[2];
  player_cards[1] = deck[3];

  printf("%s", "Dealer cards: ");
  print_card(dealer_cards[0]);
  printf("%s", "  XX\n\n");

  printf("%s", "Player cards: ");
  print_card(player_cards[0]);
  printf("%s", "  ");
  print_card(player_cards[1]);

  //////////////////////////////////////////////////////

  ///////////////////////////////////Player move goes on
  if (hand_value(player_cards, 2) == 21)
  {
    printf("%s", "\nPlayer has Blackjack!, wins $7.50\n");
    *player_money += 7.5;
    return 0;
  }
  else
  {
    player_play_response = player_play(player_cards, number_player_cards, dealer_cards[0]);

    while (player_play_response == 1) //Player hits until 17 or more
    {
      player_cards[number_player_cards] = deck[i];
      number_player_cards++;
      printf("%s%d%s", "\nPlayer hit (", hand_value(player_cards, number_player_cards), "): ");
      print_cards(player_cards, number_player_cards);

      player_play_response = player_play(player_cards, number_player_cards, dealer_cards[0]);
      i++;
    }

    if (player_play_response == 0)
    {
      printf("%s%d%s", "\nPlayer stands (", hand_value(player_cards, number_player_cards), "): ");
      print_cards(player_cards, number_player_cards);
      printf("%s", "\n");
      final_hand_player = hand_value(player_cards, number_player_cards);
    }
    else if (player_play_response == 2) //Overkill
    {
      printf("%s", "\nPlayer BUSTS ... dealer wins!\n\n");
      *player_money -= 5;
      return 0;
    }

    ///////////////////////////////////////////////////////////Player move is done

    ///////////////////////////////////////////////////////////Dealer turn to move
    player_play_response = dealer_play(dealer_cards, number_dealer_cards);

    while (player_play_response == 1) //Player hits until 17 or more
    {
      dealer_cards[number_dealer_cards] = deck[i];
      number_dealer_cards++;
      printf("%s%d%s", "\nDealer hit (", hand_value(dealer_cards, number_dealer_cards), "): ");
      print_cards(dealer_cards, number_dealer_cards);

      player_play_response = dealer_play(dealer_cards, number_dealer_cards);
      i++;
    }

    if (player_play_response == 0)
    {
      printf("%s%d%s", "\nDealer stands (", hand_value(dealer_cards, number_dealer_cards), "): ");
      print_cards(dealer_cards, number_dealer_cards);
      final_hand_dealer = hand_value(dealer_cards, number_dealer_cards);
    }
    else if (player_play_response == 2) //Overkill
    {
      printf("%s", "\nDealer BUSTS ... player wins!\n\n");
      *player_money += 5;
      return 0;
    }
    ///////////////////////////////////////////////////////////Dealer move is done

    ///////////////////////////////////////////////////////////Calculating result
    if (final_hand_dealer >= final_hand_player)
    {
      printf("%s", "\n\nDealer wins!!!\n\n");
      *player_money -= 5;
    }
    else
    {
      printf("%s", "\n\nPlayer wins!!!\n\n");
      *player_money += 5;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : show_player_money_histogram
// Description  : Display a histogram of the player funds by hands
//
// Inputs       : money_rounds - the list of player money by hand number
//                last_round - the hand number where the game stopped
// Outputs      : 0 always

int show_player_money_histogram( float money_rounds[], int last_round )
{
  int max_money = 0; //The maximum money player ever had so set as top coordinate.

  printf("%s", "\n                                             Player Cash by Round                                             \n");
  printf("%s", "     ---------------------------------------------------------------------------------------------------- \n");

  for (int i = 0; i < last_round; i++) //Setting maximum value
  {
    if (max_money < money_rounds[i])
    {
      max_money = money_rounds[i];
    }
  }

  for (int i = 200; i >= 0; i-=5)
  {
    if (i / 100 > 0)
    {
      printf("%d%s", i, " |");
    } else if (i / 10 > 0)
    {
      printf("%s%d%s", " ", i, " |");
    } else
    {
      printf("%s%d%s", "  ", i, " |");
    }

    for (int j = 1; j <= 100; j++)
    {
      if ((i <= money_rounds[j])&&(j<last_round))
      {
        printf("%s", "X");
      }
      else
      {
        printf("%s", ".");
      }
    }

    printf("%s", "|\n");
  }

  ////////////////////////////////////////////////////Editing the end of the table

  printf("%s", "     ---------------------------------------------------------------------------------------------------- \n");
  printf("%s", "     ");
  for (int i = 1; i <= 10; i++)
  {
    printf("%s%d", "         ", i);
  }
  printf("%s", "\n");
  printf("%s", "     ");
  for (int i = 1; i <= 10; i++)
  {
    for (int i = 1; i < 10; i++)
    {
      printf("%d", i);
    }
    printf("%d", 0);
  }

  printf("%s", "\n");

  return (0);
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char **argv )
{

    /* Local variables */
    int cmp311_deck[NUM_CARDS];  // This is the deck of cards
    float money_rounds[101];  //Players money per each round
    float player_money = 100;
    int i = 1;
    int winning_games = 0;

    /* Preamble information srand(time(NULL)) ;*/
    printf( "CMPSC311 - Assignment #1 - Fall 2020\n\n" );
    srand(time(NULL));

    /* Step #1 - create the deck of cards */

    for (int i = 0; i < NUM_CARDS; i++) {
      cmp311_deck[i] = i;
    }

    /* Step #2 - print the deck of cards */

    print_cards(cmp311_deck, NUM_CARDS);
    printf("%s\n", "");

    /* Step #4 - shuffle the deck */

    shuffle_cards(cmp311_deck, NUM_CARDS);

    /* Step #5 - print the shuffled deck of cards */

    print_cards(cmp311_deck, NUM_CARDS);
    printf("%s\n", "");

    /* Step #6 - sort the cards */

    sort_cards(cmp311_deck, NUM_CARDS);

    /* Step #7 - print the sorted deck of cards */

    print_cards(cmp311_deck, NUM_CARDS);
    printf("%s\n", "");

    /* Step #9 - deal the hands */////
    money_rounds[0] = player_money;

    while ((i<=100) && (player_money>=5))
    {
      play_hand(cmp311_deck, NUM_CARDS, &player_money);
      printf("%s%d%s%0.2f%s", "After hand ", i, " player has ", player_money, "$ left\n");

      if (player_money-money_rounds[i-1] > 0)
      {
        winning_games++;
      }
      money_rounds[i] = player_money;

      i++;
    }

    printf("%s", "-------------\n");
    printf("%s%d%s%d%s%d%s", "Blackjack done - player won ", winning_games, " out of ", i, " hands (", winning_games, ".00).\n");

    /* Step 10 show historgrapm */

    show_player_money_histogram(money_rounds, i);

    /* Exit the program successfully */
    printf( "\n\nCMPSC311 - Assignment #1 - Spring 2020 Complete.\n" );
    return( 0 );
}
