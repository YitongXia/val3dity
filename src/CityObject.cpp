/*
  val3dity 

  Copyright (c) 2011-2017, 3D geoinformation research group, TU Delft  

  This file is part of val3dity.

  val3dity is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  val3dity is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with val3dity.  If not, see <http://www.gnu.org/licenses/>.

  For any information or further details about the use of val3dity, contact
  Hugo Ledoux
  <h.ledoux@tudelft.nl>
  Faculty of Architecture & the Built Environment
  Delft University of Technology
  Julianalaan 134, Delft 2628BL, the Netherlands
*/


#include "CityObject.h"
#include "definitions.h"
#include "input.h"
#include "validate_prim_toporel.h"

namespace val3dity
{

CityObject::CityObject(std::string theid, std::string thetype)
{
  _id = theid;
  _is_valid = -1;
  _type = thetype; // TODO : validate the type here?
}


CityObject::~CityObject()
{}


bool CityObject::validate(double tol_planarity_d2p, double tol_planarity_normals, double tol_overlap) 
{
  if (_is_valid != -1)
    return _is_valid;
  bool bValid = Feature::validate_generic(tol_planarity_d2p, tol_planarity_normals, tol_overlap);
  //-- Building
  if ( (bValid == true) && (this->_type == "Building") )
    bValid = validate_building(tol_overlap);
  _is_valid = bValid;
  return bValid;
}


bool CityObject::validate_building(double tol_overlap)
{
  bool bValid = true;
  if (_lsPrimitives.size() == 0)
    return bValid;
  std::clog << "--- Interactions between BuildingParts ---" << std::endl;
  std::vector<Error> lsErrors;
  if (do_primitives_overlap(_lsPrimitives, 601, lsErrors, tol_overlap) == true)
  {
    bValid = false;
    std::clog << "Error: overlapping building parts" << std::endl;
    for (auto& e : lsErrors)
      this->add_error(e.errorcode, e.info1, e.info2);
  }
  return bValid;
}


bool CityObject::is_valid() 
{
  if (_is_valid == 1) 
    return true;
  else
    return false;
}


std::string CityObject::get_type() {
  return _type;
}


} // namespace val3dity
