// macro1.C

// Esempio preso e riadattato dalla guida di ROOT ufficiale
// Sezione 3.2 "A more complete example"



// Builds a graph with errores
// Displays ot
// Save it as image


// Include some header files (not necessary for Cling)

#include "TCanvas.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TArrow.h"
#include "TLatex.h"

void macro1(){


    // draw fit results on plot
    gStyle->SetOptFit(101111);

    // Data
    const int n_points=10;
    double x_data[n_points]={1,2,3,4,5,6,7,8,9,10};
    double y_data[n_points]={1.1,2.4,3.1,4.8,5.1,6.1,7.9,8.8,9.1,10.1};
    double dy_data[n_points]={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};

    // Instance of the TGraphErrors class
    TGraphErrors graph(n_points, x_data, y_data, nullptr, dy_data); // x errors (in this case none, represented by the NULL pointer)
    graph.SetTitle("Measurement xy; x [x]; y [y]"); // Defines in one shot the title of the graph and the titles of the two axes, separated by a “;”.

    // Make the plot estetically better
    graph.SetMarkerStyle(kOpenCircle);
    graph.SetLineColor(kBlue);

    // Canvas on which we'll draw the graph
    auto mycanvas = new TCanvas();
    mycanvas->SetTickx();
    mycanvas->SetTicky();

    // Draw the graph!
    graph.DrawClone("APE");
    /* COMMENTS:
    the method DrawClone draws a clone of the object on the canvas.
    It has to be a clone, to survive after the scope of macro1,
    and be displayed on screen after the end of the macro execution.
    The string option “APE” stands for:
    - A imposes the drawing of the Axes.
    - P imposes the drawing of the graph’s markers.
    – E imposes the drawing of the graph’s error bars.
    */

    // Define a linear function
    TF1 f("Linear law","[0]+x*[1]",.5,10.5);
    /*
    the constructor accepts the name of the function,
    the formula, and the function range.
    */
    // Let's make the function line nicer
    f.SetLineColor(kRed); f.SetLineStyle(2);
    // Fit it to the graph and draw it
    graph.Fit(&f);
    f.DrawClone("Same");
    /*
    draws the clone of the object on the canvas.
    The “Same” option avoids the cancellation of the already drawn objects,
    in our case, the graph. 
    The function f will be drawn using the same axis system defined 
    by the previously drawn graph.
    */

    // Add some text to the plot
    TLatex text(8.2,7.5,"#splitline{Maximum}{Deviation}"); 
    text.DrawClone();

    mycanvas->Print("graph_with_law.pdf");


}