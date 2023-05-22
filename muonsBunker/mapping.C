// map information from all angles together, to have more data in total

// e.g. for theta=0.78 and phi= 0 : 237536 entries (with PU)

double shift (double theta){
  double h = 3.97;    // height between ground and roof
  double r = 1.1;     // thicknes of roof
  
  return tan(theta) * (h + r/2.);
}

double mappingX (double x, double theta, double phi){
    double newX = x - shift(theta) *cos(phi);
    return newX;
}

double mappingY (double y, double theta, double phi){
    double newY = y - shift(theta)*sin(phi);
    return newY;
}


int mapping (){
  
  TStopwatch t;
  t.Start();
  
  // s.t. dataframe will use MT
  ROOT::EnableImplicitMT();
  
  //------------------------parameters--------------------------
  const double length_tot = 1;     // of detector area [m]
  const double length_pxl = 0.03;  // length pixels
  double nPxl = length_tot/length_pxl;
  
  // for 2DHist
  Int_t nbinsx = nPxl*8;
  Double_t xlow = -4.;
  Double_t xup = 4.;
  Int_t nbinsy = nPxl;
  Double_t ylow = -0.5;
  Double_t yup = 0.5;
  
  // reading results
  ROOT::RDataFrame df_org("Hits","combinedAll.root");
  
  // specify angle
  double theta_theo0 = 0.;         // desired zenith angle [rad]
  double epsilon = 0.01;           // allowed discrepancy to zenith angle value [rad]
  double epsilon0 = 0.04;
  //double phi_theo = 0.;//3.141592645/2.;    // [-pi,pi]
 
  double z_theo = 0.10;
  double epsilon_z = 0.01;
  
  // create 2D histogram
  Double_t w = 400;
  Double_t h = 1200;
  auto c1 = new TCanvas("c","c",w,h);
  c1->Divide(1,2);
 
  auto df = df_org.Filter([&](double z){return abs(z-z_theo) < epsilon_z;},{"fz"});
  
  // make loop out of it
  auto histoTot = TH2D("h2 - EcoMug", Form("Added Histograms"), nbinsx, xlow, xup, nbinsy, ylow, yup);
    
  for(int j = 0; j < 20; j++){
    double phi_current = M_PI - j*(2.*M_PI/20.);
    auto df_phi_current = df.Filter([&](double z){return abs(z-phi_current) < epsilon;},{"phi"});
    for(int i = 1; i < 15; ++i){   // i < 150 for all angles (1.57 = 90°)
      double theta_current = i/100.;
      auto df_current = df_phi_current.Filter([&](double z){return abs(z-theta_current) < epsilon;},{"theta"});
      auto df_mapX = df_current.Define("newfx",mappingX,{"fx","theta","phi"});
      auto df_mapXY = df_mapX.Define("newfy",mappingY,{"fy","theta","phi"}); 
      auto hist = df_mapXY.Histo2D({"h2 - EcoMug", Form("Shifted - Detector Counts for #theta = %.2f to #theta = %.2f, Total hits on Detector = %llu",1./100.,149/100., *(df.Count())), nbinsx, xlow, xup, nbinsy, ylow, yup},"newfx","newfy");
    h;
      histoTot = histoTot + (*hist);
    }
  }

  // also draw result for vertical rays
  auto df_theta0 = df.Filter([&](double z){return abs(z-theta_theo0) < epsilon0;},{"theta"});
  
  c1->cd(1);
  auto histo0 = df_theta0.Histo2D({"h2 - EcoMug", Form("0 deg - Detector Counts for #theta = 0. #pm %.2f rad ", epsilon), nbinsx, xlow, xup, nbinsy, ylow, yup},"fx","fy");
  histo0->GetXaxis()->SetTitle("xDet [m]");
  histo0->GetYaxis()->SetTitle("yDet [m]");
  histo0->DrawClone("colz");
  
  histoTot = histoTot+ (*histo0);

  c1->cd(2);
  histoTot.GetXaxis()->SetTitle("xDet [m]");
  histoTot.GetYaxis()->SetTitle("yDet [m]");
  histoTot.DrawClone("colz");
  
  // how many hits on the detector in total
  std::cout << "Total Hits on detector = " << *(df.Count()) << std::endl;

  // For Debugging
  // simply use root debugging.root -> root[0] debTree->Scan() to check what is in the new dataframe
  // df4.Snapshot("debTree","debugging.root");
  
  t.Stop();
  t.Print();
 
  return 0;    
}
