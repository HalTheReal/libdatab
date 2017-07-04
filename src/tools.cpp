#include "tools.h"

namespace tls {

  std::vector <std::string> split(const std::string &toSplit, char c) {
    std::vector <std::string> result;
    const char *str = toSplit.c_str();
    do
    {
      const char *begin = str;
      while (*begin == c && *begin) {
        begin++;
      }
      str = begin;
      while (*str != c && *str) {
        str++;
      }
      result.push_back(std::string(begin, str));
    } while (0 != *str++);
    return result;
  }

  std::vector <std::string> splitWhite(const std::string &toSplit) {
    char sp = ' ';
    char tb = '\t';
    const char *str = toSplit.c_str();
    std::vector <std::string> res;
    do
    {
      const char *begin = str;
      while ((*begin == sp || *begin == tb) && *begin) {
        begin++;
      }
      str = begin;
      while (*str != sp && *str != tb && *str) {
        str++;
      }
      res.push_back(std::string(begin, str));
    } while (0 != *str++);
    return res;
  }

}
