// -*- C++ -*-

#ifndef UTILITY_HH
#define UTILITY_HH

namespace utility
{
//_____________________________________________________________________________
std::vector<std::string>
split(std::string str, char del)
{
  std::size_t first = 0;
  std::size_t last = str.find_first_of( del );
  std::vector<std::string> result;
  while( first < str.size() ){
    std::string sub( str, first, last - first );
    result.push_back( sub );
    first = last + 1;
    last = str.find_first_of( del, first );
    if( last == std::string::npos ){
      last = str.size();
    }
  }
  return result;
}

} // namespace utility

#endif
