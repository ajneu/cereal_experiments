#include <iostream>
#include <cereal/archives/json.hpp>
#include <sstream>
#include <string>

struct Position {
  enum Pos {off,
            on};

  // constructor
  Position()                     : pos{off} {}
  Position(Pos p)                : pos{p} {}
  Position(const std::string &s) : pos{fromString(s)} {}
  Position(int i)                : pos{fromInt(i)} {}
 
  bool isOn() const {
    return pos == on;
  }

  // choose one of the following variants:
#define VARIANT_STRING
//#define VARIANT_INT
  
#ifdef VARIANT_STRING
  template <class Archive>
  std::string save_minimal( Archive const & ) const
  {
    return toString();
  }

  template <class Archive>
  void load_minimal( Archive const &, std::string const & value )
  {
    pos = fromString(value);
  }
#endif

#ifdef VARIANT_INT
  template <class Archive>
  int save_minimal( Archive const & ) const
  {
    return int(pos);
  }

  template <class Archive>
  void load_minimal( Archive const &, const int &i)
  {
    pos = fromInt(i ? on : off);
  }
#endif
  
  const std::string& toString() const {
    return (isOn() ? onStr : offStr);
  }

  Pos fromString(const std::string &s) const
  {
    return ((s == onStr) ? on : off);
  }

  int toInt(const int i) const
  {
    return (isOn() ? 1 : 0);
  }

  Pos fromInt(const int i) const
  {
    return (i ? on : off);
  }
  
private:
  Pos pos;

  static const std::string onStr;
  static const std::string offStr;
};

const std::string Position::onStr{"on"};
const std::string Position::offStr{"off"};






struct Switch
{
  int       number;
  Position  pos;
 
  Switch() {}
  Switch(int interv, const Position &sp)      : number{interv}, pos{sp} {}
  Switch(int interv, const std::string &spStr) : number{interv}, pos{spStr} {}
   
  std::string s;
 
  template <class Archive>
  void save(Archive &ar) const
  {
    ar(cereal::make_nvp(numberStr,  number),
       cereal::make_nvp(posStr, pos)
       );
  }

  template <class Archive>
  void load(Archive &ar)
  {
    std::string tmp;
    ar(number,
       //tmp
       pos
       );
    //pos = tmp;
  }

  static const std::string numberStr;
  static const std::string posStr;
};

const std::string Switch::numberStr{ "number"};
const std::string Switch::posStr{"position"};




int main()
{
  std::stringstream ss;
  {
    cereal::JSONOutputArchive archive_out(ss);

    Switch swOut{110, Position::on};
    archive_out( swOut );
  }
  std::cout << "Wrote the following JSON data:\n" << ss.str() << std::endl;

  {
    Switch swIn;

    // use this order of the following 2 lines, since then
    // archive_out's destuctor comes AFTER archive_in's destructor (which is what we want)
    cereal::JSONOutputArchive archive_out(std::cout);
    cereal::JSONInputArchive  archive_in(ss);
   
    std::cout << "\n\nRead back the following JSON data:\n";
    archive_in( swIn );
    archive_out( swIn );
  }
  std::cout << std::endl;

  return 0;
}
