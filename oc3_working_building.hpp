// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __OPENCAESAR3_WORKINGBUILDING_H_INCLUDED__
#define __OPENCAESAR3_WORKINGBUILDING_H_INCLUDED__

#include "oc3_building.hpp"

/** Building where people work */
class WorkingBuilding : public Building
{
public:
  WorkingBuilding(const BuildingType type, const Size& size);

  void setMaxWorkers(const int maxWorkers);
  int getMaxWorkers() const;

  void setWorkers(const int currentWorkers);
  void addWorkers( const int workers );

  int getWorkers() const;

  void setActive(const bool value);  // if false then this building is stopped
  bool isActive();

  void save( VariantMap& stream) const;
  void load( const VariantMap& stream);

private:
  int _currentWorkers;
  int _maxWorkers;
  bool _isActive;
};

#endif //__OPENCAESAR3_WORKINGBUILDING_H_INCLUDED__