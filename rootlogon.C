{
  gInterpreter->ExecuteMacro("PaperStyle.C");
  gInterpreter->LoadMacro   ("DrawFunctions.C+");

  gStyle->SetEndErrorSize    (  5);
  gStyle->SetHatchesLineWidth(  1);
  gStyle->SetHatchesSpacing  (0.7);
  gStyle->SetOptStat         (  0);
  gStyle->SetPalette         (1,0);
}
