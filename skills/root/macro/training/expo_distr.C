// expo_distr.C

// Visualisation of a histogram filled with exponentially
// distributed, random numbers. The exponential function is
// defined before

// define exponential function
double expo(double *x, double *par){
    return [0] + exp([1]*x);
}

void expo_distr{

    // histogram is instantiated, with name, title, # of bins
    TH1F *h = new TH1F("expo", 100, 0., 5.);

    // set parameters
    expo.SetParameter(0,1);
    expo.SetParameter(1,-1);

    // Data is entered in the histogram at line 4 using
    // the method TH1F::Fill in a loop construc
    for (int i=0; i<1000; i++) {h.Fill(expo.GetRandom());}

    // draw the histogram
    h.Draw();

}