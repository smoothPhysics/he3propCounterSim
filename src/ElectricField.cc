#include "ElectricField.hh"

#include "G4SystemOfUnits.hh"

ElectricField::ElectricField() : G4ElectricField()
{}


ElectricField::~ElectricField()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ElectricField::GetFieldValue( const G4double Point[4], G4double* Efield ) const
{
  // Point[0],Point[1],Point[2] are x-, y-, z-cordinates, Point[3] is time

  const G4double Er = 559.52*(volt/cm);

  G4double Ex,Ey;
 
  G4double posR = std::sqrt(std::pow(Point[0],2) + std::pow(Point[1],2));
  G4double cos_theta, sin_theta;

    cos_theta = Point[0]/(G4double)posR;
    sin_theta = Point[1]/(G4double)posR;
    Ex = Er*cos_theta;//apply radial electric field
    Ey = Er*sin_theta;
  
  Bfield[0]=0.0;
  Bfield[1]=0.0;
  Bfield[2]=0.0;

  Bfield[3]=Ex;
  Bfield[4]=Ey;
  Bfield[5]=0.0;

  return;
}
