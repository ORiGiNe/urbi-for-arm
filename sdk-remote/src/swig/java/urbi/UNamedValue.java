/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package urbi;
  /*--------------.
  | UNamedValue.  |
  `--------------*/

public class UNamedValue {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected UNamedValue(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(UNamedValue obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        urbiJNI.delete_UNamedValue(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public UNamedValue(String n, UValue v) {
    this(urbiJNI.new_UNamedValue__SWIG_0(n, UValue.getCPtr(v), v), true);
  }

  public UNamedValue(String n) {
    this(urbiJNI.new_UNamedValue__SWIG_1(n), true);
  }

  public UNamedValue() {
    this(urbiJNI.new_UNamedValue__SWIG_2(), true);
  }

      // Used on errors.
 public  static UNamedValue error() {
    return new UNamedValue(urbiJNI.UNamedValue_error(), false);
  }

  public void setName(String value) {
    urbiJNI.UNamedValue_name_set(swigCPtr, this, value);
  }

  public String getName() {
    return urbiJNI.UNamedValue_name_get(swigCPtr, this);
  }

  public void setVal(UValue value) {
    urbiJNI.UNamedValue_val_set(swigCPtr, this, UValue.getCPtr(value), value);
  }

  public UValue getVal() {
    long cPtr = urbiJNI.UNamedValue_val_get(swigCPtr, this);
    return (cPtr == 0) ? null : new UValue(cPtr, false);
  }

}