/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef MD5_HH
# define MD5_HH

# include <string>

class MD5
{
  public:
    /// Just calls init().
    MD5();
    MD5(const std::string& s);

    /// Set default values for ctx.
    void init();

    /// Update the content to be hashed.
    void update(const std::string& s);

    /// Get the hash in a 32 byte string.
    std::string digest_get();


  private:
    /// Structure used to store information.
    struct Context {
      unsigned int buf[4];
      unsigned int bits[2];
      unsigned char in[64];
    } ctx_;

    /// Whether or not the to_string() and digest_get() functions can be called
    bool is_computed_;

    /// Here is the resulting hash.
    unsigned char digest_[16];

    /// Main function for MD5 calculation.
    void transform();

    /// Compute the digest (all previous stuff will be unusable).
    void final();

};

#endif // MD5_HH
