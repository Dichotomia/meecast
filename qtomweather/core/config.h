#ifndef CONFIG_H
#define CONFIG_H
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include "parser.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
    class Config : public Parser{
            void parse(const std::string& filename);
            std::string *_base_icons_path;
            std::string *_iconset;
            std::string *_temperature_unit;
            std::string *_font_color;
        public:
            Config();
            Config(const Config& config);
            Config& operator=(const Config& config);
            virtual ~Config();
            void Base_Icons_Path(const std::string& text);
            std::string& Base_Icons_Path(void);
            void Iconset(const std::string& text);
            std::string& Iconset(void);
            void TemperatureUnit(const std::string& text);
            std::string& TemperatureUnit(void);
            void FontColor(const std::string& text);
            std::string& FontColor(void);
    };
} // namespace Core
////////////////////////////////////////////////////////////////////////////////
#endif // CONFIG_H
