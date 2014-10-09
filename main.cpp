#include <iostream>
#include <cstdlib>
#include <cmath>

// Included functions anc C libraries.
#include "4135FunctionDeclarations.h"

using namespace std;

// These functions are found below.
double ProfitCalc(double[],int[]);
double taou_n_tilda(int);
double Profit(int[]);

// Global variables.
int ordersForMonth_N[12];
double orderArrival_N[2000];

// These functions are found below.
int main(){
    // Seed the RNG.
    MTUniform (1);

    double bestProfit=0;       // Initiate best profit to 0

    int bestOrders[]={0,0,0,0, // Array used to store and keep
                      0,0,0,0, // track of best order
                      0,0,0,0},
        orders[]    ={0,0,0,0,   // Array used to test different
                      0,0,0,0,   // order simulations and keep
                      0,0,0,0};

    double money=0;  // Stores the profit made by using the order details in orderForMonth_N

    // Header showing the user the months that the order deliveries start and end
    // also includes number of cars to order, the overall progress and the simulated
    // expected profit
    cout << "Jan " << "Feb " << "Mar " << "Apr " << "May "
         << "Jun " << "Jul " << "Aug " << "Sep " << "Oct "
         << "Nov " << "Dec " << "Cars " << "   Profit "
         << " progress" << "\n";

    clock_t start=clock(); // To calculate elapsed time

    // Loop that begins the simulation
    for(int numOfOrders=5; numOfOrders<25; numOfOrders++){  // Range indicates the max number of car delivered in month p
        for(int month=0; month<12; month++){ // p loops through all the months
            double maxP=0;       // Store the best profit of the simulates strategies

            money=0; // Reset the expected amount to be made to zero for testing purposes
            for(int orders_i=0; orders_i<numOfOrders; orders_i++){   // Loop through all the possibilities for each month
                ordersForMonth_N[month]=orders_i; // Set the number of cars to be delivered in month i
                money=Profit(ordersForMonth_N); // Calculate the profit for this strategy

                if(money>bestProfit){ // Test to see if this strategy is better then one already found
                    bestProfit=money; // if true then store this as the profit for the best strategy
                    maxP=money;       // also set it as the best profit for this set of simulations
                    orders[month]=orders_i; // Store the specified delivery amount to keep track of the best strategy
                }
            }

            /*
              Test new stategy to see if it is better then the
              already found strategy (money indicates the profit
              of the new strategy and best profit indicates the
              best of all tested strategies)
             */

            if(money<bestProfit){
                copy(orders, orders+12,
                     bestOrders);
            }

            // Make a copy of the array containing the
            // simulations previous optimal outcome to
            // test the next set of possibilities
            copy(orders,orders+12,
                 ordersForMonth_N);
        }

        cout << " ";   int cars=0;  // Variable used to store the number of cars
        for(int c=0; c<12; c++){
            cout << bestOrders[c] << "   "; // Displays the best orders
            cars+=bestOrders[c];            // Calculates the number of cars
        }
        // Display the optimal number of cars for the simulation, expected profit, and progress
        cout << cars << "    "  << Profit(bestOrders) << "   " << numOfOrders+1-5 << "-"  << 20 << endl;
    }

    cout<<"Computations took "<< double(clock()-start)/CLOCKS_PER_SEC<<
    " seconds.\n\n\t";
    // Pause before closing up the window.
    Pause ();
} // This brace ends the main program.


#include "4135FunctionLibrary.h"


//////////////////////////////////////////////////////////////////////////////////////////
// This function computes the expected number of cars to be sold per month over many years
double taou_n_tilda(int n) {
    int i=1, done=0;
    double stdTbarhat=0.00,
    T2barhat  =0.00,
    Tbarhat   =0.00,
    epsilon   =0.01,
    pi        =4*atan(-1);

    while(!done){
        i++;
        double Tn, U, V, taou_n=orderArrival_N[n-1], lambda=1.0/90;

        U=MTUniform(0); V=MTUniform(0);
        while(1.0/90*(50-10*(double)cos(2*pi*taou_n))>=V){
            U=MTUniform(0); V=MTUniform(0);

            Tn=-1*lambda*log(U);
            taou_n+=Tn;
        }

        Tbarhat =(Tbarhat*(i-1)+taou_n)/i;
        T2barhat=(T2barhat*(i-1)+taou_n*taou_n)/i;

        if(i%5000==0){
            if (1.96*(sqrt((T2barhat-Tbarhat*Tbarhat)/i))<= epsilon)
                done=1;
        }
    }

    orderArrival_N[n]=Tbarhat;

    return (Tbarhat);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// This function simulates a sample scenario for the order arrivals in a year and sends it to
// the ProfitCalc to indicate the profit for the strategy specified in the array orders
double Profit(int Orders[]){
    // i counts the number of times the simulation was looped
    // done turns to 1 when the simulation is complete (telling
    // the program to break the loop)
    int i=0, done=0;

    double P2barhat  =0.000,   // Stores the sample avg second moment to test the variance
    Pbarhat   =0.000,       // Stores the sample avg to test variance and confidence
    epsilon   =5.000,       // Error tolerance
    profit    =0.000;       // Profit for single simulation

    // Loops simulation till error tolerance is met
    while(!done){
        i++;  // Increment i by one to keep track of the num of simuations

        /*
         Tn     = simulated time till next arrival
         U      = random variable U(0,1)
         taou_n = time till nth arrival
         lambda for the possion prosces
         sampleArrivals = simulated order arrivals
         */
        double Tn, U, taou_n=0, lambda=1.0/50,
        sampleArrivals[]={0,0,0,0,
                          0,0,0,0,
                          0,0,0,0};

        /*
         Loop until the nth arrival happens at t>=1 (corresponding
         to the end of the year or the beginning of next)
         */
        while(taou_n<1){
            U=MTUniform(0); // Generate a random number from 0-1

            Tn=-1*lambda*log(U); // Use the inverse transform to
            // get the simulated time till
            // the next arrival

            taou_n+=Tn;          // Update the time till the nth arrival

            // Check to see if a year has passed till the nth arrival.
            // if it has break the loop. Otherwise add simulated order
            // to the correct month
            if(int(taou_n/(1.0/12))<12)
                sampleArrivals[int(taou_n/(1.0/12))]++;
            else
                break;
        }

        // Calculate the profit for the sample arrivals and the
        // Orders specified at the begining of the orders
        profit=ProfitCalc(sampleArrivals, Orders);


        Pbarhat =(Pbarhat*(i-1)+profit)/i;          // Update first sample moment
        P2barhat=(P2barhat*(i-1)+profit*profit)/i;  // Update second sample moment

        // If the number of simulations in this loop are a multiple of 1000 check:
        if(i%1000==0){
            // If error tolerance is met indicate that the loop is done by making
            // done a positive number
            if (1.96*(sqrt((P2barhat-Pbarhat*Pbarhat)/i))<= epsilon)
                done=1;
        }
    }

    // Return the average expected profit
    return (Pbarhat);
}

/////////////////////////////////////////////////////////////////////////////////////////
//This function calculates the profit of the strategy in the array Orders for the sample
//scenario (in array arrivals) that was generated by the function profit
double ProfitCalc(double arrivals[], int Orders[]){
    // n is the number of cars specified in the current order
    // the loop simply calculates n by adding all the orders
    // specified for each month
    int n=0;
    for(int c=0; c<12; c++)
        n+=Orders[c];

    double costPer  =150,      // Variable representing cost per car
    sellFor  =200,      // Variable representing the selling price of each car
    delivery =20 ,      // One time yearly delivery fee
    clearance=75 ,      // Price of cars remaining on the lot (end of the year) are sold
                        // at clearance ($75K)
    carryCost=10.0/12;  // Cost of holding a car for a month ($10K per year)

    double orders[13],         // Array used to store a duplicate of the specified orders
    Revenue=0,          // Variable used to store simulated revenue
    netCost=costPer*n+  // Variable used to store simulated cost
    delivery;


    /*
     orders[12] is used to store cars carried over to next year
     that will have to be sold at clearance prices

     orders is made to be a copy of Orders to make it easier to
     store the leftover cars in slot 13 of orders (which will be sold
     for $75K)
     */
    orders[12]=0;
    copy(Orders,Orders+12,orders);

    // Loop used to calculate total net profit and cost
    for(int i=0; i<12; i++){
        // Check if there were more oders specified for month i
        // then order arrival simulated for month i
        if(orders[i]>arrivals[i]){
            // If this is the case
            // you make the money for the orders that arrived
            Revenue+=sellFor*arrivals[i];
            // You pay the carrying cost for the cars remaining on the lot
            netCost+=carryCost*(orders[i]-arrivals[i]);
            //add the cars that were unsolt to next months inventory
            orders[i+1]+=orders[i]-arrivals[i];
        }

        // If there are less cars on the lot then orders that arrived in month i
        else if(orders[i]<=arrivals[i]){
            // You simply make $200K per car sold
            Revenue+=sellFor*orders[i];
        }

        // Note: if no cars were sold the second condition specifies that no money is
        // made, nor is any money spent
    }

    // Check if any cars are left on the lot at the end of the year
    // if so, sell them at the clearance price
    if(orders[12]>0)
        Revenue+=clearance*orders[12];

    return(Revenue-netCost); // Return the simulated profit made for this year
}
