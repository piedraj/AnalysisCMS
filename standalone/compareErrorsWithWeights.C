//------------------------------------------------------------------------------
//
// root -l -b -q compareErrorsWithWeights
//
//------------------------------------------------------------------------------
const Int_t nbins    = 100;
const Int_t nprocess =   3;

const Double_t weight[nprocess] = {
  0.011,
  0.520,
  0.073
};

TH1F* h[nprocess];


//------------------------------------------------------------------------------
//
// main
//
//------------------------------------------------------------------------------
void compareErrorsWithWeights()
{
  // Print information
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" Comparison of different methods that estimate the error of the sum of\n");
  printf(" three dummy MC processes with different weights.\n");
  printf("\n");
  printf(" Method 1 estimates the error as the square root of the combined yield.\n");
  printf("\n");
  printf(" Method 2 estimates the error as the propagated sum of the individual\n");
  printf(" MC samples, considering the weights as constants.\n");
  printf("\n");
  printf(" Method 3 estimates the error as the quadratic sum of the combined yield\n");
  printf(" bin-by-bin errors.\n");
  printf("\n");
  printf("----------------------------------------------------------\n");


//------------------------------------------------------------------------------
  // Fill histograms
  //----------------------------------------------------------------------------
  for (Int_t i=0; i<nprocess; i++) {

    h[i] = new TH1F(Form("h%d", i), "", nbins, 0, 100);

    h[i]->Sumw2();

    for (Int_t j=0; j<1000; j++) {

      Double_t content = 1e-4 * j*j;

      h[i]->Fill(content, weight[i]);
    }
  }

  
  // Compute the errors
  //----------------------------------------------------------------------------
  TH1F* hsum = (TH1F*)h[0]->Clone("hsum");

  hsum->Add(h[1]);
  hsum->Add(h[2]);

  Double_t method1_value = hsum->GetSumOfWeights();
  Double_t method1_error = sqrt(hsum->GetSumOfWeights());

  Double_t method2_value = 0;
  Double_t method2_error = 0;
  
  Double_t method3_value = 0;
  Double_t method3_error = 0;

  for (Int_t i=0; i<nprocess; i++) {

    method2_value += h[i]->GetEntries() * weight[i];
    method2_error += h[i]->GetEntries() * weight[i] * weight[i];
  }
  
  for (Int_t j=1; j<=nbins; j++) {

    method3_value += hsum->GetBinContent(j);
    method3_error += hsum->GetBinError(j) * hsum->GetBinError(j);
  }

  method2_error = sqrt(method2_error);
  method3_error = sqrt(method3_error);


  // Print results
  //----------------------------------------------------------------------------
  printf("\n");
  printf(" Method 1 yield: %.4f +- %.4f\n", method1_value, method1_error);
  printf(" Method 2 yield: %.4f +- %.4f\n", method2_value, method2_error);
  printf(" Method 3 yield: %.4f +- %.4f\n", method3_value, method3_error);
  printf("\n");
}
