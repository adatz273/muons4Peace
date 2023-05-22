// draw hits on detector for two angles and two files (e.g. one with obstacle and one w.o. obstacle)





int analysis5 (){
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
  std::array<std::string, 2> filenames = {"resultsMonAug10949.root","resultsMonAug11131.root"};
  //auto filename = "resultsSunJul312107.root";
  auto treeName = "Hits";
  
  // specify angle
  double theta1_theo = 0.0 ;    // desired zenith angle [rad]
  double theta2_theo = 0.78;    // 
  double epsilon = 0.20;        // allowed discrepancy to zenith angle value [rad]
 
  // create 2D histogram
  Double_t w = 800;
  Double_t h = 800;
  auto c1 = new TCanvas("c","c",w,h);
  c1->Divide(2,2);
 
  int counter = 1;
  for(const auto& filename : filenames){
    // read file content to dataframe
    ROOT::RDataFrame df(treeName,filename);
       
    // filter for zenith angle
    auto df_zenithCut1 = df.Filter([&](double z){return abs(z-theta1_theo) < epsilon;},{"theta"});
    auto df_zenithCut2 = df.Filter([&](double z){return abs(z-theta2_theo) < epsilon;},{"theta"});
         
    c1->cd(counter);
    counter++;
    //std::string_view plotTitle= "theta = " + std::to_string(theta_theo) + "+/-" + std::to_string(epsilon);
    auto histo1 = df_zenithCut1.Histo2D({"h1 - EcoMug", Form("Detector Counts for #theta = %.2f #pm %.2f rad",theta1_theo, epsilon), nbinsx, xlow, xup, nbinsy, ylow, yup},"fx","fy");
    histo1->GetXaxis()->SetTitle("xDet [m]");
    histo1->GetYaxis()->SetTitle("yDet [m]");
    histo1->DrawClone("colz");
  
    c1->cd(counter);
    auto histo2 = df_zenithCut2.Histo2D({"h2 - EcoMug", Form("Detector Counts for #theta = %.2f #pm %.2f rad",theta2_theo, epsilon), nbinsx, xlow, xup, nbinsy, ylow, yup},"fx","fy");
    histo2->GetXaxis()->SetTitle("xDet [m]");
    histo2->GetYaxis()->SetTitle("yDet [m]");
    histo2->DrawClone("colz");
    counter++;
    
  // For Debugging
  // simply use root debugging.root -> root[0] debTree->Scan(); to check what is in the new dataframe
  df_zenithCut2.Snapshot("debTree","debugging.root");
  }
 
  return 0;    
}
