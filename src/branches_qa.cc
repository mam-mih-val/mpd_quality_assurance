//
// Created by mikhail on 9/21/20.
//

#include "branches_qa.h"

namespace AnalysisTree{
void AddEventHeaderQA(QA::Task* qa_manager) {
  qa_manager->AddH1({"VTX_{z}, mm", {"RecoEvent.", "vtx_z"}, {500, -0.1, 0.1}});
  qa_manager->AddH1({"VTX_{x}, mm", {"RecoEvent.", "vtx_x"}, {500, -0.1, 0.1}});
  qa_manager->AddH1({"VTX_{y}, mm", {"RecoEvent.", "vtx_y"}, {500, -0.1, 0.1}});

  qa_manager->AddH2({"VTX_{x}, mm", {"RecoEvent.", "vtx_x"}, {500, -0.1, 0.1}},
                    {"VTX_{y}, mm", {"RecoEvent.", "vtx_y"}, {500, -0.1, 0.1}});
}

void AddTpcTracksQA(QA::Task* qa_manager) {
  qa_manager->AddH1({"Vertex tracks p_{T}, #frac{GeV}{c}", {"TpcTracks.", "pT"}, {500, 0.0, 2.5}});
  qa_manager->AddH1({"#chi^{2}_{RK}", {"TpcTracks.", "chi2"}, {250, 0.0, 250.0}});
  qa_manager->AddH1({"DCA_{x}", {"TpcTracks.", "dca_x"}, {500, -15.0, 15.0}});
  qa_manager->AddH1({"DCA_{y}", {"TpcTracks.", "dca_y"}, {500, -15.0, 15.0}});
  qa_manager->AddH1({"DCA_{z}", {"TpcTracks.", "dca_z"}, {500, -20.0, 20.0}});
  qa_manager->AddH1({"#phi, [rad]", {"TpcTracks.", "phi"}, {500, -4.0, 4.0}});
  qa_manager->AddH1({"m^{2} TOF [#frac{GeV^{2}}{c^{4}}]", {"TpcTracks.", "tof_mass2"}, {500, -0.5, 2.0}});

  Variable charged_p(
      "p_divide_z", {
          {"TpcTracks.", "charge"},
          {"TpcTracks.","p" }},
      [](std::vector<double> &var) { return var.at(1)/var.at(0); });
  Variable charged_pT(
      "p_divide_z", {
          {"TpcTracks.", "charge"},
          {"TpcTracks.","pT" }},
      [](std::vector<double> &var) { return var.at(1)/var.at(0); });

  qa_manager->AddH2({"#eta", {"TpcTracks.", "eta"}, {250, -2.5, 2.5}},
                    {"pt, #frac{GeV}{c}", {"TpcTracks.", "pT"}, {250, 0.0, 2.5}});

  qa_manager->AddH2({"p/z, [#frac{GeV}{c^{2}}]", charged_p, {250, -5.0, 5.0}},
                    {"#frac{dE}{dx} TPC", {"TpcTracks.", "dedx"}, {200, 0.0, 15000.0}});
  qa_manager->AddH2({"p_{T}/z, [#frac{GeV}{c^{2}}]", charged_pT, {250, -2.5, 2.5}},
                    {"m^{2} TOF [#frac{GeV^{2}}{c^{4}}]", {"TpcTracks.", "tof_mass2"}, {250, -0.5, 2.0}});

  Variable theta( "#theta, [rad]", {{"TpcTracks.", "eta"}},
                  []( const std::vector<double>& vars ){
                    auto eta = vars.at(0);
                    return 2*atan( exp( -eta ) );
                  } );

  qa_manager->AddH2({"#phi, [rad]", {"TpcTracks.", "phi"}, {315, -3.15, 3.15}},
                    {"#theta [rad]", theta, {315, 0.0, 3.15}});
}

void AddFhCalQA(QA::Task* qa_manager) {
  qa_manager->AddH1({"FHCal signal", {"FHCalModules.", "signal"}, {500, 0.0, 1.0}});
  qa_manager->AddH1({"FHCal module #varphi, [rad]", {"FHCalModules.", "phi"}, {500, -4.0, 4.0}});
  qa_manager->AddH2({"FHCal signal", {"FHCalModules.", "signal"}, {250, 0.0, 1.0}},
                    {"FHCal module id", {"FHCalModules.", "id"}, {90, 0.0, 90.0}});
}

void AddSimDataQA(QA::Task* qa_manager) {
  qa_manager->AddH1({"b, fm", {"McEvent.", "B"}, {200, 0.0, 20.0}});
  qa_manager->AddH1({"#Psi_{RP}, rad", {"McEvent.", "PhiRp"}, {315, 0.0, 6.30}});

  qa_manager->AddH1({"mass, [#frac{GeV}{c}", {"McTracks.", "mass"}, {500, 0.0, 2.0}});

  std::vector<Cuts*> particles {
      nullptr,
    new Cuts("protons", {{{"McTracks.", "pid"}, 2212}, {{"McTracks.", "mother_id"}, -1}}),
    new Cuts("pi_plus", {{{"McTracks.", "pid"}, 211}, {{"McTracks.", "mother_id"}, -1}}),
    new Cuts("pi_minus", {{{"McTracks.", "pid"}, -211}, {{"McTracks.", "mother_id"}, -1}}),
    new Cuts("k_plus", {{{"McTracks.", "pid"}, 321}, {{"McTracks.", "mother_id"}, -1}}),
    new Cuts("k_minus", {{{"McTracks.", "pid"}, -321}, {{"McTracks.", "mother_id"}, -1}}),
  };

  Variable momentum_resolution( "p_resolution", {{"McTracks.", "p"}, {"TpcTracks.", "p"}},
                                [](const std::vector<double> &vars){
                                  return (vars.at(0)-vars.at(1))/vars.at(0)*100.0;
                                });
  for( auto particle : particles  ) {
    qa_manager->AddH2(
        {"y_{cm}", {"McTracks.", "rapidity"}, {250, -3.0, 3.0}},
        {"pt_{p}, #frac{GeV}{c}", {"McTracks.", "pT"}, {250, 0.0, 2.5}},
        particle);

    qa_manager->AddH2({"GEN #eta", {"McTracks.", "eta"}, {250, -2.5, 2.5}},
                      {"RECO #eta", {"TpcTracks.", "eta"}, {250, -2.5, 2.5}},
                      particle);

    qa_manager->AddH2(
        {"GEN p [GeV/c]", {"McTracks.", "p"}, {300, 0.0, 3.0}},
        {"RECO p [GeV/c]", {"TpcTracks.", "p"}, {300, 0.0, 3.0}}, particle);

    qa_manager->AddH2(
        {"GEN p_{T} [GeV/c]", {"McTracks.", "pT"}, {250, 0.0, 2.5}},
        {"RECO p_{T} [GeV/c]", {"TpcTracks.", "pT"}, {250, 0.0, 2.5}},
        particle);

    qa_manager->AddH2(
        {"GEN #varphi [GeV/c]", {"McTracks.", "phi"}, {315, -3.15, 3.15}},
        {"RECO #varphi [GeV/c]", {"TpcTracks.", "phi"}, {315, -3.15, 3.15}},
        particle);

    qa_manager->AddProfile(
        {"GEN p, [GeV/c]", {"McTracks.", "p"}, {150, 0.0, 3.0}},
        {"#frac{|p_{g}-p_{r}|}{p_{g}}, (%)",
         momentum_resolution,
         {100, 0.0, 100.5}},
        particle);
  }
}
}