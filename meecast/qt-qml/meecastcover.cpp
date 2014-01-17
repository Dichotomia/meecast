/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather) - MeeCast
 *
 * Copyright (C) 2006-2014 Vlad Vasilyeu
 *     for the code
 *
 * Copyright (C) 2008 Andrew Zhilin
 *		      az@pocketpcrussia.com 
 *	for default icon set (Glance)
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU weather-config.h General Public
 * License along with this software; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
*/
/*******************************************************************************/

#include "meecastcover.h"
#include <iostream>

MeeCastCover::MeeCastCover(QObject* parent): QObject(parent){
    _status = false;
    try{
       _config =  ConfigQml::Instance(Core::AbstractConfig::getConfigPath()+
                               "config.xml",
                               Core::AbstractConfig::prefix+
                               Core::AbstractConfig::schemaPath+
                               "config.xsd");
    }
    catch(const std::string &str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config =  ConfigQml::Instance();
    }
    catch(const char *str){
        std::cerr<<"Error in Config class: "<< str <<std::endl;
        _config =  ConfigQml::Instance();
    }

}

MeeCastCover::~MeeCastCover(){
}

bool MeeCastCover::MeeCastCover::status(){
    return _status;
}
void MeeCastCover::MeeCastCover::setStatus(bool _status){
    _status = _status;
    emit statusChanged();
    std::cerr<<"MeeCastCover::MeeCastCover::setStatus"<<std::endl;
    if (_status)
      _config->check_and_update_station();
}
