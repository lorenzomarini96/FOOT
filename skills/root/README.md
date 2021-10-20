# ROOT


## Links

- [root](https://root.cern)
- [tutorial](https://root.cern/doc/master/group__Tutorials.html)
    - [histogram](https://root.cern/doc/master/group__tutorial__hist.html)
    - [tree](https://root.cern/doc/master/group__tutorial__tree.html)
    - [fit](https://root.cern/doc/master/group__tutorial__fit.html)
    - [RooFit](https://root.cern/doc/master/group__tutorial__roofit.html)
    - [graphs](https://root.cern/doc/master/group__tutorial__graphs.html)
    - [Math](https://root.cern/doc/master/group__tutorial__math.html)



## Basic knowledge

### ROOT files

- ROOT files contain C++ objects that are stored to disk.
- You can open files when starting ROOT:

  ```
  $ root file.root
  ```
  
- ROOT files often contain columnar data.
- With a **TBrowser** you can browse all ROOT objects within a ROOT file.
- You can create it with rootbrowse or for instance as part of a ROOT session:

  ```
   $ root file.root
   root[0] TBrowser
  ```
  
### Tree

- A TTree represents a columnar **dataset**.
- Any C++ type can be stored in its **columns**.
- A TTree, often called **tree**, consists of a list of independent columns or **branches**, represented by the **TBranch** class.
- The ROOT user can decide to make one single branch and serialize one or to make several branches.


##### Further insights:

- [ROOT files](https://root.cern/manual/root_files/)
- [TTree Class Reference](https://root.cern/doc/master/classTTree.html)


#### MakeClass()

- ROOT has a **MakeClass** command which generates a `.C` and `.h` file to run over a pre-existing tree structure.
- Open up one of the data files (the files with the `*.root` extension) and do `MakeClass` on it.
- You will have created a new macro that works on any tree of the same structure as the data file.
- Assuming to have a **data file** in your working directory called `"filename.root"` and within that data file is a **TTree** with the name `TreeName`
(you can find this out using the TBrowser)
- Run `MakeClass` the following way in the root session:
  - `root[0] TFile *f = new TFile("filename.root");`
  - `root[1] TreeName->MakeClass("NewClassName");`
  
- This create **two files** in the directory you are working:`NewClassName.C` and `NewClassName.h`.
  - `NewClassName.h` will have all the variables declared that are stored in the original data file
  - `NewClassName.C` will contain the skeleton of code that you would need to be able to *loop over all the entries* in the data file. 
  
 ```cpp
 
  void NewClassName::Loop()
  {
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
  ```
  
  What you actually do with that data is then your own choice and can be added to the code inside `NewClassName.C`.
 
    
  ##### How to use

  ```
  $ root -l
  root[0] .L NewClassName.C  // “L”oad the file NewClassName.C
  root[1] NewClassName t     // Create an object of type NewClassName
  root[2] t.Loop()           // Access the “Loop” method (where your analysis code is)
  ```
