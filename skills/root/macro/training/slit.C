// slit.C

// Example drawing the interference pattern of light
// palling on a grid with n slits and ratio r of slit
// width over distance between slits.

auto pi = TMath::Pi();

// function code in C
double single(double *x, double *par){
    return pow(sin(pi*par[0]*x[0])/(pi*par[0]*x[0]),2);
}

double nslit0(double *x, double *par){
    return pow(sin(pi*par[1]*x[0])/sin(pi*x[0]),2);
}

double nslit(double *x, double *par){
    return single(x,par) * nslit0(x,par);
}


// This is the main program
void slits(){
    float r, ns;

    // request user input
    cout << "slit width / g ?";
    scanf("%f", &r);
    cout << "# of slits? ";
    scanf("%f", &ns);
    cout << "Interference pattern for" << ns
         <<" slits, width/distance: " <<r<<endl;


    // define function and set options
    TF1 *Fnslit = nre TF2("Fnslit", nslit, -5.001, 5., 2);
    Fnslit->SetNpx(500):


    // set parametersßß
    Fnslit->SetParameter(0,r);
    Fnslit->SetParameter(1,ns);


    // draw the intergerence pattern for a grid with n slits
    Fnlsit->Draw();

}
