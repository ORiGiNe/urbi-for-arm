/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package urbi;

public enum UProperty {
  PROP_RANGEMIN,
  PROP_RANGEMAX,
  PROP_SPEEDMIN,
  PROP_SPEEDMAX,
  PROP_BLEND,
  PROP_DELTA,
  PROP_CONSTANT;

  public final int swigValue() {
    return swigValue;
  }

  public static UProperty swigToEnum(int swigValue) {
    UProperty[] swigValues = UProperty.class.getEnumConstants();
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (UProperty swigEnum : swigValues)
      if (swigEnum.swigValue == swigValue)
        return swigEnum;
    throw new IllegalArgumentException("No enum " + UProperty.class + " with value " + swigValue);
  }

  @SuppressWarnings("unused")
  private UProperty() {
    this.swigValue = SwigNext.next++;
  }

  @SuppressWarnings("unused")
  private UProperty(int swigValue) {
    this.swigValue = swigValue;
    SwigNext.next = swigValue+1;
  }

  @SuppressWarnings("unused")
  private UProperty(UProperty swigEnum) {
    this.swigValue = swigEnum.swigValue;
    SwigNext.next = this.swigValue+1;
  }

  private final int swigValue;

  private static class SwigNext {
    private static int next = 0;
  }
}

