#ifndef F05Field_h
#define F05Field_h 1

#include "globals.hh"
#include "G4ElectricField.hh"

class ElectricField : public G4ElectricField
{

public:

  ElectricField();
  virtual ~ElectricField();

  /// DoesFieldChangeEnergy() returns true.
  virtual G4bool DoesFieldChangeEnergy() const { return true; };

  /// GetFieldValue() returns the field value at a given point[].
  /// field is really field[6]: Bx,By,Bz,Ex,Ey,Ez.
  /// point[] is in global coordinates: x,y,z,t.
  virtual void GetFieldValue(const G4double Point[4], G4double* Bfield) const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
