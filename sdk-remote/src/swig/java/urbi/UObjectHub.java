/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package urbi;
  //! Main UObjectHub class definition

public class UObjectHub extends UContext {
  private long swigCPtr;

  protected UObjectHub(long cPtr, boolean cMemoryOwn) {
    super(urbiJNI.SWIGUObjectHubUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(UObjectHub obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        urbiJNI.delete_UObjectHub(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public UObjectHub(String arg0, UContextImpl ctx) {
    this(urbiJNI.new_UObjectHub__SWIG_0(arg0, UContextImpl.getCPtr(ctx), ctx), true);
  }

  public UObjectHub(String arg0) {
    this(urbiJNI.new_UObjectHub__SWIG_1(arg0), true);
  }

  public void addMember(UObjectCPP obj) {
    urbiJNI.UObjectHub_addMember(swigCPtr, this, UObjectCPP.getCPtr(obj), obj);
  }

  public void delMember(UObjectCPP obj) {
    urbiJNI.UObjectHub_delMember(swigCPtr, this, UObjectCPP.getCPtr(obj), obj);
  }

      /// Set a timer that will call update() every 'period' milliseconds.
 public  void USetUpdate(double period) {
    urbiJNI.UObjectHub_USetUpdate(swigCPtr, this, period);
  }

  public int update() {
    return urbiJNI.UObjectHub_update(swigCPtr, this);
  }

      //   UObjectList* getAllSubClass(const std::string&); //TODO
 public  String get_name() {
    return urbiJNI.UObjectHub_get_name(swigCPtr, this);
  }

}
