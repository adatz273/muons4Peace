
int analysis (){
  //------------------------parameters--------------------------
  const double length_tot = 1;     // of detector area [m]
  const double length_pxl = 0.01;  // length pixels
  double nPxl = length_tot/length_pxl;
  
  // for 2DHist
  Int_t nbinsx = nPxl;
  Double_t xlow = -0.5;
  Double_t xup = 0.5;
  Int_t nbinsy = nPxl;
  Double_t ylow = -0.5;
  Double_t yup = 0.5;
  
  // reading results
  auto filename = "resultsWedJul271303.root";
  auto treeName = "Hits";
  
  // specify angle
  double theta_theo = 0.;     // desired zenith angle [rad]
  double epsilon = 0.2;       // allowed discrepancy to zenith angle value [rad]
  //---------------------------------------------------------
 
  
  // read file content to dataframe
  ROOT::RDataFrame df(treeName,filename);
       
  // filter for zenith angle
  auto df_zenithCut = df.Filter([&](double z){return abs(z-theta_theo) < epsilon;},{"theta"});
    
  // create 2D histogram
  auto c1 = new TCanvas();
  auto histo = df_zenithCut.Histo2D({"h2", "fancy title", nbinsx, xlow, xup, nbinsy, ylow, yup},"fx","fy");
  histo->GetXaxis()->SetTitle("X");
  histo->DrawClone("colz");
    
  // For Debugging
  // simply use root debugging.root -> root[0] debTree->Scan(); to check what is in the new dataframe
  df_zenithCut.Snapshot("debTree","debugging.root");

  return 0;    
}
