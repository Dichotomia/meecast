/* vim: set sw=4 ts=4 et: */
/*
 * This file is part of Other Maemo Weather(omweather)
 *
 * Copyright (C) 2006-2011 Vlad Vasiliev
 * Copyright (C) 2006-2011 Pavel Fialko
 * Copyright (C) 2010-2011 Tanya Makova
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


#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <QTextStream>
#include "parser.h"
#include "stationlist.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            #ifdef LIBXML
            void processNode(const xmlpp::Node* node);
            #endif
            std::string *_filename;
            std::string *_pathPrefix;
            std::string *_iconset;
            std::string *_temperature_unit;
            std::string *_wind_speed_unit;
            std::string *_pressure_unit;
            bool         _update_connect;
            bool         _fullscreen;
            bool         _lockscreen;
            bool         _standbyscreen;
            bool         _gps;
            bool         _splash;
            int          _update_period;
            std::string *_font_color;
            int          _current_station_id;
            StationsList *_stations;
        protected:
            static Config* _self;
            static int _refcount;
            Config(const std::string& filename, const std::string& schema_filename = prefix + schemaPath + "config.xsd");
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
           
        public:
            static Config* Instance();
            static Config* Instance(const std::string& filename, const std::string& schema_filename);
            void LoadConfig();
            void ReLoadConfig();
            std::string& prefix_path(void);
            void iconSet(const std::string& text);
            std::string& iconSet(void);
            void current_station_id(int id_station);
            int current_station_id(void);
            void TemperatureUnit(const std::string& text);
            std::string& TemperatureUnit(void);
            void WindSpeedUnit(const std::string& text);
            std::string& WindSpeedUnit(void);
            void PressureUnit(const std::string& text);
            std::string& PressureUnit(void);
            void UpdateConnect(const bool uc);
            bool UpdateConnect(void);
            void Fullscreen(const bool uc);
            bool Fullscreen(void);
            void Lockscreen(const bool uc);
            bool Lockscreen(void);
            void Standbyscreen(const bool uc);
            bool Standbyscreen(void);
            void Gps(const bool uc);
            bool Gps(void);
            void Splash(const bool uc);
            bool Splash(void);
            void UpdatePeriod(const int period);
            int UpdatePeriod(void);
            void FontColor(const std::string& text);
            std::string& FontColor(void);
            StationsList& stationsList();
            std::string& stationname();
            std::string& nextstationname();
            std::string& prevstationname();
            std::string& iconspath();
            //void stationsList(std::vector<Core::Station*> list);
            void stationsList(StationsList list);
            void saveConfig();


    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
