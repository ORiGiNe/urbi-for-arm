/*
 * Copyright (C) 2009-2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <urbi/uobject.hh>
#include "md5-impl.hh"

class md5
  : public urbi::UObject
{
  public:
    md5 (const std::string& s);

    int init () { return 1; }

    /// Return the md5sum of the string str.
    std::string sum (const std::string& str);
};

md5::md5 (const std::string& s)
  : urbi::UObject (s)
{
  UBindFunction(md5, init);
  UBindFunction(md5, sum);
}

std::string
md5::sum (const std::string& str)
{
  return MD5(str).digest_get();
}

UStart(md5);
