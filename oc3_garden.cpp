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

#include "oc3_garden.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_tile.hpp"
#include "oc3_scenario.hpp"
#include "oc3_city.hpp"

Garden::Garden() : Construction(B_GARDEN, Size(1) )
{
  // always set picture to 110 (tree garden) here, for sake of building preview
  // actual garden picture will be set upon building being constructed
  setPicture( Picture::load( ResourceGroup::entertaiment, 110 ) ); // 110 111 112 113
}

void Garden::setTerrain(TerrainTile &terrain)
{
  bool isMeadow = terrain.isMeadow();
  terrain.clearFlags();
  terrain.setOverlay(this);
  terrain.setBuilding(true); // are gardens buildings or not???? try to investigate from original game
  terrain.setGarden(true);
  terrain.setMeadow(isMeadow);    
}

bool Garden::isWalkable() const
{
  return true;
}

bool Garden::isNeedRoadAccess() const
{
  return false;
}

void Garden::build( const TilePos& pos )
{
  // this is the same arrangement of garden tiles as existed in C3
  int theGrid[2][2] = {{113, 110}, {112, 111}};

  Construction::build( pos );
  setPicture( Picture::load( ResourceGroup::entertaiment, theGrid[pos.getI() % 2][pos.getJ() % 2] ) );

  PtrTilesList tilesAround = Scenario::instance().getCity()->getTilemap().getRectangle( getTilePos() - TilePos( 1, 1), 
                                                                                        getTilePos() + TilePos( 1, 1 ) );
  for( PtrTilesList::iterator it=tilesAround.begin(); it != tilesAround.end(); it++ )
  {
    GardenPtr garden = (*it)->getTerrain().getOverlay().as<Garden>(); 
    if( garden.isValid() )
    {
      garden->update();
    }
  }
}

void Garden::update()
{
  PtrTilesArea nearTiles = Scenario::instance().getCity()->getTilemap().getFilledRectangle( getTilePos(), Size(2) );

  bool canGrow2squareGarden = true;
  for( PtrTilesArea::iterator it=nearTiles.begin(); it != nearTiles.end(); it++ )
  {
    GardenPtr garden = (*it)->getTerrain().getOverlay().as<Garden>();
    canGrow2squareGarden &= (garden.isValid() && garden->getSize().getArea() <= 2 );
  }

  if( canGrow2squareGarden )
  {   
    for( PtrTilesArea::iterator it=nearTiles.begin(); it != nearTiles.end(); it++ )
    {
      LandOverlayPtr overlay = (*it)->getTerrain().getOverlay();

      //not delete himself
      if( overlay != this && overlay.isValid() )
      {
        overlay->deleteLater();
      }
    }

    setSize( 2 );
    Construction::build( getTilePos() );
    setPicture( Picture::load(  ResourceGroup::entertaiment, 114 + rand() % 3 ));
  }
}