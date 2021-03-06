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

#include "oc3_empire.hpp"
#include "oc3_empirecity.hpp"
#include "oc3_variant.hpp"
#include "oc3_saveadapter.hpp"
#include "oc3_stringhelper.hpp"

class Empire::Impl
{
public:
  EmpireCities cities;
};

Empire::Empire() : _d( new Impl )
{

}

EmpireCities Empire::getCities() const
{
  return _d->cities;
}

Empire::~Empire()
{

}

void Empire::initialize( const io::FilePath& filename )
{
  VariantMap cities = SaveAdapter::load( filename.toString() );

  if( cities.empty() )
  {
    StringHelper::debug( 0xff, "Can't load cities model from %s", filename.toString().c_str() );
    return;
  }

  for( VariantMap::iterator it=cities.begin(); it != cities.end(); it++ )
  {
    VariantMap cityOptions = it->second.toMap();
    Point location = cityOptions.get( "location" ).toPoint();
    EmpireCityPtr city = addCity( it->first );
    city->setLocation( location );
  }
}

EmpireCityPtr Empire::addCity( const std::string& name )
{
  EmpireCities::iterator it = _d->cities.begin();

  for( ; it != _d->cities.end(); it++ )
  {
    if( (*it)->getName() == name )
    {
      _OC3_DEBUG_BREAK_IF( "City already exist" );
      StringHelper::debug( 0xff, "City %s already exist", name.c_str() );
      return *it;
    }
  }

  EmpireCityPtr ret = new EmpireCity( name );
  _d->cities.push_back( ret );

  return ret;
}

EmpirePtr Empire::create()
{
  EmpirePtr ret( new Empire() );
  ret->drop();

  return ret;
}