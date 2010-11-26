#include "config.h"
////////////////////////////////////////////////////////////////////////////////
namespace Core{
////////////////////////////////////////////////////////////////////////////////
    Config::Config() : Parser(){
        _base_icons_path = new std::string("/usr/share/omweather/icons");
        _iconset = new std::string("Glance");
        _temperature_unit = new std::string("C");
        _font_color = new std::string("#00ff00");
        parser->set_validate(true);
    }
////////////////////////////////////////////////////////////////////////////////
    Config::~Config(){
        delete _base_icons_path;
        delete _iconset;
        delete _temperature_unit;
    }
////////////////////////////////////////////////////////////////////////////////
    void
    Config::parse(const std::string& filename){
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        try{
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
            parser->parse_file(filename);
            if(parser){
                //Walk the tree:
                const xmlpp::Node* pNode = parser->get_document()->get_root_node(); //deleted by DomParser.
//                print_node(pNode);
            }
#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
        }
        catch(const std::exception& ex){
            throw(ex.what());
        }
#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    }
////////////////////////////////////////////////////////////////////////////////
     void
     Config::Base_Icons_Path(const std::string& text){
         /* ToDo Check access to path */
         _base_icons_path->assign(text);
     }
////////////////////////////////////////////////////////////////////////////////
     std::string&
     Config::Base_Icons_Path(){
         return *_base_icons_path;
     }
 ////////////////////////////////////////////////////////////////////////////////
     void
     Config::Iconset(const std::string& text){
         /* ToDo Check access to path */
         _iconset->assign(text);
     }
 ////////////////////////////////////////////////////////////////////////////////
     std::string&
     Config::Iconset(){
         return *_iconset;
     }
 ////////////////////////////////////////////////////////////////////////////////
         void
         Config::TemperatureUnit(const std::string& text){
             /* ToDo Check access to path */
             _temperature_unit->assign(text);
         }
 ////////////////////////////////////////////////////////////////////////////////
         std::string&
         Config::TemperatureUnit(){
             return *_temperature_unit;
         }
 ////////////////////////////////////////////////////////////////////////////////
         void
         Config::FontColor(const std::string& text){
             /* ToDo Check access to path */
             _font_color->assign(text);
         }
 ////////////////////////////////////////////////////////////////////////////////
         std::string&
         Config::FontColor(){
             return *_font_color;
         }
 ////////////////////////////////////////////////////////////////////////////////

} // namespace Core
