/*

 Name:           f1.C
 Created by:     Lorenzo Marini
 Date:           October 21st, 2021

 Purpose:        Example program under ROOT to compute the mean values
                 betwewm two  a given integer input 

 To run it, do:

 - start ROOT
 root [0] .L mean.C
 root [1] mean(double a, double b);

 */

#include <iostream>

double mean(double a, double b)
{
    return (a+b)/2;
}


