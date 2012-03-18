/*
 * Copyright (C) 2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef CACHEABLE_HH
#define CACHEABLE_HH

template<class T> class Cacheable
{
  public:
    static T* fetch(const std::string& s)
    {
      cache_type& cache = cache_get();
      typename cache_type::iterator i = cache.find(s);
      if (i == cache.end())
      {
        T* res = new T(s);
        cache[s] = res;
        return res;
      }
      else
        return i->second;
    }
  private:
    typedef boost::unordered_map<std::string, T*> cache_type;
    static cache_type& cache_get()
    {
      static cache_type cache;
      return cache;
    }
};

#endif
