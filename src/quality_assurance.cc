//
// Created by mikhail on 5/27/20.
//

#include <iostream>

#include <boost/program_options.hpp>

#include "branches_qa.h"

int main(int argv, char **argc){
  namespace po = boost::program_options;
  if( argv < 2 ){
    throw std::runtime_error( "No arguments are specified. Try ./run_qa --help for more information" );
  }
  std::string input_list;
  std::string output_file{"output.root"};
  int n_events{-1};
  po::options_description options("Options");
  options.add_options()
      ("help,h", "Help screen")
      ("MC,m", "MC data sample")
      ("input,i", po::value<std::string>(&input_list),
       "Input file list")
      ("output,o", po::value<std::string>(&output_file),
       "Name of output file")
      ("events,N", po::value<int>(&n_events),
       "Number of analysing events");
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argv, argc).options(options).run();
  po::store(parsed, vm);
  po::notify(vm);
  if (vm.count("help")){
    std::cout << options << std::endl;
    return 0;
  }
  bool is_mc=vm.count("MC");

  if( std::empty(input_list) )
    throw std::runtime_error( "Input file is not specified. Try ./run_qa --help for more information" );
  AnalysisTree::QA::Manager qa_manager({input_list}, {"aTree"});
  qa_manager.SetOutFileName(output_file);
  auto* qa_task = new AnalysisTree::QA::Task;

  AnalysisTree::AddEventHeaderQA(qa_task);
  AnalysisTree::AddTpcTracksQA(qa_task);
  AnalysisTree::AddFhCalQA(qa_task);
  if( is_mc )
    AnalysisTree::AddSimDataQA(qa_task);

  qa_manager.AddTask(qa_task);
  qa_manager.Init();
  qa_manager.Run(n_events);
  qa_manager.Finish();
  return 0;
}