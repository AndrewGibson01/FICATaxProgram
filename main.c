
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

long getMoneyInput(char desc[], int * decDigits, int * decDigits2);
int printCustomNum(long myNum, int MyPower);
long roundCustomNumber(long myNum, int * decDigits);
long formatCustomNumber(long myNum, int * decDigits);
int retCustomNumber(long myNum, int MyPower, char* resultingChars, int useCommas);
int getStringSize(char* resultingChars);
int setStringLengthPost(char* myInput, int length);
long formatCustomNumber2(long myNum, int * decDigits);

int main()
{
    int empCount = 1;
    int moreEmps = 1;
    long totalGross = 0;
    long totalTax = 0;
    printf("Enter the name & path of the input file you want the tax report to be stored (max 20 characters): ");
    FILE *outFile;
    char fileOutName[21];
    scanf( "%s", fileOutName);
    outFile = fopen( fileOutName, "w" );
	if( outFile == NULL ){
		printf("Could not create the output file! Program terminating.");
		exit(1);
	}
    printf("Enter today's date (max 13 characters): ");
    char date[14];
    scanf( "%s", date);
    printf("Enter your company's name: ");
    char CompName[50];
    scanf( "%s", CompName);
    fprintf(outFile, "%45s", CompName);
    fprintf(outFile, "%20s\n", date);
    fprintf(outFile, "Line No.      Emp. No.      Given     Surname       YTD ($)       Gross ($)       FICA Tax ($)\n");
    while (moreEmps != 0) 
    {
        int isGood = 0;
        int decimalCounter1 = 69;
        int digitCounter1 = 69;
        long ytd = 0;
        int decimalCounter2 = 69;
        int digitCounter2 = 69;
        long gross = 0;
        char YtdToString[50];
        char GrossToString[50];
        char id[9]; 
        char fName[21];
        char lName[21];
        printf("\nEnter Employee's Number (max 8 digits): ");
        scanf( "%s", id);
        printf("Enter Employee #%s's First Name (max 20 characters): ", id);
        scanf( "%s", fName);
        printf("Enter Employee #%s's Last Name (max 20 characters): ", id);
        scanf( "%s", lName);
        while (!isGood)
        {
            ytd = getMoneyInput("Enter this employee's YTD (in dollars): ", &decimalCounter1, &digitCounter1);
            ytd = formatCustomNumber(ytd, &decimalCounter1);
            gross = getMoneyInput("Enter this employee's Gross (in dollars): ", &decimalCounter2, &digitCounter2);
            gross = formatCustomNumber(gross, &decimalCounter2);
            retCustomNumber(ytd, -decimalCounter1, YtdToString, 1);
            retCustomNumber(gross, -decimalCounter2, GrossToString, 1);
            printf("\nEmployee #%s: ", id);
            printf("\nEmployee's First Name: %s", fName);
            printf("\nEmployee's Last Name: %s", lName);
            printf("\nYTD: $");
            printf("%s\n", YtdToString);
            printf("Gross: $");
            printf("%s\n", GrossToString);
            char myAnswer[2];
            printf("\nIs this data accurate? (type y for yes or n for no) ");
            scanf("%s", myAnswer);
            if (myAnswer[0] == 121 || myAnswer[0] == 89) {
                isGood = 1;
            } else {
                printf("\n");
            }
        }
        int taxDigits = -3;
        int taxPercent = 42;
        long taxable = 0;
        int taxableDigits = -2;
        long TAXLIMIT = 110100 * 100;
        if (ytd + gross <= TAXLIMIT) {
            taxable = gross;
        }
        if (ytd > TAXLIMIT) {
              taxable = 0;
        }
        else if (ytd + gross >= TAXLIMIT) {
            taxable = TAXLIMIT - ytd;
        }
        long taxedAmount = taxable * taxPercent;
        int taxedDigits = taxDigits + taxableDigits;
        //printf("TAX DIGITS: %d", taxedDigits);
        taxedAmount = roundCustomNumber(taxedAmount, &taxedDigits);
        taxedAmount = formatCustomNumber2(taxedAmount, &taxedDigits);
        printf("Here is the FICA tax Employee #%s need to pay: $", id);
        printCustomNum(taxedAmount, taxedDigits);
        
        char ans[2];
        int isGood2 = 0;
        while (isGood2 == 0)
        {
            printf("\nAre there any more employees to enter? (Type y for yes or n for no) ");
            scanf( "%s", ans);
            if (ans[0] == 'y' || ans[0] == 'Y' || ans[0] == 'n' || ans[0] == 'N') {
                isGood2 = 1;
            } else {
                printf("\nINVALID INPUT");
            }
        }
        if (ans[0] == 'n' || ans[0] == 'N') {
            moreEmps = 0;
        }
        fprintf(outFile, "%-8d", empCount);
        fprintf(outFile, "%14s", id);
        fprintf(outFile, "      ");
        setStringLengthPost(fName, 10);
        fprintf(outFile, "%s", fName);
        setStringLengthPost(lName, 10);
        fprintf(outFile, "%s", lName);
        fprintf(outFile, "%11s", YtdToString);
        fprintf(outFile, "%16s", GrossToString);
        char taxed[50];
        retCustomNumber(taxedAmount, taxedDigits, taxed, 1);
        fprintf(outFile, "%19s\n", taxed);
        totalGross += gross;
        totalTax += taxedAmount;
        empCount++;
    }
    fprintf(outFile, "\n                                      Total: ");
    char totals[50];
    retCustomNumber(totalGross, -2, totals, 1);
    fprintf(outFile, "%30s", totals);
    retCustomNumber(totalTax, -2, totals, 1);
    fprintf(outFile, "%19s", totals);
    fclose(outFile);
    printf("\n\n");
    FILE *readFile;
    char c;
    // Open file
    readFile = fopen(fileOutName, "r");
    if (readFile == NULL)
    {
        printf("Cannot open file \n");
        exit(1);
    }
    // Read contents from file
    c = fgetc(readFile);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(readFile);
    }
    fclose(readFile);
    return 0;
}

int setStringLengthPost(char* myInput, int length)
{
    int i = 0;
    int hasSeenEnd = 0;
    while (i < length) 
    {
        if (myInput[i] == 0) 
        {
            hasSeenEnd = 1;
        }
        if (hasSeenEnd != 0) 
        {
            myInput[i] = ' ';
        }
        
        i++;
    }
    myInput[i] = 0;
    return 0;
}

long formatCustomNumber2(long myNum, int * decDigits)
{
    if (myNum == 0) {
        *decDigits = 2;
    } else {
    
        int totalDigits = 1;
        long tester = 1;
        while (tester <= myNum) {
            totalDigits++;
            tester *= 10;
        }
        
        int difference = *decDigits + totalDigits;
        //printf("DIFFERENCE: %d", difference);
        while (totalDigits < 2)
        {
            myNum /= 10;
            totalDigits++;
            *decDigits++;
        }
    }
    return myNum;
}

long formatCustomNumber(long myNum, int * decDigits)
{
    while (*decDigits < 2)
    {
        
        myNum *= 10;
        *decDigits = *decDigits + 1;
    }
    return myNum;
}

long roundCustomNumber(long myNum, int * decDigits)
{
    
    while (*decDigits <= -3)
    {
        if (myNum % 10 != 0) {
            myNum += 10;
        }
        myNum /= 10;
        *decDigits = *decDigits + 1;
    }
    return myNum;
}

int getStringSize(char* myInput) 
{
    
    int count = 1;
    int i = -1;
    while (1) 
    {
        i++;
        //end of string
        if (myInput[i] != 0 ) 
        {
            count++;
        } else {
            break;
        }
    }
    return count;
}

int retCustomNumber(long myNum, int MyPower, char* resultingChars, int useCommas)
{
    int numOfDigits = 0;
    long e = 1;
    while (myNum >= e) {
        
        numOfDigits++;
        e *= 10;
    }
    char myInput[50];
    sprintf(myInput, "%ld", myNum);
    int hasSeenPeriod = 0;
    int resIndex = 0;
    int commaCounter = 0;
    int commaOffset = 0;
    if (!(numOfDigits < 6)) {
        commaOffset = 2 - (numOfDigits % 3);
    }
    for (int i = 0; i < 50; i++) 
    {
        //end of string
        if (myInput[i] != 0 ) 
        {
            int pos = numOfDigits + MyPower - i;
            
            if (pos <= 0 && hasSeenPeriod != 1) {
                if (i == 0) {
                    resultingChars[resIndex++] = '0';
                }
                resultingChars[resIndex++] = '.';
                
                while (pos < 0) {
                    pos++;
                    resultingChars[resIndex++] = '0';
                }
                
                hasSeenPeriod = 1;
            }
            resultingChars[resIndex++] = myInput[i];
            if (i + 3 < numOfDigits && (i + commaOffset) % 3 == 2) {
                resultingChars[resIndex++] = ',';
                commaCounter = 0;
            }
            commaCounter++;
        } else 
        {
            break;
        }
    }
    resultingChars[resIndex] = '\0';
    return 0;
}
int printCustomNum(long myNum, int MyPower) 
{
    int numOfDigits = 0;
    long e = 1;
    while (myNum >= e) {
        numOfDigits++;
        e *= 10;
    }
    char myInput[50];
    sprintf(myInput, "%ld", myNum);
    int hasSeenPeriod = 0;
    for (int i = 0; i < 50; i++) 
    {
        //end of string
        if (myInput[i] != 0 ) 
        {
            int pos = numOfDigits + MyPower - i;
            if (pos <= 0 && hasSeenPeriod != 1) {
                if (i == 0) {
                    printf("0");
                }
                printf(".");
                while (pos < 0) {
                    pos++;
                    printf("0");
                }
                hasSeenPeriod = 1;
            }
            printf("%c", myInput[i]);
        } else 
        {
            break;
        }
        
    }
    return 0;
}
long getMoneyInput(char desc[], int * decDigits, int * decDigits2) {
    char myInput[50];
    long ytd2 = 0;
    int numOfDigits = 0;
    int hasSeenPeriod = 0;
    int hasIllegalChars = 1;
    int decimalCounter = 0;
    while (hasIllegalChars == 1)
    {
        numOfDigits = 0;
        hasSeenPeriod = 0;
        hasIllegalChars = 0;
        decimalCounter = 0;
        
        printf("%s", desc);
        scanf("%s", myInput);
        
        for (int i = 0; i < 50; i++) 
        {
            //end of string
            if (myInput[i] != 0 ) 
            {
                // $ sign
                if (myInput[i] == 36 ) 
                {
                    
                } // .
                else if (myInput[i] == 46 ) 
                {
                    hasSeenPeriod = 1;
                } // 0-9 
                else if (myInput[i] >= 48 && myInput[i] <= 57)
                {
                    if (hasSeenPeriod == 0) {
                        numOfDigits++;
                    } else {
                        decimalCounter++;
                    }
                    
                } // anything else
                else {
                    hasIllegalChars = 1;
                    break;
                }
                
            } else 
            {
                break;
            }
            
        }
        if (hasIllegalChars != 0 || decimalCounter > 2) {
            printf("Hey idiot you didn't put in a correct number.\n");
            hasIllegalChars = 1;
        }
    }
    *decDigits = decimalCounter;
    *decDigits2 = numOfDigits;
    
    int digitsLeft = numOfDigits + decimalCounter;
    
    for (int i = 0; i < 50; i++) 
    {
        //end of string
        if (myInput[i] != 0 ) 
        {
            //printf("%ld ", ytd2);
            // $ sign
            if (myInput[i] == 36 ) 
            {
                
            } // .
            else if (myInput[i] == 46 ) 
            {
                hasSeenPeriod = 1;
            } // 0-9 
            else if (myInput[i] >= 48 && myInput[i] <= 57)
            {
                int multiplier = 1;
                for (int e = 1; e < digitsLeft; e++) {
                    multiplier *= 10;
                }
                
                ytd2 += (myInput[i] - 48) * multiplier;
                digitsLeft--;
                
            } // anything else
            else {
                hasIllegalChars = 1;
                break;
            }
        } else 
        {
            break;
        }
    }
    return ytd2;
}
