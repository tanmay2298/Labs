#include<iostream>
#include<stdlib.h>

using namespace std;


int get_max(int a, int b) {
    if ( a < b )
        return b ;
    return a ;
}


int knapsack( int row ,int col , int* table , int *weights , int *profits , int max )  {
    if ( table[row*max + col] == -1 ) {
        if ( weights[row - 1] > col )
            table[row*max + col] = knapsack( row-1 , col , table , weights , profits , max )  ; 
        else {
            table[row*max + col] = get_max(knapsack( row-1 , col , table , weights , profits , max )  ,
                profits[row - 1] + knapsack( row-1 , col - weights[row - 1], table , weights , profits , max ) ) ;
        }
    }
    return table[row*max + col] ;
}

int main() {

    int num_elements , max;
    cout<<"Total Capacity ? " ; cin>> max ;
    cout<<" Enter the number of elements in contention " ;  cin>> num_elements  ; 

    int *weights = (int*) malloc(sizeof(int)*num_elements) ;
    cout<<" Now enter the weights of each " ;
    for(int i = 0 ; i < num_elements ; i++ )
        cin>>weights[i] ;

    int *profits = (int*) malloc(sizeof(int)*num_elements) ;
    cout<<" Now enter the profits of each " ;
    for(int i = 0 ; i < num_elements ; i++ )
        cin>>profits[i] ;
   
    int *table = (int *)calloc( (max+1)*(num_elements+1) , sizeof(int)) ; // row size -> max , col size -> num_elements
     for(int i = 1 ; i <= num_elements ; i++ )
         for(int j = 1 ; j <= max ; j++ )
             table[i*(max +1) +j] = -1 ;

    cout<<" Answer -> "<<knapsack(num_elements, max , table , weights , profits , max+1) <<endl;
   
    return 0 ;
} 