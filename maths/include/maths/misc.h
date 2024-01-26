#ifndef MATHS_MISC_H
#define MATHS_MISC_H


namespace maths {

  /**
    * \brief  Returns the signed minimum between var or abs_limit
    */
  template <typename TYPE>
  inline TYPE signedMin(const TYPE& var, const TYPE& abs_limit) {
    if (std::signbit(var))
      return std::max(var, -abs_limit);
    else
      return std::min(var, abs_limit);
  }

  /**
    * \brief  Returns the signed maximum between var or abs_limit
    */
  template <typename TYPE>
  inline TYPE signedMax(const TYPE & var, const TYPE & abs_limit) {
    if (std::signbit(var))
      return std::min(var, -abs_limit);
    else
      return std::max(var, abs_limit);
  }

}

#endif //MATHS_MISC_H