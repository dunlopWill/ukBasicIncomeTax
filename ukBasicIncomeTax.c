#include <stdio.h>
#include <math.h>

// Income types
int nonSavingsIncome = 12500; //Input non-savings income here (Example)
int savingsIncome = 48000; //Input savings income here (Example)
int dividendIncome = 0; //Input dividend income (Example)

// 2018/19 Personal allowance and tax rate bands
int standardPA = 11850; //Personal allowance for the tax year 2018/19
int ceilingBRB = 34500; //Basic rate band ceiling for non-savings and savings income for the tax year 2018/19
int ceilingHRB = 150000; //Higher rate band ceiling for non-savings and savings income for the tax year 2018/19

int main(void)
{
    //Calculate net income
    int netIncome = nonSavingsIncome + savingsIncome + dividendIncome;
    printf("Net income is: £%i\n", netIncome);

    //Calculate adjusted personal allowance
    int adjustedPA;
    int ceilingPA = 100000 + standardPA * 2;

    if (netIncome <= 100000)
    {
        adjustedPA = standardPA;
    }
    else if (netIncome >= ceilingPA)
    {
        adjustedPA = 0;
    }
    else
    {
        adjustedPA = standardPA - (netIncome - 100000)/2;
    }
    printf("Adjusted personal allowance is: £%i\n", adjustedPA);

    //Calcualte taxable income by reducing each income type by the remaining adjusted personal allowance
    int taxableNSI; //Taxable non-savings income
    int taxableSI; //Taxable savings income
    int taxableDI; //Taxable dividend income

    if (nonSavingsIncome >= adjustedPA)
    {
        taxableNSI = nonSavingsIncome - adjustedPA;
        taxableSI = savingsIncome;
        taxableDI = dividendIncome;
    }
    else
    {
        taxableNSI = 0;
        adjustedPA = adjustedPA - nonSavingsIncome;
        if (savingsIncome >= adjustedPA)
        {
            taxableSI = savingsIncome - adjustedPA;
            taxableDI = dividendIncome;
        }
        else
        {
            taxableSI = 0;
            adjustedPA = adjustedPA - savingsIncome;
            if (dividendIncome >= adjustedPA)
            {
                taxableDI = dividendIncome - adjustedPA;
            }
            else
            {
                taxableDI = 0;
            }
        }
    }
    printf("Taxable non-savings income is: £%i\n", taxableNSI);
    printf("Taxable savings income is: £%i\n", taxableSI);
    printf("Taxable dividend income is: £%i\n", taxableDI);

    int taxableIncome = taxableNSI + taxableSI + taxableDI;
    printf("Total taxable income is: £%i\n", taxableIncome);

    //Calculate income tax liability on taxable non-savings income
    int liableNSI;

    if (taxableNSI == 0)
    {
        liableNSI = 0;
    }
    else if (taxableNSI > 0 && taxableNSI <= ceilingBRB)
    {
        liableNSI = taxableNSI * 0.2;
    }
    else if (taxableNSI > ceilingBRB && taxableNSI <= ceilingHRB)
    {
        liableNSI = ceilingBRB * 0.2 + (taxableNSI - ceilingBRB) * 0.4;
    }
    else
    {
        liableNSI = ceilingBRB * 0.2 + (ceilingHRB - ceilingBRB) * 0.4 + (taxableNSI - ceilingHRB) * 0.45;
    }

    printf("Tax liability on taxable non-savings income is: £%d\n", liableNSI);


    //Keep track of how much taxableIncome has been taxed
    int cumulativeTaxed;
    cumulativeTaxed = taxableNSI;

    //Establish starting rate band for taxable savings income
    int startingRateBand; //Starting rate band (SRB) applies only to taxpayers with no more than £5000 of taxable non-savings income
    if (taxableNSI <= 5000)
    {
        startingRateBand = 5000 - taxableNSI;
    }
    else
    {
        startingRateBand = 0;
    }

    //Establish savings nil rate band for taxable savings income
    int savingsNRB; //Savings nil rate band (NRB)
    if (taxableIncome <= ceilingBRB) //If basic rate tax payer, savings NRB is £1000
    {
        savingsNRB = 1000;
    }
    else if (taxableIncome <= ceilingHRB) //If higher rate tax payer, savings NRB is £500
    {
        savingsNRB = 500;
    }
    else //If additional rate tax payer, savings NRB is £0.
    {
        savingsNRB = 0;
    }

    //Reduce taxable savings income by SRB and NRB
    if (taxableSI < startingRateBand + savingsNRB)
    {
        cumulativeTaxed = cumulativeTaxed + taxableSI;
        taxableSI = 0;
    }
    else
    {
        taxableSI = taxableSI - startingRateBand - savingsNRB;
        cumulativeTaxed = cumulativeTaxed + startingRateBand + savingsNRB;
    }

    //Calculate savings income tax liablity
    int liableSI = 0;

    if (taxableSI == 0) //...if taxable savings income is zero
    {
        liableSI = 0;
    }
    else if (cumulativeTaxed + taxableSI <= ceilingBRB) //...if adding taxable savings income is still within BRB
    {
        liableSI = taxableSI * 0.2;
    }
    else if (cumulativeTaxed + taxableSI <= ceilingHRB) //...if adding taxable income exceeds BRB but is still within HRB
    {
        if (cumulativeTaxed <= ceilingBRB) //...if cumulativeTaxed is within BRB
        {
            liableSI = (ceilingBRB - cumulativeTaxed) * 0.2 + (cumulativeTaxed + taxableSI - ceilingBRB) * 0.4;
        }
        else
        {
            liableSI = taxableSI * 0.4;
        }
    }
    else if (cumulativeTaxed + taxableSI > ceilingHRB) //...if adding taxable income exceeds HRB
    {
        if (cumulativeTaxed <= ceilingBRB)
        {
            liableSI = (ceilingBRB - cumulativeTaxed) * 0.2 + (ceilingHRB - ceilingBRB) * 0.4 + (cumulativeTaxed + taxableSI - ceilingHRB) * 0.45;
        }
        else if (cumulativeTaxed <= ceilingHRB)
        {
            liableSI = (ceilingHRB - cumulativeTaxed) * 0.4 + (cumulativeTaxed + taxableSI - ceilingHRB) * 0.45;
        }
        else
        {
            liableSI = taxableSI * 0.45;
        }
    }

    printf("Tax liability on taxable savings income is: £%d\n", liableSI);

    cumulativeTaxed = cumulativeTaxed + taxableSI;

    //Reduce taxable dividend income by NRB
    int dividendNRB = 2000; //Dividend nil rate band applies to all
    if (taxableDI < dividendNRB)
    {
        cumulativeTaxed = cumulativeTaxed + taxableDI;
        taxableDI = 0;
    }
    else
    {
        taxableDI = taxableDI - dividendNRB;
        cumulativeTaxed = cumulativeTaxed + dividendNRB;
    }

    //Calculate dividend income tax liability;
    int liableDI = 0;

        if (taxableDI == 0) //...if taxable savings income is zero
    {
        liableDI = 0;
    }
    else if (cumulativeTaxed + taxableDI <= ceilingBRB) //...if adding taxable savings income is still within BRB
    {
        liableDI = taxableDI * 0.075;
    }
    else if (cumulativeTaxed + taxableDI <= ceilingHRB) //...if adding taxable income exceeds BRB but is still within HRB
    {
        if (cumulativeTaxed <= ceilingBRB) //...if cumulativeTaxed is within BRB
        {
            liableDI = (ceilingBRB - cumulativeTaxed) * 0.075 + (cumulativeTaxed + taxableDI - ceilingBRB) * 0.325;
        }
        else
        {
            liableDI = taxableDI * 0.325;
        }
    }
    else if (cumulativeTaxed + taxableDI > ceilingHRB) //...if adding taxable income exceeds HRB
    {
        if (cumulativeTaxed <= ceilingBRB)
        {
            liableDI = (ceilingBRB - cumulativeTaxed) * 0.075 + (ceilingHRB - ceilingBRB) * 0.325 + (cumulativeTaxed + taxableDI - ceilingHRB) * 0.381;
        }
        else if (cumulativeTaxed <= ceilingHRB)
        {
            liableDI = (ceilingHRB - cumulativeTaxed) * 0.325 + (cumulativeTaxed + taxableDI - ceilingHRB) * 0.381;
        }
        else
        {
            liableDI = taxableDI * 0.381;
        }
    }

     printf("Tax liability on taxable dividend income is: £%d\n", liableDI);

    //Calculate total income tax liability
    int totalLiable = liableSI + liableNSI + liableDI;
    printf("Total income tax liability is: £%d\n", totalLiable);
}
