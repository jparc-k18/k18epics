void ShowGraph(char* infile, char* name)
{
  TFile *file = new TFile(infile);
  if(file->IsZombie()) return;
  
  TTree *tree = (TTree*)file->Get("tree");

  if( tree->FindBranch(name) == 0 ){
    printf("No such branch name in tree: %s\n", name);
    TObjArray *list = tree->GetListOfLeaves();
    int num = list->GetSize();
    for(int i=0;i<num;i++){
      std::cout<<i<<" "<<list->At(i)->GetName()<<std::endl;
    }
    return;
  }

  int UnixTime;
  double val;

  tree->SetBranchAddress("UnixTime", &UnixTime);
  tree->SetBranchAddress(name, &val);
 
  int entries = tree->GetEntries();

  TGraph *g = new TGraph();
  int gp=0;

  for(int i=0;i<entries;i++){
    tree->GetEntry(i);

    if(val == -999999.999 || val == -999.9 ) continue;
    if( -0.0001 < val && val < 0.0001 ) continue;

    g->SetPoint(gp, UnixTime, val);
    gp++;
    
    //printf("%d %f\n",UnixTime,val);
  }


  g->GetXaxis()->SetTimeDisplay(1);
  g->GetXaxis()->SetLabelOffset(0.02);
  g->GetXaxis()->SetTimeFormat("#splitline{%Y/%m/%d}{  %H:%M:%S}");
  g->GetXaxis()->SetTimeOffset(0,"jpn");
  g->GetXaxis()->SetNdivisions(-503);
  g->Draw("al");
  
}
