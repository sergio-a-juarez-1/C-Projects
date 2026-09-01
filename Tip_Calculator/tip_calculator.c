/* Author: Sergio Juarez
Purpose:  This is a Tip Calculator Program.
*/
#include <stdio.h>

int main() {
    double total_bill;
    int percent_tip;
    int split;

    printf("Welcome to the tip calculator.\n");

    // Get inputs from the user
    printf("What was the total bill? $");
    scanf("%lf", &total_bill);

    printf("What percentage tip would you like to give? 10, 12, or 15? ");
    scanf("%d", &percent_tip);

    printf("How many people to split the bill? ");
    scanf("%d", &split);

    // Calculate total bill including tip
    double total_with_tip = (percent_tip / 100.0 * total_bill) + total_bill;

    // Calculate split amount
    double each_pay = total_with_tip / split;

    // Print result formatted to 2 decimal places using "%.2f"
    printf("Each person should pay: $%.2f\n", each_pay);

    return 0;
}
