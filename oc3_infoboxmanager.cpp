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
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com

#include "oc3_infoboxmanager.hpp"
#include "oc3_gui_info_box.hpp"
#include "oc3_guienv.hpp"
#include "oc3_road.hpp"
#include "oc3_building_prefecture.hpp"
#include "oc3_tile.hpp"
#include "oc3_building_service.hpp"
#include "oc3_building_engineer_post.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_house.hpp"
#include "oc3_gettext.hpp"

class InfoBoxHouseCreator : public InfoboxCreator
{
public:
  GuiInfoBox* create( Widget* parent, const Tile& tile )
  {
    HousePtr house = tile.getTerrain().getOverlay().as<House>();
    if( house->getNbHabitants() > 0 )
    {
      return new InfoBoxHouse( parent, tile );
    }
    else
    {
      return new InfoBoxFreeHouse( parent, tile );
    }
  }
};

class ServiceBaseInfoboxCreator : public InfoboxCreator
{
public:
  ServiceBaseInfoboxCreator( const std::string& caption,
                             const std::string& descr,
                             bool drawWorkers=false )
  {
    title = caption;
    text = descr;
    isDrawWorkers = drawWorkers;
  }

  GuiInfoBox* create( Widget* parent, const Tile& tile )
  {
    Size  size = parent->getSize();
    WorkingBuildingPtr building = tile.getTerrain().getOverlay().as<WorkingBuilding>();
    if( building.isValid() )
    {
      InfoBoxWorkingBuilding* infoBox = new InfoBoxWorkingBuilding( parent, building );
      infoBox->setPosition( Point( (size.getWidth() - infoBox->getWidth()) / 2, size.getHeight() - infoBox->getHeight()) );

      infoBox->setTitle( title );
      infoBox->setText( text );
      return infoBox;
    }
    
    return 0;
  }

  std::string title, text;
  bool isDrawWorkers;
};

class InfoBoxBasicCreator : public InfoboxCreator
{
public:
  InfoBoxBasicCreator( const std::string& caption,
                       const std::string& desc )
  {
    title = caption;
    text = desc;
  }

  GuiInfoBox* create( Widget* parent, const Tile& tile )
  {
    Size  size = parent->getSize();
    InfoBoxBasic* infoBox = new InfoBoxBasic( parent, tile );
    infoBox->setPosition( Point( (size.getWidth() - infoBox->getWidth()) / 2, 
                                  size.getHeight() - infoBox->getHeight()) );

    infoBox->setTitle( title );
    infoBox->setText( text );
    return infoBox;
  }

  std::string title, text;
};

class InfoBoxManager::Impl
{
public:
    GuiEnv* gui;
    bool showDebugInfo;

    typedef std::map< BuildingType, InfoboxCreator* > InfoboxCreators;
    std::map< std::string, BuildingType > name2typeMap;
    InfoboxCreators constructors;
};

InfoBoxManagerPtr InfoBoxManager::create( GuiEnv* gui )
{
    InfoBoxManagerPtr ret( new InfoBoxManager() );
    ret->_d->gui = gui;
    ret->_d->showDebugInfo = true;

    return ret;
}

InfoBoxManager::InfoBoxManager() : _d( new Impl )
{

  addCreator( B_ROAD, OC3_STR_EXT(B_ROAD), new BaseInfoboxCreator<InfoBoxLand>() );
  addCreator( B_HOUSE, OC3_STR_EXT(B_HOUSE), new InfoBoxHouseCreator() );
  addCreator( B_PREFECTURE, OC3_STR_EXT(B_PREFECTURE), new ServiceBaseInfoboxCreator( "##prefecture_title##", "##prefecture_text##") );
  addCreator( B_ENGINEER_POST, OC3_STR_EXT(B_ENGINEER_POST), new ServiceBaseInfoboxCreator( "##engineering_post_title##", "##engineering_post_text##" ) ); 
  addCreator( B_WELL, OC3_STR_EXT(B_WELL), new ServiceBaseInfoboxCreator( "##well_title##", "##well_text##" ) );
  addCreator( B_DOCTOR, OC3_STR_EXT(B_DOCTOR), new ServiceBaseInfoboxCreator( "##doctor_title##", "##doctor_text##" ) );
  addCreator( B_BATHS, OC3_STR_EXT(B_BATHS), new ServiceBaseInfoboxCreator( "##baths_title##", "##baths_text##" ) );
  addCreator( B_BARBER, OC3_STR_EXT(B_BARBER), new ServiceBaseInfoboxCreator( "##barber_title##", "##barber_text##" ) );
  addCreator( B_HOSPITAL, OC3_STR_EXT(B_HOSPITAL), new ServiceBaseInfoboxCreator( "##hospital_title##", "##hospital_text##" ) );
  addCreator( B_FOUNTAIN, OC3_STR_EXT(B_FOUNTAIN), new ServiceBaseInfoboxCreator( "##fontaun_title##", "##fontaun_text##" ) );
  addCreator( B_MARKET, OC3_STR_EXT(B_MARKET), new BaseInfoboxCreator<GuiInfoMarket>() );
  addCreator( B_GRANARY, OC3_STR_EXT(B_GRANARY), new BaseInfoboxCreator<GuiInfoGranary>() );
  addCreator( B_GRAPE_FARM, OC3_STR_EXT(B_GRAPE_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_WHEAT_FARM, OC3_STR_EXT(B_WHEAT_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_VEGETABLE_FARM, OC3_STR_EXT(B_VEGETABLE_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_OLIVE_FARM, OC3_STR_EXT(B_OLIVE_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_FRUIT_FARM, OC3_STR_EXT(B_FRUIT_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_WAREHOUSE, OC3_STR_EXT(B_WAREHOUSE), new BaseInfoboxCreator<InfoBoxWarehouse>() );
  addCreator( B_PIG_FARM, OC3_STR_EXT(B_PIG_FARM), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_TEMPLE_CERES, OC3_STR_EXT(B_TEMPLE_CERES), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_TEMPLE_MARS, OC3_STR_EXT(B_TEMPLE_MARS), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_TEMPLE_NEPTUNE, OC3_STR_EXT(B_TEMPLE_NEPTUNE), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_TEMPLE_VENUS, OC3_STR_EXT(B_TEMPLE_VENUS), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_TEMPLE_MERCURE, OC3_STR_EXT(B_TEMPLE_MERCURE), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_BIG_TEMPLE_CERES, OC3_STR_EXT(B_BIG_TEMPLE_CERES), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_BIG_TEMPLE_MARS, OC3_STR_EXT(B_BIG_TEMPLE_MARS), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_BIG_TEMPLE_NEPTUNE, OC3_STR_EXT(B_BIG_TEMPLE_NEPTUNE), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_BIG_TEMPLE_VENUS, OC3_STR_EXT(B_BIG_TEMPLE_VENUS), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_BIG_TEMPLE_MERCURE, OC3_STR_EXT(B_BIG_TEMPLE_MERCURE), new BaseInfoboxCreator<InfoBoxTemple>() );
  addCreator( B_TEMPLE_ORACLE, OC3_STR_EXT(B_TEMPLE_ORACLE), new BaseInfoboxCreator<InfoBoxTemple>() ); 
  addCreator( B_SCHOOL, OC3_STR_EXT(B_SCHOOL), new ServiceBaseInfoboxCreator( _("##school_title##"), _("##school_text##") ));
  addCreator( B_COLLEGE, OC3_STR_EXT(B_COLLEGE), new ServiceBaseInfoboxCreator( _("##college_title##"), _("##college_text##") ));
  addCreator( B_LIBRARY, OC3_STR_EXT(B_LIBRARY), new ServiceBaseInfoboxCreator( _("##library_title##"), _("##library_text##") ));
  addCreator( B_GARDEN, OC3_STR_EXT(B_GARDEN), new InfoBoxBasicCreator( _("building_garden"), _("##garden_desc##")) );
  addCreator( B_STATUE1, OC3_STR_EXT(B_STATUE1), new InfoBoxBasicCreator( _("building_statue_small"), _("##statue_desc##")) );
  addCreator( B_STATUE2, OC3_STR_EXT(B_STATUE2), new InfoBoxBasicCreator( _("building_statue_middle"), _("##statue_desc##")) );
  addCreator( B_STATUE3, OC3_STR_EXT(B_STATUE3), new InfoBoxBasicCreator( _("building_statue_big"), _("##statue_desc##")) );
  addCreator( B_PLAZA, OC3_STR_EXT(B_PLAZA), new BaseInfoboxCreator<InfoBoxLand>() );
  addCreator( B_NONE, OC3_STR_EXT(B_NONE), new BaseInfoboxCreator<InfoBoxLand>() );
  addCreator( B_POTTERY, OC3_STR_EXT(B_POTTERY), new BaseInfoboxCreator<GuiInfoFactory>() );
  addCreator( B_WEAPONS_WORKSHOP, OC3_STR_EXT(B_WEAPONS_WORKSHOP), new BaseInfoboxCreator<GuiInfoFactory>() );
  addCreator( B_FURNITURE, OC3_STR_EXT(B_FURNITURE), new BaseInfoboxCreator<GuiInfoFactory>() );
  addCreator( B_CLAY_PIT, OC3_STR_EXT(B_CLAY_PIT), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_TIMBER_YARD, OC3_STR_EXT(B_TIMBER_YARD), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_MARBLE_QUARRY, OC3_STR_EXT(B_MARBLE_QUARRY), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_IRON_MINE, OC3_STR_EXT(B_IRON_MINE), new BaseInfoboxCreator<InfoBoxRawMaterial>() );
  addCreator( B_WINE_WORKSHOP, OC3_STR_EXT(B_WINE_WORKSHOP), new BaseInfoboxCreator<GuiInfoFactory>() );
  addCreator( B_OIL_WORKSHOP, OC3_STR_EXT(B_OIL_WORKSHOP), new BaseInfoboxCreator<GuiInfoFactory>() );
  addCreator( B_SENATE, OC3_STR_EXT(B_SENATE), new BaseInfoboxCreator<InfoBoxSenate>() );
  addCreator( B_THEATER, OC3_STR_EXT(B_THEATER), new ServiceBaseInfoboxCreator( _("##theater_title##"), _("##theater_text##")) );
  addCreator( B_ACTOR_COLONY, OC3_STR_EXT(B_ACTOR_COLONY), new ServiceBaseInfoboxCreator( _("##actor_colony_title##"), _("##actor_colony_text##")) );
  addCreator( B_AMPHITHEATER, OC3_STR_EXT(B_AMPHITHEATER), new ServiceBaseInfoboxCreator( _("##amphitheater_title##"), _("##amphitheater_text##")) );
  addCreator( B_GLADIATOR_SCHOOL, OC3_STR_EXT(B_GLADIATOR_SCHOOL), new ServiceBaseInfoboxCreator( _("##gladiator_school_title##"), _("##gladiator_school_text##")) );
  addCreator( B_COLLOSSEUM, OC3_STR_EXT(B_COLLOSSEUM), new ServiceBaseInfoboxCreator( _("##colloseum_title##"), _("##colloseum_text##")) );
  addCreator( B_LION_HOUSE, OC3_STR_EXT(B_LION_HOUSE), new ServiceBaseInfoboxCreator( _("##lion_house_title##"), _("##lion_house_text##")) );
  addCreator( B_HIPPODROME, OC3_STR_EXT(B_HIPPODROME), new ServiceBaseInfoboxCreator( _("##hippodrome_title##"), _("##hippodrome_text##")) );
  addCreator( B_CHARIOT_MAKER, OC3_STR_EXT(B_CHARIOT_MAKER), new ServiceBaseInfoboxCreator( _("##chario_maker_title##"), _("##chario_maker_text##")) );
  addCreator( B_FORUM, OC3_STR_EXT(B_FORUM), new ServiceBaseInfoboxCreator( _("##forum_title##"), _("##forum_text##")) );
  addCreator( B_GOVERNOR_HOUSE, OC3_STR_EXT(B_GOVERNOR_HOUSE), new ServiceBaseInfoboxCreator( _("##governor_house_title##"), _("##governonr_house_text##")) );
  addCreator( B_GOVERNOR_VILLA, OC3_STR_EXT(B_GOVERNOR_VILLA), new ServiceBaseInfoboxCreator( _("##governor_villa_title##"), _("##governonr_villa_text##")) );
  addCreator( B_GOVERNOR_PALACE, OC3_STR_EXT(B_GOVERNOR_PALACE), new ServiceBaseInfoboxCreator( _("##governor_palace_title##"), _("##governonr_palace_text##")) );
  addCreator( B_HIGH_BRIDGE, OC3_STR_EXT(B_HIGH_BRIDGE), new InfoBoxBasicCreator( _("##high_bridge_title##"), _("##high_bridge_text##")) );
  addCreator( B_LOW_BRIDGE, OC3_STR_EXT(B_LOW_BRIDGE), new InfoBoxBasicCreator( _("##low_bridge_title##"), _("##low_bridge_text##")) );
}

InfoBoxManager::~InfoBoxManager()
{

}

void InfoBoxManager::showHelp( const Tile& tile )
{
  LandOverlayPtr overlay = tile.getTerrain().getOverlay();
  BuildingType type;

  if( _d->showDebugInfo )
  {
    StringHelper::debug( 0xff, "Tile debug info: dsrbl=%d", tile.getTerrain().getDesirability() ); 
  }

  type = overlay.isNull() ? B_NONE : overlay->getType();

  Impl::InfoboxCreators::iterator findConstructor = _d->constructors.find( type );

  GuiInfoBox* infoBox = findConstructor != _d->constructors.end() 
                                  ? findConstructor->second->create( _d->gui->getRootWidget(), tile )
                                  : 0;
  
  if( infoBox && infoBox->isAutoPosition() )
  {
    Size rSize = _d->gui->getRootWidget()->getSize();
    int y = ( _d->gui->getCursorPos().getY() < rSize.getHeight() / 2 ) 
                ? rSize.getHeight() - infoBox->getHeight() - 5
                : 30;
    Point pos( ( rSize.getWidth() - infoBox->getWidth() ) / 2, y );

    infoBox->setPosition( pos );
  }
}

void InfoBoxManager::setShowDebugInfo( const bool showInfo )
{
  _d->showDebugInfo = showInfo;
} 

void InfoBoxManager::addCreator( const BuildingType type, const std::string& typeName, InfoboxCreator* ctor )
{
  bool alreadyHaveConstructor = _d->name2typeMap.find( typeName ) != _d->name2typeMap.end();
  _OC3_DEBUG_BREAK_IF( alreadyHaveConstructor && "already have constructor for this type");

  if( !alreadyHaveConstructor )
  {
    _d->name2typeMap[ typeName ] = type;
    _d->constructors[ type ] = ctor;
  }
}

bool InfoBoxManager::canCreate( const BuildingType type ) const
{
  return _d->constructors.find( type ) != _d->constructors.end();   
}
