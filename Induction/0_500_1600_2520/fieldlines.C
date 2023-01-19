#include <iostream>

#include <TApplication.h>
#include <TCanvas.h>
#include "Garfield/Sensor.hh"
#include "Garfield/ComponentAnsys123.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/MediumMagboltz.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  TApplication app("app", &argc, argv);

  // Load the field map.
  ComponentAnsys123 fm;
  fm.Initialise("ELIST.lis", "NLIST.lis", "MPLIST.lis", "PRNSOL.lis", "mm");
  fm.EnableMirrorPeriodicityX();
  fm.EnableMirrorPeriodicityY();
  fm.PrintRange();

  // Setup the gas.
  MediumMagboltz gas("ar", 70., "co2", 30.);

  // Associate the gas with the corresponding field map material. 
  fm.SetGas(&gas);
  fm.PrintMaterials();

  // Dimensions of the GEM [cm]
  constexpr double pitch = 0.045;
  const double xmin = -0.5 * pitch;
  const double xmax =  0.5 * pitch;
  const double zmin = -0.25;
  const double zmax =  0.5;

  Sensor sensor;
  sensor.AddComponent(&fm);
  // Set the plot limits in the current viewing plane.
  sensor.SetArea(-0.5 * pitch, -0.5 * pitch, -0.1,
                  0.5 * pitch,  0.5 * pitch,  0.1);

  ViewField fieldView;
  fieldView.SetComponent(&fm);
  fieldView.SetPlaneXZ();
  
  fieldView.SetArea(xmin, zmin, xmax, zmax);
  fieldView.SetVoltageRange(-1600.0, -400.0);
  fieldView.GetCanvas()->SetLeftMargin(0.16);
  //fieldView.Plot("v", "CONT1");
  fieldView.PlotContour();

  std::vector<double> xf;
  std::vector<double> yf;
  std::vector<double> zf;
  fieldView.EqualFluxIntervals(xmin, 0, 0.99*zmax,xmax, 0, 0.99*zmax, xf, yf, zf, 20);

  fieldView.PlotFieldLines(xf, yf, zf, true, false);
  app.Run(true);
}
