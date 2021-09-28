# mpd_quality_assurance
QA for MPD data in AnalysisTree format

# Requirement

* CMake 3.15
* ROOT v6-20-04 built with C++17
* AnalysisTree built with C++17 (see https://docs.google.com/document/d/1ztejoeJ45Aqdgq3a4m1DXTg4A1hUR3SDqk4ybYkboLA/edit for instructions)
* AnalysisTreeQA built with C++17 (see https://github.com/HeavyIonAnalysis/AnalysisTreeQA for instructions)

# Building

Clone this repository with
```
  git clone https://github.com/mam-mih-val/hades_quality_assurance
```
Create build directory
```
  cd hades_quality_assurance
  mkdir build
  cd build
```
Source root environment
```
  source /path/to/root/install/bin/thisroot.sh
``` 
Export AnalysisTree library (see https://github.com/HeavyIonAnalysis/AnalysisTree)
```
  export AnalysisTree_DIR=/path/to/AnalysisTree/install/lib/cmake/AnalysisTree/
```
Export AnalysisTreeQA library built with tag 1.0.1 (see https://github.com/HeavyIonAnalysis/AnalysisTreeQA)
```
  export AnalysisTreeQA_DIR=/path/to/AnalysisTreeQA/install/lib/cmake/AnalysisTreeQA/
```
Build the project
```
  cmake ..
  make -j
```

# Usage
To use the program run from build directory
```
  ./run_qa -i path/to/file.list
```
To get list of all availible options use -h option
```
  ./run_qa -h
```
Example of file list you can find in "lists" directory.

Detailed description of how to work with AnalysisTree is stored in https://docs.google.com/document/d/1pWh8T4xAjVvJJyB1OQYLRzVW_HHinZ_uxHHPz_1rfQs/edit.
