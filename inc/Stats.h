#ifndef MYSTATISTICS_H
#define MYSTATISTICS_H

#include <math.h>

namespace Stats {

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type mean(InputIterator first, InputIterator last) {
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      int count = 0;
      for (; first != last; ++first) {
        tot += *first;
        ++count;
      }
      return tot / count;
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type mean(InputIterator first, InputIterator last, int nodata) {
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      int count = 0;
      for (; first != last; ++first) {
        if (*first != nodata) {
          tot += *first;
          ++count;
        }
      }
      return count == 0 ? nodata : tot / count;
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type stDev(InputIterator first, InputIterator last) {
      auto media = mean(first, last);
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      int count = 0;
      for (; first != last; ++first) {
        tot += pow(*first - media, 2);
        ++count;
      }
      return sqrt(tot / count);
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type stDev(InputIterator first, InputIterator last, int nodata) {
      auto media = mean(first, last, nodata);
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      int count = 0;
      for (; first != last; ++first) {
        if (*first != nodata) {
          tot += pow(*first - media, 2);
          ++count;
        }
      }
      return count == 0 ? nodata : sqrt(tot / count);
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type sum(InputIterator first, InputIterator last) {
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      for (; first != last; ++first) {
        tot += *first;
      }
      return tot;
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type sum(InputIterator first, InputIterator last, int nodata) {
      typename std::iterator_traits<InputIterator>::value_type tot = 0;
      int count = 0;
      for (; first != last; ++first) {
        if (*first != nodata) {
          tot += *first;
          ++count;
        }
      }
      return count == 0 ? nodata : tot;
    }

  template <typename InputIterator>
    typename std::iterator_traits<InputIterator>::value_type median(InputIterator first, InputIterator last) {
      std::vector<typename std::iterator_traits<InputIterator>::value_type> tosort(first, last);
      std::sort(tosort.begin(), tosort.end());
      if (tosort.size() % 2 == 0) {   // Numero pari di elementi
        int high = tosort.size() / 2;
        int low = high - 1;
        return (tosort[low] + tosort[high]) / 2.0;
      }
      return tosort[(tosort.size() / 2) + 1];
    }
}

#endif
