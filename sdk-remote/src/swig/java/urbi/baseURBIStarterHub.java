/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package urbi;
  /*--------------.
  | UStarterHub.  |
  `--------------*/
  /// URBIStarter base class used to store heterogeneous template
  /// class objects in starterlist

public class baseURBIStarterHub {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected baseURBIStarterHub(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(baseURBIStarterHub obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        urbiJNI.delete_baseURBIStarterHub(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public UObjectHub instanciate(UContextImpl ctx, String n) {
    long cPtr = urbiJNI.baseURBIStarterHub_instanciate__SWIG_0(swigCPtr, this, UContextImpl.getCPtr(ctx), ctx, n);
    return (cPtr == 0) ? null : new UObjectHub(cPtr, false);
  }

  public UObjectHub instanciate(UContextImpl ctx) {
    long cPtr = urbiJNI.baseURBIStarterHub_instanciate__SWIG_1(swigCPtr, this, UContextImpl.getCPtr(ctx), ctx);
    return (cPtr == 0) ? null : new UObjectHub(cPtr, false);
  }

  public void setName(String value) {
    urbiJNI.baseURBIStarterHub_name_set(swigCPtr, this, value);
  }

  public String getName() {
    return urbiJNI.baseURBIStarterHub_name_get(swigCPtr, this);
  }

}
