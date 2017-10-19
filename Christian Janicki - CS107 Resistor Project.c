/*
This program calculates the resistance of a basic circuit
using user-inputted circuit type, number of resistors,
and resistor color code.
Class: CS107, Fall 2017
System: Vim/GCC, Debian Linux + macOS
@author Christian Janicki
@version: September 25,2017
*/
#include <stdio.h>
#include <string.h>	//Necessary for string functions
#include <ctype.h>	//Necessary for 
#include <math.h>	//Necessary for pow function

//This function calculates the resistance of one resistor by taking
//in the user's inputted color band(after capitalization) and compares it
//to a list of known colors. It then calculates and returns the resistance.
float resistanceCalc(const char modColorBand[3][30],const char resistorColors[12][15], const float *resistorValues)
{
	//Counters for the three colors on the colorband
	int firstColor=0;
	int secColor=0;
	int thirdColor=0;

	//These three while loops determine the color of their respective part of 
	//the band by using string compare. Assuming the color inputted and the color
	//from the known list  are not the same, and the counter is less than 9 or 12, the
	//counters increment until one becomes false. 
	while(strcmp(modColorBand[0], resistorColors[firstColor])!= 0 && firstColor < 10){
		firstColor++;}
	//9 was chosen as the upper limit for these two counters because it disallows for
	//Gold and Silver to be chosen for the first two bands.
	while(strcmp(modColorBand[1], resistorColors[secColor])!= 0&& secColor < 10){
		secColor++;}
	while(strcmp(modColorBand[2], resistorColors[thirdColor]) != 0&& thirdColor < 12){
		thirdColor++;}

	//This checks to make sure the user's entries are indeed valid.
	if(firstColor >=10 || secColor >= 10 || thirdColor >= 12){
		printf("Please enter proper resistance values:\n");
		return -1.0;}

	//Assuming it is valid, the resistance is calculated using numbers from an
	//array of values that correspond to the band colors.
	else{
		float resistance = ((10.0 * resistorValues[firstColor]) + (resistorValues[secColor]))*(pow(10.0,resistorValues[thirdColor]));

		return resistance;
	}
}

//This function determines the circuit type inputted by the user.
//It uses string compare to compare a capitalized version of the
//user's input to either (S)ERIES or (P)ARALLEL. It returns an integer
//that is used in main to determine the final type.
int orientation(const char* resistStatus)
{
	if(!strcmp(resistStatus, "SERIES") || !strcmp(resistStatus, "S"))
		return 1;
	else if(!strcmp(resistStatus, "PARALLEL") || !strcmp(resistStatus, "P"))
		return 2;
	else
		return 3;

}

//This function takes the user's input, capitalizes it and removes any
//spaces. It takes the original input and stores it in a copy, just so
//no damage is done to the original in case of some error.
void allCapsNoSpace(char* modifiedString, const char* passString)
{
	//The way it works is that, if the character in the string
	//does not equal the terminating character, and its not a space,
	//then capitalize it. If it is a space, remove it. If it is a 
	//terminating chracter, then end.
	while(*passString != '\0'){
		if(!isspace(*passString)){
			*modifiedString = toupper(*passString);
			passString++;
		}
		modifiedString++;
	}
	*modifiedString ='\0';
}

//This function determines the number of resistors in the circuit, and
//ensures the user did not enter a character. This would be in the main 
//function (as I only use it once), but leaving it in the main caused errors
//if the user decided to use the entire calculator more than once in a row.
//Long was used in case of large numbers, but current functionality for extremely
//large numbers is poor.
//This function is good at catching a small amounts of characters, but aborts
//if presented with large amounts of characters. I was researching the 
//realloc function to attempt to give the string more memory as needed,
//but was not able to properly implement it in time.
long resistorCount ()
{
	long numResistors;	//variable for number of resistors
	int scanValue;		//variable for scanf's return value
	char charCheck;		//checks for a character input
	char bufferString[50];	//small string buffer

	//If scanf's return value is not 2 or the check for characters variable
	//is not equal to new line, or the number of resistors is greater than 0
	//continue the loop.
	while(scanValue != 2|| charCheck != '\n'|| numResistors >= 0)
	{
		//If scanf returns 2, it means it picked up both an integer
		//and a character
		scanValue = scanf("%ld%c", &numResistors, &charCheck);

		//If it is not 2, return an error, and catch and bad input 
		//in a buffer string
		if(scanValue != 2 || charCheck != '\n'){
			scanf("%s", bufferString);
			printf("Please enter an integer value (No characters)\n");}
		//Else, if the number of resistors is less than zero, print an error
		else if(numResistors <= 0)
			printf("Please enter an integer greater than 0\n");	
		//Break the loop.
		else
			break;
	}
	//Finally, return the number of resistors in the circuit.
	return numResistors;

}

//This function determines the SI prefix of any floats.
//It compares the resistances to constants to determine
//if it requires a small or large prefix, and determines
//the place by dividing by 10 and counting up until the
//remainder is zero.
int unitsSI (long double  resistance)
{
	int count = 0;

	//The counts go up until modulus 10 returns 0 or until
	//count divided by 3 passes 9. The counts are divided by
	//3 because the prefixes are powers of 3.
	if(resistance > 1.0){
		while((count/3) <9 &&((int)resistance % 10) == 0)
		{
			resistance = resistance / 10;
			count++;
		}
		return count;
	}
	else if(resistance < 1.0){
		while((count/3)<9 &&((int)resistance % 10) == 0)
		{
			resistance = resistance * 10;
			count++;
		}
		return count;
	}
	return 0;
}

int main ()
{
	//Plethora of variables. The ones with the mod prefixes are
	//modified 'clones' of the orignals, to protect the original
	//data.
	char resistorStatus[10];
	char modResistStatus[10];
	long numResistors=0;
	//The color bands are stored in 2D arrays for easy access.
	char colorBand[3][30];
	char modColorBand[3][30];
	int resistSP = 0;	//This variable is the circuit type tracker from the function above
	float resistance= -1.0;
	long double totalResistance;	//Long double is used here in case of large
					//numbers. (Currently large numbers are 
					//not functioning as intended.)
	//What follows is a 2D array of strings, to which the colorBand string is compared
	const char resistorColors [12][15] ={
		{"BLACK"},{"BROWN"},{"RED"},{"ORANGE"},{"YELLOW"},{"GREEN"},
		{"BLUE"},{"VIOLET"},{"GREY"},{"WHITE"},{"GOLD"},{"SILVER"}};
	//The following array corresponds to the values of the above color.
	//Their place in the array matches the place of the colors in the above array.
	const float resistorValues[12] =
		{0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,-1.0,-2.0};
	char loopYesNo[20];		//variable that determines if the main function is looped
	char modloopYesNo[20];		//Capitalized version of above
	int keepGoing = 1;		//Final determination of above mentioned loop.
	int totalResUnits,resUnits;	//Variables for SI units
	//SI units list and the resistances corresponding SI characters
	char largeListSI[9] = 
		{'\0','k','M','G','T','P','E','Z','Y'};
	char smallListSI[9] = 
		{'\0','m','u','n','p','f','a','z','y'};
	char totalResSymbol, resSymbol;
	
	int colorCount;			//Variable that serves as a counter in a later loop.

	printf("Welcome to the simple resistance calculator\n");

	while(keepGoing != 0){
		//Infinitely loops this function until resistSP does not equal 
		//3. This is the check for the resistor circuit type.
		//A while loop might have been better and cleaner, but this
		//one functions the way I wanted it to.
		for(;;){
			//Asks for and scans in the user input for circuit type
			printf("Are the resistors in Series or Parallel\n");
			scanf("%s", resistorStatus);

			//Capitalizes it and removes any spaces
			allCapsNoSpace(modResistStatus, resistorStatus);

			//Compares the modified user input to known values
			resistSP = orientation(modResistStatus);

			//Decides whether the input was correct or not, and loops appropriately
			if(resistSP == 3)
				printf("Please enter Series or Parallel\n\n");
			else
				break;
		}
	
		//Asks for and checks the user input for number of resistors
		printf("How many resistors are there in the circuit?\n");
		numResistors = resistorCount();


		printf("The first three color bands on the resistors: \n");

		//This loop grabs the colors,capitalizes them, and stores them in the array
		do
		{
			for(colorCount = 0; colorCount< 3; colorCount++){
				scanf("%s", colorBand[colorCount]);
				allCapsNoSpace(modColorBand[colorCount], colorBand[colorCount]);
			}
			//Calculates the resistance of the resistor
			resistance = resistanceCalc(modColorBand, resistorColors, resistorValues);
		}
		while(resistance == -1.0);	//Makes sure a valid response is given

		//Calculates the total resistance based on the circuit type
		if(resistSP == 1)
			totalResistance = numResistors * resistance;
		else if(resistSP == 2)
			totalResistance = (1/(numResistors *(1/ resistance)));

		//Finds the SI Prefix number for the total resistance and regular resistance
		totalResUnits = unitsSI(totalResistance)+1;
		resUnits = unitsSI(resistance)+1;
		//Adding one to calibrate`

		//Finds the SI prefix letter based on whether the resistance is
		//larger or smaller than 1. Also adjusts the number to match the
		//prefix. Ideally, I would've liked to have the smaller values (<1)
		//grow to the 100s place, and add the proper prefix, but at the moment,
		//it functions differently.
		if(resistance >1.0){
			resSymbol = largeListSI[resUnits/3];
			if(resUnits/3 != 0)
				resistance = resistance /pow(10,resUnits-(resUnits % 3));
		}
		else if(resistance < 1.0){
			resSymbol = smallListSI[resUnits/3];
			if(resUnits/3 != 0)
		       		resistance = resistance *pow(10,resUnits-(resUnits % 3));
		}
		if(totalResistance >1.0){
			totalResSymbol = largeListSI[totalResUnits/3];
			if(totalResUnits/3 != 0)
				totalResistance = totalResistance/pow(10, totalResUnits-(totalResUnits % 3));
		}
		else if(totalResistance < 1.0){
			totalResSymbol = smallListSI[totalResUnits/3];	
			if(totalResUnits/3 != 0)
				totalResistance = totalResistance*pow(10, totalResUnits-(totalResUnits % 3));
		}
	
		//Prints the resistances and their prefixes
		printf("Each resistor is %.2f %c ohms and the total resistance of the circuit is %.2Lf %c ohms\n", resistance,resSymbol , totalResistance, totalResSymbol);

		//Asks the user whether they wish to continue using the program
		//Based on the response, it ends or continues the loop.
		//A while loop would likely be more appropriate, but again,
		//it works the way it should.
		for(;;){
			printf("Continue?: (Y)es / (N)o\n");
			scanf("%s", loopYesNo);
			allCapsNoSpace(modloopYesNo, loopYesNo);
			if(!strcmp(modloopYesNo,"YES")||!strcmp(modloopYesNo, "Y")){
				keepGoing = 1; break;}
			else if(!strcmp(modloopYesNo, "NO") || !strcmp(modloopYesNo, "N")){
				keepGoing = 0; break;}
			else 
				printf("Please enter a valid choice\n");
		}

	}

	return 0;

}


/*Test Cases:
 
 Welcome to the simple resistance calculator
 Are the resistors in Series or Parallel
 SeRiEs
 How many resistors are there in the circuit?
 10
 The first three color bands on the resistors: 
 red white blue
 Each resistor is 29.00 M ohms and the total resistance of the circuit is 290.00 M ohms
 Continue?: (Y)es / (N)o
 y
 Are the resistors in Series or Parallel
 test
 Please enter Series or Parallel

 Are the resistors in Series or Parallel
 parallel
 How many resistors are there in the circuit?
 test
 Please enter an integer value (No characters)
 21
 The first three color bands on the resistors: 
 green white silver
 Each resistor is 0.59  ohms and the total resistance of the circuit is 28.10 m ohms
 Continue?: (Y)es / (N)o
 y
 Are the resistors in Series or Parallel
 series
 How many resistors are there in the circuit?
 20
 The first three color bands on the resistors: 
 gold red green
 Please enter proper resistance values:
 not a color
 Please enter proper resistance values:
 red red red
 Each resistor is 2.20 k ohms and the total resistance of the circuit is 44.00 k ohms
 Continue?: (Y)es / (N)o
 test
 Please enter a valid choice
 Continue?: (Y)es / (N)o
 no

 *
 *
 */
