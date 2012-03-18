/*
 * Copyright (C) 2006-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <cstdarg>

#include <libport/bind.hh>
#include <libport/lexical-cast.hh>
#include <libport/foreach.hh>
#include <libport/hash.hh>
#include <libport/lexical-cast.hh>
#include <libport/synchronizer.hh>

#include <kernel/config.h>

#include <urbi/kernel/uconnection.hh>
#include <urbi/kernel/userver.hh>
#include <kernel/uvalue-cast.hh>
#include <kernel/uobject.hh>

#include <urbi/object/cxx-primitive.hh>
#include <urbi/object/dictionary.hh>
#include <object/finalizable.hh>
#include <urbi/uevent.hh>
#include <urbi/object/event.hh>
#include <urbi/object/float.hh>
#include <urbi/object/global.hh>
#include <urbi/object/lobby.hh>
#include <urbi/object/object.hh>
#include <urbi/object/object.hh>
#include <urbi/object/string.hh>
#include <urbi/object/tag.hh>
#include <urbi/object/symbols.hh>
#include <object/system.hh>
#include <object/uconnection.hh>
#include <urbi/object/urbi-exception.hh>
#include <object/uvalue.hh>
#include <object/uvar.hh>
#include <urbi/runner/raise.hh>
#include <runner/runner.hh>

#include <urbi/uobject.hh>
#include <urbi/uexternal.hh>
#include <urbi/uvalue-serialize.hh>

#include <urbi/ucontext-factory.hh>

GD_CATEGORY(Urbi.UObject);

// Make it more readable.
using object::List;
using object::rPath;
//Next using disabled, fails on gcc3, conflict with boost::filesystem::Path.
//using object::Path;
using object::rEvent;
using object::rObject;
using object::rLobby;
using object::objects_type;
using object::void_class;
using object::nil_class;
using libport::Symbol;
using kernel::server;

// Declare our UObject implementation
namespace urbi
{
  namespace impl
  {
    class KernelUContextImpl: public UContextImpl
    {
    public:
      /// FIXME: only one kernel for now
      KernelUContextImpl();
      virtual UObjectHub* getUObjectHub(const std::string& n);
      virtual UObject* getUObject(const std::string& n);
      virtual void newUObjectClass(baseURBIStarter* s);
      virtual void newUObjectHubClass(baseURBIStarterHub* s);
      virtual void uobject_unarmorAndSend(const char* str);
      virtual void send(const char* str);
      virtual void send(const void* buf, size_t size);
      virtual void call(const std::string& object,
                         const std::string& method,
                         UAutoValue v1 = UAutoValue(),
                         UAutoValue v2 = UAutoValue(),
                         UAutoValue v3 = UAutoValue(),
                         UAutoValue v4 = UAutoValue(),
                         UAutoValue v5 = UAutoValue(),
                         UAutoValue v6 = UAutoValue()
                         );
      virtual void declare_event(const UEvent* owner);
      virtual void emit(const std::string& object,
                        UAutoValue& v1,
                        UAutoValue& v2,
                        UAutoValue& v3,
                        UAutoValue& v4,
                        UAutoValue& v5,
                        UAutoValue& v6,
                        UAutoValue& v7
                        );
      virtual UObjectMode getRunningMode() const;
      virtual std::pair<int, int> kernelVersion() const;
      virtual void yield() const;
      virtual void yield_until(libport::utime_t deadline) const;
      virtual void yield_for(libport::utime_t delay) const;
      virtual void yield_until_things_changed() const;
      virtual void side_effect_free_set(bool s);
      virtual bool side_effect_free_get() const;
      virtual UVarImpl* getVarImpl();
      virtual UObjectImpl* getObjectImpl();
      virtual UGenericCallbackImpl* getGenericCallbackImpl();
      virtual TimerHandle setTimer(UTimerCallback* cb);
      virtual void registerHub(UObjectHub*);
      virtual void removeHub(UObjectHub*) ;
      virtual void setHubUpdate(UObjectHub*, ufloat);
      virtual void instanciated(UObject* uob);
      virtual void lock();
      virtual void unlock();
      virtual boost::asio::io_service& getIoService();
      static inline KernelUContextImpl* instance() {return instance_;}

    private:
      static KernelUContextImpl* instance_;
    };

    KernelUContextImpl* KernelUContextImpl::instance_ = 0;
    class KernelUGenericCallbackImpl;
    class KernelUObjectImpl: public UObjectImpl
    {
    public:
      virtual void initialize(UObject* owner);
      virtual void clean();
      virtual void setUpdate(ufloat period);
      virtual bool removeTimer(TimerHandle h);
      virtual ~KernelUObjectImpl();
    private:
      UObject* owner_;
      friend class KernelUGenericCallbackImpl;
      std::vector<UGenericCallback*> callbacks_;
    };

    class KernelUVarImpl: public UVarImpl
    {
    public:
      KernelUVarImpl();
      virtual void initialize(UVar* owner);
      virtual void clean();
      virtual void setOwned();
      virtual void sync();
      virtual void request();
      virtual void keepSynchronized();
      virtual void set(const UValue& v);
      virtual const UValue& get() const;
      virtual ufloat& in();
      virtual ufloat& out();
      virtual UDataType type() const;
      virtual UValue getProp(UProperty prop);
      virtual void setProp(UProperty prop, const UValue& v);
      virtual bool setBypass(bool enable);
      virtual time_t timestamp() const;
      virtual void unnotify();
      virtual void useRTP(bool enable);
      virtual void setInputPort(bool enable);
    private:
      libport::Lockable asyncLock_; // lock for pending_
      /* Schedule an operation to be executed by the main thread, preventing
       * the destruction of this object and the owner UVar until the operation
       * is completed.
       */
      void schedule(libport::Symbol s, boost::function0<void> op) const;
      void async(boost::function0<void> op);
      void async_get(UValue**, libport::Semaphore&) const;
      void async_get_prop(UValue&, libport::Semaphore&, UProperty);
      unsigned pending_; // number of pending asynchronous operations
      UVar* owner_;
      bool bypassMode_;
      mutable UValue cache_;
      typedef std::vector<KernelUGenericCallbackImpl*> callbacks_type;
      callbacks_type callbacks_;
      object::rUVar ruvar_;
      friend class KernelUGenericCallbackImpl;
    };

    class KernelUGenericCallbackImpl: public UGenericCallbackImpl
    {
    public:
      virtual void initialize(UGenericCallback* owner, bool owned);
      virtual void initialize(UGenericCallback* owner);
      virtual void registerCallback();
      virtual void clear();
      /// Force using the closed Var instead of the one given as argument.
      bool useClosedVar_;
    private:
      UGenericCallback* owner_;
      bool owned_;
      bool registered_;
      // Notify id
      int id_;
      /// The Primitive we put in 'change'.
      rObject callback_;
      /// The connection we implicitly created or 0.
      object::rUConnection connection_;
      /// Input port the notify was redirected to, or empty
      std::string inportName_;
      friend class KernelUObjectImpl;
      friend class KernelUVarImpl;
    };
  }
}

// Where to store uobjects
static rObject where;
typedef boost::unordered_map<std::string, urbi::UObject*> uobject_to_robject_type;
static uobject_to_robject_type uobject_to_robject;
static std::set<void*> initialized;
static bool trace_uvars = 0;
static libport::file_library uobjects_path;
using urbi::uobjects::get_base;
using urbi::uobjects::split_name;
using urbi::uobjects::StringPair;

// No rObject here as we do not want to prevent object destruction.
static boost::unordered_map<std::string, object::Object*> uobject_map;

static void setTrace(rObject, bool v)
{
  trace_uvars = v;
}

namespace
{
  inline
  rObject
  xget_base(const std::string& objname,
            const std::string& error = "no such UObject: %s")
  {
    if (rObject res = get_base(objname))
      return res;
    FRAISE(error, objname);
  }
}


// Object,method names of last call
static std::vector<std::pair<std::string, std::string> > bound_context;

#define MAKE_VOIDCALL(ptr, cls, meth)                   \
  object::primitive(                                    \
    boost::function1<void, rObject>(                    \
      boost::bind(&cls::meth, ptr)))                    \

#ifdef SCHED_CORO_OSTHREAD
# define CHECK_MAINTHREAD()
#else
# define CHECK_MAINTHREAD()				\
  if (::kernel::urbiserver->isAnotherThread())          \
    throw std::runtime_error("Current thread is not main thread")
#endif

namespace Stats
{
  struct Value
  {
    unsigned sum;
    unsigned min;
    unsigned max;
    unsigned count;
  };
  typedef boost::unordered_map<std::string, Value> Values;
  static Values hash;
  static bool enabled = false;
  static void add(void*, const std::string& key, libport::utime_t d)
  {
    if (!enabled)
      return;
    Values::iterator i = hash.find(key);
    if (i == hash.end())
    {
      Value& v = hash[key];
      v.sum = v.min = v.max = d;
      v.count = 1;
    }
    else
    {
      i->second.sum += d;
      i->second.count++;
      i->second.min = std::min(i->second.min, (unsigned)d);
      i->second.max = std::max(i->second.max, (unsigned)d);
    }
  }

  static void clear(rObject)
  {
    hash.clear();
  }

  static object::rDictionary get(rObject)
  {
    using object::Float;
    object::rDictionary res = new object::Dictionary();
    foreach(Values::value_type &v, hash)
    {
      object::rList l = new object::List();
      l->insertBack(new Float(v.second.sum / v.second.count));
      l->insertBack(new Float(v.second.min));
      l->insertBack(new Float(v.second.max));
      l->insertBack(new Float(v.second.count));
      res->set(new object::String(v.first), l);
      /*
      size_t sep = v.first.find_first_of(' ');
      std::string sPtr = v.first.substr(0, sep);
      std::string name = v.first.substr(sep+1, v.first.npos);
      */
    }
    return res;
  }
  static void enable(rObject, bool state)
  {
    enabled = state;
  }
};

static inline void traceOperation(urbi::UVar*v, libport::Symbol op)
{
  if (trace_uvars)
  {
    StringPair p = split_name(v->get_name());
    rObject o = xget_base(p.first);
    object::global_class
      ->slot_get(SYMBOL(UVar))
      ->slot_get(op)
      ->call(SYMBOL(syncEmit), o, o->slot_get(Symbol(p.second)),
             object::to_urbi(bound_context));
  }
}

static void periodic_call(rObject, ufloat interval, rObject method,
                          libport::Symbol msg, object::objects_type args)
{
  runner::Runner& r = ::kernel::runner();
  libport::utime_t delay = libport::seconds_to_utime(interval);
  while (true)
  {
    urbi::setCurrentContext(urbi::impl::KernelUContextImpl::instance());
    r.apply(method, msg, args);
    libport::utime_t target = libport::utime() + delay;
    r.yield_until(target);
  }
}

void uobjects_reload()
{
  urbi::impl::KernelUContextImpl::instance()->init();
  foreach (urbi::baseURBIStarterHub* i, urbi::baseURBIStarterHub::list())
    if (!libport::mhas(initialized, i))
    {
      initialized.insert(i);
    }
  foreach (urbi::baseURBIStarter* i, urbi::baseURBIStarter::list())
    if (!libport::mhas(initialized, i))
    {
      initialized.insert(i);
    }
}

//! Read/Write UObjects PATH
const libport::file_library
uobject_uobjects_path()
{
  return uobjects_path;
}

List::value_type
uobject_uobjectsPath(const rObject&)
{
  List::value_type list;

  foreach (const libport::path& p,
           uobjects_path.search_path_get())
    list << new object::Path(p);
  return list;
}

void
uobject_uobjectsPathSet(const rObject&, List::value_type list)
{
  uobjects_path.search_path().clear();
  foreach (rObject p, list)
  {
    rPath path = p->as<object::Path>();
    uobjects_path.search_path().push_back(path->value_get());
  }
}

static void delete_uvar(urbi::UVar* v)
{
  delete v;
}

static rObject
wrap_ucallback_notify(const object::objects_type& ol,
                      urbi::UGenericCallback* ugc,
                      const std::string& traceName)
{
  GD_FPUSH_TRACE("Calling bound notify %s", traceName);
  urbi::setCurrentContext(urbi::impl::KernelUContextImpl::instance());
  bound_context
    << std::make_pair(&ugc->owner ? ugc->owner.__name : "unknown",
                      ugc->name);
  urbi::impl::KernelUGenericCallbackImpl& impl =
    static_cast<urbi::impl::KernelUGenericCallbackImpl&>  (*ugc->impl_);
  bool dummy = false;
  FINALLY(((bool, dummy)), bound_context.pop_back(); GD_INFO_TRACE("Done"));
  urbi::UList l;
  l.array << new urbi::UValue();
  // Decide whether we use the closed UVar, or the one given in arguments.
  bool useArgVar = ol.size() > 1 && !impl.useClosedVar_;
  if (useArgVar)
    l[0].storage = new urbi::UVar
  (object::from_urbi<std::string>(ol[1]->call(SYMBOL(fullName))));
  else
    l[0].storage = ugc->target;
  if (!ugc->isSynchronous())
  {
    /* This is an asynchronous notifychange. For synchronous ones we pass the
     * UVar to the callback function, but it is not appropriate for asynchronous
     * calls: we want each call to receive the value at the time of the call.
     * So we pass the value. It just means our user cannot take a UVar& as
     * callback argument, and must take the value directly.
     */
     urbi::UVar* uv = (urbi::UVar*)l[0].storage;
     l[0] = uv->val();
     if (useArgVar)
     {
       delete uv;
       useArgVar = false;
     }
     GD_FINFO_DUMP("async args: %s", l);
  }
  libport::utime_t t = libport::utime();
  if (useArgVar)
    ugc->eval(l, boost::bind(delete_uvar, (urbi::UVar*)l[0].storage));
  else
    ugc->eval(l);
  Stats::add(ol.front().get(), traceName, libport::utime() - t);
  return object::void_class;
}

static void write_and_unfreeze_mainthread(object::rTag* tag,
                                          urbi::UValue& resSlot,
                                          std::string& exceptionSlot,
                                          bool* async_abort,
                                          urbi::UValue* result,
                                          std::string exceptionResult)
{
  // Magic, we are in the main thread, so no possible race with wrap_ucallback!
  if (*async_abort)
  { // The call was aborted on the other end. Do absolutely nothing.
    GD_INFO_LOG("Aborted threaded call, ignoring return value");
    delete async_abort;
  }
  else
  {
    exceptionSlot = exceptionResult;
    resSlot = *result;
    delete result;
    (*tag)->unfreeze();
    delete tag;
  }
}

static void write_and_unfreeze(urbi::UValue& r, std::string& exception,
                               bool* async_abort,
                               object::rTag* tag,
                               urbi::UValue& v, const std::exception* e)
{
  std::string exceptionMessage;

  if (e)
    exceptionMessage = e->what();
  if (server().isAnotherThread())
    server().schedule_fast(
                      boost::bind(&write_and_unfreeze_mainthread, tag,
                                  boost::ref(r), boost::ref(exception),
                                  async_abort, new urbi::UValue(v),
                                  exceptionMessage));
  else
  {
    exception = exceptionMessage;
    r = v;
    (*tag)->unfreeze();
    delete tag;
  }

}

static rObject wrap_ucallback(const object::objects_type& ol,
                              urbi::UGenericCallback* ugc,
                              const std::string& message, bool withThis)
{
  GD_FPUSH_TRACE("Calling bound function %s", message);
  urbi::UList l;
  l.array.reserve(ol.size() - (withThis?1:0));
  urbi::setCurrentContext(urbi::impl::KernelUContextImpl::instance());
  object::check_arg_count(ol.size() - (withThis?1:0), ugc->nbparam);
  bool first = true;
  foreach (const rObject& co, ol)
  {
    if (withThis && first)
    {
      first = false;
      continue;
    }
    urbi::UValue v = uvalue_cast(co);
    l.array << new urbi::UValue(v);
  }
  libport::utime_t start = libport::utime();
  urbi::UValue res;

  bound_context
    << std::make_pair(&ugc->owner ? ugc->owner.__name : "unknown",
                      ugc->name);
  FINALLY(((bool, first)), bound_context.pop_back();GD_INFO_DUMP("Done"));
  // write_and_unfreeze will delete us if true, we handle it if false
  bool* async_abort = new bool;
  *async_abort = false;
  try
  {
    // This if is there to optimize the synchronous case.
    if (ugc->isSynchronous())
      res = ugc->__evalcall(l);
    else
    {
      /* We are going to make a threaded call that will return a result.
       * But while it runs we ourselve can be interrupted by a tag.stop.
       * Furthermore our shared ptr are not thread safe. So:
       * - We give to the thread references to res and exception on our stack.
       * - If we are stopped, those will be invalid because the stack will
       * unwind, so we also give it a bool* async_abort not on stack that we set
       * to true if we are interrupted (if it is false we delete it otherwise
       * the thread does).
       * - To ensure no race condition in the handling of the three above
       * pointers, the thread is going to server().schedule_fast the operation
       * handling passing back the return value, so that it is performed
       * also on this thread.
       */
      libport::Finally f;
      /* write_and_unfreeze will delete tag. It is only passed along by the
       * thread, not copied, not dereferenced, so its safe.
       */
      object::rTag* tag = new object::rTag(new object::Tag);
      ::kernel::runner().apply_tag(*tag, &f);
      // Tricky: tag->freeze() yields, but we must freeze tag before calling
      // eval or there will be a race if asyncEval goes to fast and unfreeze
      // before we freeze. So go throug backend.
      (*tag)->value_get()->freeze();
      std::string exception;
      ugc->eval(l, boost::bind(write_and_unfreeze, boost::ref(res),
                               boost::ref(exception),
                               async_abort, tag, _1, _2));
      ::kernel::runner().yield();
      if (!exception.empty())
        throw std::runtime_error("Exception in threaded call: " + exception);
    }
  }
  catch (const sched::exception&)
  {
    *async_abort = true;
    throw;
  }
  catch (const std::exception& e)
  {
    FRAISE("Exception caught while calling %s: %s",
           ugc->getName(), e.what());
  }
  catch (...)
  {
    FRAISE("Unknown exception caught while calling %s",
           ugc->getName());
  }
  delete async_abort;
  start = libport::utime() - start;
  Stats::add(ol.front().get(), message, start);
  return object_cast(res);
}


static
rObject
wrap_event(const object::objects_type& ol,
           urbi::UGenericCallback* ugc,
           const std::string& traceName)
{
  // We were called with arg1 = event, arg2 = payload, arg3 = pattern.
  object::objects_type args = ol[2]->as<object::List>()->value_get();
  if (args.size() == (unsigned int)ugc->nbparam)
    return wrap_ucallback(args, ugc, traceName, false);
  else
    GD_FINFO_DEBUG("C++ at %s not called: wrong arity", traceName);
  return object::void_class;
}


static rObject
uobject_clone(const object::objects_type& l)
{
  rObject proto = l.front();
  return urbi::uobjects::uobject_new(proto);
}

static rObject
uobject_finalize(const object::objects_type& args)
{
  rObject o = args.front();
  std::string objName = o->slot_get(SYMBOL(__uobjectName)).get<std::string>();
  // FIXME: uobject_to_robject[objName] should be enough
  urbi::UObject* uob = urbi::impl::KernelUContextImpl::instance()
    ->getUObject(objName);
  if (!uob)
    FRAISE("uobject_finalize: No uobject by the name of %s found", objName);
  delete uob;
  urbi::impl::KernelUContextImpl::instance()->objects.erase(objName);
  return object::void_class;
}

// Write to an UVar, pretending we are comming from lobby ctx.
static void writeFromContext(const std::string& ctx,
                             const std::string& varName,
                             const urbi::UValue& val)
{
  CHECK_MAINTHREAD();
  if (ctx.substr(0, 2) != "0x")
    throw std::runtime_error("invalid context: " + ctx);
  rLobby rl;
  foreach (object::Lobby* lobby, object::Lobby::instances_get())
    if (lobby->uid() == ctx)
    {
      rl = lobby;
      break;
    }
  if (!rl)
  {
    GD_FWARN("writeFromContext: non existing lobby: %x", ctx);
    return;
  }
  runner::Runner& r = kernel::urbiserver->getCurrentRunner();
  rLobby cl = r.lobby_get();
  FINALLY(((rLobby, cl))((runner::Runner&, r)), r.lobby_set(cl));
  r.lobby_set(rl);
  object::rUValue ov(new object::UValue());
  ov->put(val, false);
  StringPair p = split_name(varName);
  rObject o = get_base(p.first);
  if (!o)
    runner::raise_lookup_error(libport::Symbol(varName), object::global_class);
  o->slot_get(Symbol(p.second))->call(SYMBOL(update_timed), ov,
                                      object::to_urbi(libport::utime()));
  ov->invalidate();
}

namespace urbi
{

  /*----------.
  | UObject.  |
  `----------*/

  static void bounce_update(urbi::UObject* ob, void* me, const std::string& key)
  {
    urbi::setCurrentContext(urbi::impl::KernelUContextImpl::instance());
    libport::utime_t t = libport::utime();
    ob->update();
    Stats::add(me, key, libport::utime()-t);
  }

  typedef std::pair<std::string, std::string> StringPair;
  /// Split a string of the form "a.b" in two
  static StringPair
  split_name(const std::string& name)
  {
    size_t p = name.find_last_of(".");
    std::string oname = name.substr(0, p);
    std::string slot = name.substr(p + 1, name.npos);
    return StringPair(oname, slot);
  }
  UObjectMode running_mode()
  {
    return MODE_PLUGIN;
  }

  namespace impl
  {

    UContextImpl* getPluginContext()
    {
      return KernelUContextImpl::instance();
    }

    KernelUContextImpl::KernelUContextImpl()
    {
      rtpSend = 0;
      rtpSendGrouped = 0;
      instance_ = this;
    }

    void
    KernelUContextImpl::newUObjectClass(baseURBIStarter* s)
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(
                          boost::bind(&KernelUContextImpl::newUObjectClass,
                                      this, s));
        return;
      }
      GD_FINFO_TRACE("MakeProto for %s", s->name);
      object::rObject proto = ::urbi::uobjects::uobject_make_proto(s->name);
      GD_FINFO_TRACE("Writing class for %s", s->name);
      where->slot_set(libport::Symbol(s->name + "_class"), proto);
      // Make our first instance.
      GD_FINFO_TRACE("Creating instance for %s", s->name);
      rObject o = ::urbi::uobjects::uobject_new(proto, true);
    }

    void
    KernelUContextImpl::newUObjectHubClass(baseURBIStarterHub* s)
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(
          boost::bind(&KernelUContextImpl::newUObjectHubClass, this, s));
        return;
      }
      s->instanciate(this);
    }

    void
    KernelUContextImpl::send(const char* str)
    {
      if (server().isAnotherThread())
      { // Copy the data
        server().schedule_fast(boost::bind(
          (void (UContextImpl::*)(const std::string&))&UContextImpl::send,
           this, std::string(str)));
        return;
      }
      kernel::urbiserver->ghost_connection_get().received(str);
    }

    void
    KernelUContextImpl::send(const void* buf, size_t size)
    {
      if (server().isAnotherThread())
      { // Copy the data
        server().schedule_fast(boost::bind(
          (void (UContextImpl::*)(const std::string&))&UContextImpl::send,
           this, std::string((const char*)buf, size)));
        return;
      }
      // Feed this to the ghostconnection.
      kernel::urbiserver->ghost_connection_get()
        .received(static_cast<const char*>(buf), size);
    }

#define INTERRUPTIBLE                                                      \
  runner::Runner& r = ::kernel::runner();                                  \
  bool b = r.non_interruptible_get();                                      \
  r.non_interruptible_set(false);                                          \
  FINALLY(((runner::Runner&, r))((bool, b)), r.non_interruptible_set(b))


    void KernelUContextImpl::yield() const
    {
      CHECK_MAINTHREAD();
      INTERRUPTIBLE;
      ::kernel::runner().yield();
    }

    void KernelUContextImpl::yield_until(libport::utime_t deadline) const
    {
      CHECK_MAINTHREAD();
      INTERRUPTIBLE;
      ::kernel::runner().yield_until(deadline);
    }

    void KernelUContextImpl::yield_for(libport::utime_t delay) const
    {
      CHECK_MAINTHREAD();
      INTERRUPTIBLE;
      ::kernel::runner().yield_for(delay);
    }

    void KernelUContextImpl::yield_until_things_changed() const
    {
      CHECK_MAINTHREAD();
      INTERRUPTIBLE;
      ::kernel::runner().yield_until_things_changed();
    }

#undef INTERRUPTIBLE

    void KernelUContextImpl::side_effect_free_set(bool s)
    {
      CHECK_MAINTHREAD();
      ::kernel::runner().side_effect_free_set(s);
    }

    bool KernelUContextImpl::side_effect_free_get() const
    {
      CHECK_MAINTHREAD();
      return ::kernel::runner().side_effect_free_get();
    }

    UObjectHub*
    KernelUContextImpl::getUObjectHub(const std::string& n)
    {
      return UContextImpl::getUObjectHub(n);
    }

    UObject*
    KernelUContextImpl::getUObject(const std::string& n)
    {
      /// That check can be done in any thread.
      UObject* res = UContextImpl::getUObject(n);
      if (res)
        return res;
      /// But those below cannot.
      if (server().isAnotherThread())
        return 0;
      uobject_to_robject_type::iterator it = uobject_to_robject.find(n);
      if (it != uobject_to_robject.end())
        return it->second;
      rObject r = get_base(n);
      if (!r)
        return 0;
      std::string name =
        object::from_urbi<std::string>(r->slot_get(SYMBOL(__uobjectName)));
      if (name == n)
        return 0;
      return getUObject(name);
    }

    void
    KernelUContextImpl::call(const std::string& object,
                             const std::string& method,
                             UAutoValue v1,
                             UAutoValue v2,
                             UAutoValue v3,
                             UAutoValue v4,
                             UAutoValue v5,
                             UAutoValue v6)
    {
      // Bypass writeFromContext to avoid an extra UValue copy.
      if (method == "$uobject_writeFromContext")
      {
        std::string ctx = v1;
        std::string varname = v2;
        urbi::UValue val = v3;
        if (server().isAnotherThread())
          server().schedule_fast(boost::bind(&writeFromContext,
                                                         ctx, varname, val));
        else
          writeFromContext(v1, v2, v3);
        return;
      }
      if (server().isAnotherThread())
      {
        server().schedule_fast(boost::bind(&KernelUContextImpl::call,
                                                      this, object, method,
                                                      v1, v2, v3, v4, v5, v6));
        return;
      }
      rObject b = xget_base(object);
      object::objects_type args;
      args << b;

#define ARG(Nth)                               \
      do {                                     \
        if (v##Nth.type != DATA_VOID)          \
          args << object_cast(v##Nth);         \
      } while (false)

      ARG(1);
      ARG(2);
      ARG(3);
      ARG(4);
      ARG(5);
      ARG(6);
      b->call_with_this(libport::Symbol(method), args);
    }

    static void declare_event_name(std::string name)
    {
      rEvent e = new object::Event(object::Event::proto);
      StringPair p = split_name(name);
      rObject o = xget_base(p.first,
                            "UEvent creation on non existing object: %s");
      if (!o->local_slot_get(Symbol(p.second)))
        o->slot_set(Symbol(p.second), e);
    }
    void
    KernelUContextImpl::declare_event(const UEvent* owner)
    {
      if (server().isAnotherThread())

        server().schedule_fast(boost::bind(&declare_event_name,
                                       owner->get_name()));
      else
        declare_event_name(owner->get_name());
    }

    static void doEmit(const std::string& object, const object::objects_type& args)
    {
      StringPair p = split_name(object);
      rObject o = xget_base(p.first)->slot_get(libport::Symbol(p.second));
      o->call(SYMBOL(emit), args);
    }

    void
    KernelUContextImpl::emit(const std::string& object,
                             UAutoValue& v1,
                             UAutoValue& v2,
                             UAutoValue& v3,
                             UAutoValue& v4,
                             UAutoValue& v5,
                             UAutoValue& v6,
                             UAutoValue& v7
                             )
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(boost::bind(&KernelUContextImpl::emit,
          this, object, v1, v2, v3, v4, v5, v6, v7));
        return;
      }
      object::objects_type args;
      ARG(1);
      ARG(2);
      ARG(3);
      ARG(4);
      ARG(5);
      ARG(6);
      ARG(7);
      doEmit(object, args);
    }

    static void unarmor_and_send(std::string s)
    {
      server().ghost_connection_get().received(s.c_str(), s.length());
    }

    void
    KernelUContextImpl::uobject_unarmorAndSend(const char* str)
    {
      size_t len = strlen(str);
      if (server().isAnotherThread())
      {
        std::string arg;
        if (2 <= len && str[0] == '(')
          arg = std::string(str+1, len-1);
        else
          arg = str;
        server().schedule_fast(boost::bind(&unarmor_and_send, arg));
        return;
      }
      // Feed this to the ghostconnection.
      kernel::UConnection& ghost = kernel::urbiserver->ghost_connection_get();
      if (2 <= len && str[0] == '(')
        ghost.received(str + 1, len - 2);
      else
        ghost.received(str);
    }

    UObjectMode
    KernelUContextImpl::getRunningMode() const
    {
      return MODE_PLUGIN;
    }

    UVarImpl*
    KernelUContextImpl::getVarImpl()
    {
      return new KernelUVarImpl();
    }

    UObjectImpl*
    KernelUContextImpl::getObjectImpl()
    {
      return new KernelUObjectImpl();
    }

    UGenericCallbackImpl*
    KernelUContextImpl::getGenericCallbackImpl()
    {
      return new KernelUGenericCallbackImpl();
    }

    static void set_timer(UTimerCallback* cb, TimerHandle* handle,
                          libport::Semaphore& sem)
    {
      *handle = KernelUContextImpl::instance()->setTimer(cb);
      sem++;
    }

    TimerHandle
    KernelUContextImpl::setTimer(UTimerCallback* cb)
    {
      if (server().isAnotherThread())
      {
        TimerHandle th;
        libport::Semaphore sem;
        GD_INFO_DUMP("SetTimer: async request.");
        server().schedule_fast(boost::bind(&set_timer, cb, &th,
                                           boost::ref(sem)));
        sem--;
        GD_INFO_DUMP("SetTimer: returning.");
        return th;
      }
      rObject me = xget_base(cb->objname);
      rObject f = me->slot_get(SYMBOL(setTimer));
      rObject p = new object::Float(cb->period / 1000.0);
      std::string stag = object::String::proto->as<object::String>()->fresh();
      rObject tag = new object::String(stag);
      rObject call = MAKE_VOIDCALL(cb, urbi::UTimerCallback, call);
      object::objects_type args;
      args << me << p << call << tag;
      ::kernel::runner().apply(f, SYMBOL(setTimer), args);
      GD_FINFO_DUMP("SetTimer on %s: %s", cb, stag);
      return TimerHandle(new std::string(stag));
    }

    boost::asio::io_service&
    KernelUContextImpl::getIoService()
    {
      return ::kernel::urbiserver->get_io_service();
    }

    bool
    KernelUObjectImpl::removeTimer(TimerHandle h)
    {
      GD_FINFO_DUMP("RemoveTimer on %s %s", h, owner_->__name);
      if (!h)
        return false;
      if (server().isAnotherThread())
      { // Return value is not that important, so lie for the sake of making
        // an asynchronous call.
        server().schedule_fast(
          boost::bind(&KernelUObjectImpl::removeTimer, this, h));
        return true;
      }
      rObject me = xget_base(owner_->__name);
      me->call(SYMBOL(removeTimer), new object::String(*h));
      h.reset();
      return true;
    }

    void KernelUContextImpl::registerHub(UObjectHub*)
    {
    }

    void KernelUContextImpl::removeHub(UObjectHub*)
    {
    }

    void KernelUContextImpl::setHubUpdate(UObjectHub* hub, ufloat period)
    {
      // Call Urbi-side setHubUpdate, passing an rPrimitive wrapping
      // the 'update' call.
      if (server().isAnotherThread())
      {
        server().schedule_fast(boost::bind(
          &KernelUContextImpl::setHubUpdate, this, hub, period));
        return;
      }
      rObject uob = object::Object::proto->slot_get(SYMBOL(UObject));
      rObject f = uob->slot_get(SYMBOL(setHubUpdate));
      object::objects_type args;
      args << uob
           << rObject(new object::String(hub->get_name()))
           << new object::Float(period / 1000.0)
           << MAKE_VOIDCALL(hub, urbi::UObjectHub, update);
      ::kernel::runner().apply(f, SYMBOL(setHubUpdate), args);
    }

    std::pair<int, int>
    KernelUContextImpl::kernelVersion() const
    {
      //FIXME: fetch from some other place
      return std::make_pair(2, 0);
    }

    void
    KernelUContextImpl::instanciated(UObject*)
    {
    }

    void
    KernelUContextImpl::lock()
    {
    }

    void
    KernelUContextImpl::unlock()
    {
    }

    KernelUObjectImpl::~KernelUObjectImpl()
    {
      foreach(UGenericCallback* c, callbacks_)
        delete c;
    }

    void
    KernelUObjectImpl::initialize(UObject* owner)
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(
                          boost::bind(&KernelUObjectImpl::initialize, this,
                                      owner));
        return;
      }
      static int uid = 0;
      owner_ = owner;
      bool fromcxx = owner_->__name.empty();
      if (fromcxx)
        owner_->__name = "uob_plug_" + string_cast(++uid);
      if (fromcxx)
      {
        rObject r = ::urbi::uobjects::uobject_new(
                      where->slot_get(SYMBOL(UObject)), false, false);
        r->call(SYMBOL(uobjectInit));
        r->call(SYMBOL(init));
        r->slot_set(SYMBOL(finalize), new object::Primitive(&uobject_finalize));
        r->slot_set(SYMBOL(clone), new object::Primitive(&uobject_clone));
        r->slot_set(SYMBOL(periodicCall), object::primitive(&periodic_call));
        owner->__name = r->slot_get(SYMBOL(__uobjectName))->as<object::String>()
        ->value_get();
      }
      uobject_to_robject[owner_->__name] = owner;
      owner_->ctx_->registerObject(owner);
      rObject o = get_base(owner->__name);
      if (!o)
      {
        // Instanciation occured through ucontext::bind.
        o = ::urbi::uobjects::uobject_make_proto(owner->__name);
        where->slot_set(Symbol(owner->__name), o);
      }
      o->slot_set(SYMBOL(lobby), object::Lobby::proto->slot_get(SYMBOL(systemLobby)));
    }

    void
    KernelUObjectImpl::clean()
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(
                          boost::bind(&KernelUObjectImpl::clean, this));
        return;
      }
      uobject_to_robject.erase(owner_->__name);
    }

    static
    std::string
    me_id(rObject me)
    {
      object::objects_type args;
      args << me;
      return
        ::kernel::runner()
        .apply(me->slot_get(SYMBOL(DOLLAR_id)), SYMBOL(DOLLAR_id), args)
        ->as<object::String>()
        ->value_get();
    }

    static
    std::string
    trace_name(rObject me)
    {
      if (me->slot_has(SYMBOL(compactName)))
        return
          me
          ->slot_get(SYMBOL(compactName))
          ->as<object::String>()
          ->value_get();
      else if (me->slot_has(SYMBOL(__uobjectName)))
        return
          me
          ->slot_get(SYMBOL(__uobjectName))
          ->as<object::String>()
          ->value_get();
      else
        return me_id(me);
    }

    void
    KernelUObjectImpl::setUpdate(ufloat period)
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(
                          boost::bind(&KernelUObjectImpl::setUpdate, this,
                                      period));
        return;
      }
      rObject me = xget_base(owner_->__name);
      rObject f = me->slot_get(SYMBOL(setUpdate));
      me->slot_update
        (SYMBOL(update),
         object::primitive
         (boost::function1<void, rObject>(
           boost::bind(&bounce_update,
                       owner_, me.get(), me_id(me) + " update"))));
      object::objects_type args;
      args << me
           << new object::Float(period / 1000.0);
      ::kernel::runner().apply(f, SYMBOL(setUpdate), args);
    }

    void
    KernelUVarImpl::async(boost::function0<void> f)
    {
      f();
      libport::BlockLock bl(asyncLock_);
      --pending_;
    }

    void
    KernelUVarImpl::schedule(libport::Symbol, boost::function0<void> f) const
    {
      GD_INFO_DUMP("Fast async schedule from UVar");
      KernelUVarImpl* self = const_cast<KernelUVarImpl*>(this);
      {
        libport::BlockLock bl(self->asyncLock_);
        ++self->pending_;
      }
      server().schedule_fast(boost::bind(&KernelUVarImpl::async, self, f));
    }

    void
    KernelUVarImpl::unnotify()
    {
      if (server().isAnotherThread())
      {
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::unnotify, this));
        return;
      }
      GD_FINFO_TRACE("Unnotify on %s: %s callbacks", owner_->get_name(),
                     callbacks_.size());
      // Try to locate the target urbiscript UVar, which might be
      // gone.  In this case, callbacks are gone too.
      foreach (KernelUGenericCallbackImpl* v, callbacks_)
      {
        GD_FINFO_TRACE("Unnotify processing callback %s", v);
        object::rUVar r;
        if (v->inportName_.empty())
          r = ruvar_;
        else
        {
          rObject o = object::UVar::fromName(v->inportName_);
          if (o)
            r = o->as<object::UVar>();
        }
        if (r)
        {
          bool ok = false;
          if (v->owner_->type == "var")
          {
            if (v->owned_)
              ok = r->removeNotifyChangeOwned(v->id_);
            else
              ok = r->removeNotifyChange(v->id_);
          }
          else if (v->owner_->type == "varaccess")
          {
            ok = r->removeNotifyAccess(v->id_);
          }
          GD_FINFO_TRACE("Removal said %s for id %s on %s(%s)", ok, v->id_,
                         owner_->get_name(), ruvar_.get());
        }
        if (v->connection_)
        {
          v->connection_->CxxObject::call(SYMBOL(disconnect));
          GD_FINFO_TRACE("Killing connection associated with UGC %s", v);
        }
      }
    }

    KernelUVarImpl::KernelUVarImpl()
      : pending_(0)
      , bypassMode_(false)
    {
    }

    void
    KernelUVarImpl::initialize(UVar* owner)
    {
      GD_FINFO_TRACE("KernelUVarImpl::init %s", owner->get_name());
      if (server().isAnotherThread())
      {
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::initialize, this,
                                      owner));
        return;
      }
      // Protect against multiple parallel creation of the same UVar.
      runner::Runner& runner = ::kernel::runner();
      bool prevState = runner.non_interruptible_get();
      FINALLY(
        ((bool, prevState))
        ((runner::Runner&, runner))
        ((UVar*, owner)),
        runner.non_interruptible_set(prevState););
      runner.non_interruptible_set(true);
      owner_ = owner;
      owner_->owned = false;
      bypassMode_ = false;
      StringPair p = split_name(owner_->get_name());
      rObject o = get_base(p.first);
      if (!o)
        FRAISE("UVar creation on non existing object: %s", owner->get_name());
      Symbol varName(p.second);
      // Force kernel-side variable creation, init to void.
      rObject initVal;
      if (o->slot_locate(varName).first == o.get())
      {
        initVal = o->local_slot_get(varName)->value();
        // Check if the variable exists and is an uvar.
        if (initVal->slot_has(SYMBOL(owned)))
        {
          traceOperation(owner, SYMBOL(traceBind));
          ruvar_ = o->slot_get(varName)->as<object::UVar>();
          GD_FINFO_DUMP("UVar %s reusing existing object %s",
                        owner_->get_name(),
                        ruvar_.get());
          return;
        }
        else
          o->slot_remove(varName);
      }

      rObject protouvar = object::Object::proto->slot_get(SYMBOL(UVar));
      rObject uvar = protouvar->call(SYMBOL(new),
                                     o, new object::String(varName));
      // If the variable existed but was not an uvar, copy its old value.
      if (initVal)
        o->slot_get(varName)->slot_update(SYMBOL(val), initVal);
      traceOperation(owner, SYMBOL(traceBind));
      ruvar_ = uvar->as<object::UVar>();
      GD_FINFO_DUMP("Uvar %s creating new object %s.",
                    owner_->get_name(),
                    ruvar_.get());
    }

    void KernelUVarImpl::clean()
    {
      GD_FINFO_TRACE("KernelUVarImpl::clean %s", owner_->get_name());
      // Called by the owner UVar's destructor.
      // Block destruction until all pending operations are finished
      if (server().isAnotherThread())
        while(pending_)
          usleep(100);
      else
        while(pending_)
          ::kernel::runner().yield();
      //noop
    }

    void KernelUVarImpl::setOwned()
    {
      if (server().isAnotherThread())
      {
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::setOwned, this));
        return;
      }

      owner_->owned = true;
      // Write 1 to the Urbi-side uvar owned slot.
      ruvar_->slot_update(SYMBOL(owned), object::true_class);
    }

    void KernelUVarImpl::sync()
    {
      //noop
    }

    void KernelUVarImpl::request()
    {
      //noop
    }

    void KernelUVarImpl::keepSynchronized()
    {
      //noop
    }

    void KernelUVarImpl::set(const UValue& v)
    {
      if (server().isAnotherThread())
      {
        UValue vv(v);
        if (vv.type == DATA_BINARY)
          vv.binary->temporary_ = true;
        schedule(SYMBOL(UObject), boost::bind(&KernelUVarImpl::set, this, vv));
        return;
      }
      traceOperation(owner_, SYMBOL(traceSet));
      object::rUValue ov(new object::UValue());
      ov->put(v, bypassMode_);
      if (owner_->owned)
        ruvar_->writeOwned(ov);
      else
        ruvar_->update_(ov);
      ov->invalidate();
    }

    void KernelUVarImpl::async_get(UValue** v, libport::Semaphore& sem) const
    {
      *v = &const_cast<UValue&>(get());
      sem++;
    }

    const UValue& KernelUVarImpl::get() const
    {
      if (server().isAnotherThread())
      {
        libport::Semaphore sem;
        urbi::UValue* v;
        schedule(SYMBOL(UObject),
                          boost::bind(&KernelUVarImpl::async_get, this,
                                      &v, boost::ref(sem)));
        sem--;
        return *v;
      }
      aver(ruvar_);
      traceOperation(owner_, SYMBOL(traceGet));
      try
      {
        rObject o = (owner_->owned
                     ? ruvar_->val
                     : ruvar_->getter(true));
        aver(o);
        if (object::rUValue bv = o->as<object::UValue>())
          return bv->value_get();
        else
        {
          cache_ = ::uvalue_cast(o);
          return cache_;
        }
      }
      catch (object::UrbiException& e)
      {
        FRAISE("invalid read of void UVar '%s': %s", owner_->get_name(),
               e.what());
      }
    }

    ufloat& KernelUVarImpl::in()
    {
      throw std::runtime_error("in() is not implemented");
    }

    ufloat& KernelUVarImpl::out()
    {
      throw std::runtime_error("out() is not implemented");
    }

    UDataType
    KernelUVarImpl::type() const
    {
      if (server().isAnotherThread())
      {
        urbi::UValue* v;
        libport::Semaphore sem;
        schedule(SYMBOL(UObject),
                          boost::bind(&KernelUVarImpl::async_get, this,
                                      &v, boost::ref(sem)));
        sem--;
        return v->type;
      }
      rObject o = (owner_->owned
                   ? ruvar_->val
                   : ruvar_->getter(true));
      return ::uvalue_type(o);
    }

    void
    KernelUVarImpl::async_get_prop(urbi::UValue&v,
                                   libport::Semaphore& sem,
                                   UProperty prop)
    {
      v = getProp(prop);
      sem++;
    }

    UValue KernelUVarImpl::getProp(UProperty prop)
    {
      if (server().isAnotherThread())
      {
        urbi::UValue v;
        libport::Semaphore sem;
        schedule(SYMBOL(UObject),
                          boost::bind(&KernelUVarImpl::async_get_prop, this,
                                      boost::ref(v), boost::ref(sem), prop));
        sem--;
        return v;
      }
      aver(ruvar_);
      if (prop == urbi::PROP_RANGEMIN)
        return UValue(ruvar_->rangemin);
      else if (prop == urbi::PROP_RANGEMAX)
        return UValue(ruvar_->rangemax);
      StringPair p = split_name(owner_->get_name());
      rObject o = xget_base(p.first);
      return ::uvalue_cast(o->call(SYMBOL(getProperty),
                                   new object::String(p.second),
                                   new object::String(UPropertyNames[prop])));
    }

    void KernelUVarImpl::setProp(UProperty prop, const UValue& v)
    {
      if (server().isAnotherThread())
      { // Copy the data
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::setProp,
           this, prop, v));
        return;
      }
      StringPair p = split_name(owner_->get_name());
      rObject o = xget_base(p.first);
      o->call(SYMBOL(setProperty),
              new object::String(p.second),
              new object::String(UPropertyNames[prop]),
              object_cast(v));
    }

    bool KernelUVarImpl::setBypass(bool enable)
    {
      bypassMode_ = enable;
      return true;
    }

    void KernelUVarImpl::useRTP(bool enable)
    {
      if (server().isAnotherThread())
      { // Copy the data
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::useRTP,
                                             this, enable));
        return;
      }
      ruvar_->slot_set(SYMBOL(rtp),  object::to_urbi(enable));
    }

    void KernelUVarImpl::setInputPort(bool enable)
    {
      if (server().isAnotherThread())
      { // Copy the data
        schedule(SYMBOL(UObject),boost::bind(&KernelUVarImpl::setInputPort,
           this, enable));
        return;
      }
      runner::Runner& r = ::kernel::urbiserver->getCurrentRunner();
      bool last_rdm = r.redefinition_mode_get();
      FINALLY( ((bool, last_rdm))((runner::Runner&, r)),
                      r.redefinition_mode_set(last_rdm));
      r.redefinition_mode_set(true);
      if (enable)
        ruvar_->slot_set(SYMBOL(inputPort), object::to_urbi(true));
      else
        ruvar_->slot_remove(SYMBOL(inputPort));
    }

    time_t
    KernelUVarImpl::timestamp() const
    {
      if (!ruvar_)
        throw std::runtime_error("UVar without cache");
      return time_t(ruvar_->timestamp);
    }

    void
    KernelUGenericCallbackImpl::initialize(UGenericCallback* owner, bool owned)
    {
      owner_ = owner;
      owned_ = owned;
      registered_ = false;
    }

    void
    KernelUGenericCallbackImpl::initialize(UGenericCallback* owner)
    {
      initialize(owner, false);
    }

    void
    KernelUGenericCallbackImpl::registerCallback()
    {
      if (server().isAnotherThread())
      {
        server().schedule_fast(boost::bind(
                     &KernelUGenericCallbackImpl::registerCallback, this));
        return;
      }
      if (registered_)
      {
        std::cerr << "###UGenericcallback on " << owner_->name
                  << " already registered" << std::endl;
        return;
      }
      useClosedVar_ = false;
      registered_ = true;
      StringPair p = split_name(owner_->name);
      std::string method = p.second;
      GD_FPUSH_DUMP("UGC %s, %s, %s, %s", owner_->type,p.first, method, owned_);
      // UObject owning the variable/event to monitor
      rObject me = xget_base(p.first); //objname?
      std::string traceName = trace_name(me);
      if (owner_->type == "function")
      {
        traceName += "." + p.second;
        me->slot_set(libport::Symbol(method), new object::Primitive(
                       boost::function1<rObject, const objects_type&>
                       (boost::bind(&wrap_ucallback, _1, owner_, traceName,
                                    true))));
      }
      if (owner_->type == "event")
      {
        UEvent e = UEvent(p.first, p.second); // force creation of the event
        rEvent event =
          me->slot_get(libport::Symbol(method))->as<object::Event>();
        event->onEvent(0, new object::Primitive(
                       boost::function1<rObject, const objects_type&>
                       (boost::bind(&wrap_event, _1, owner_, traceName))));
      }
      if (owner_->type == "var" || owner_->type == "varaccess")
      {
        // NotifyChange or NotifyAccess callback
        GD_FINFO_DUMP("UGC %s using backend UConnection.", this);
        //owner_->owner is the UObject that sets the callback: target
        // Compute tracing name
        // traceName is objName__obj__var
        traceName += "." + p.second + " --> ";
        if (&owner_->owner)
          if (rObject you = get_base(owner_->owner.__name))
            traceName += trace_name(you);

        // Source UVar
        object::rUVar var = me->slot_get(Symbol(method))->as<object::UVar>();
        aver(var);
        rObject source = xget_base(owner_->objname);
        GD_FINFO_TRACE("UGC same source: %s (%s==%s)", source == me,
                       owner_->objname, p.first);
        if (source != me)
        {
          if (owner_->type != "varaccess" && !owned_)
          {
            GD_FINFO_TRACE("UGC %s using UConnection backend.", this);
            // Use the new mechanism: create an input port and use it
            std::string ipname = owner_->name;
            size_t p = ipname.find_first_of('.');
            if (p != ipname.npos)
              ipname[p] = '_';
            InputPort ip(owner_->objname, ipname);
            inportName_ = owner_->objname + "." + ipname;
            // Retarget callback
            var = object::UVar::fromName(owner_->objname + "." + ipname)
              ->as<object::UVar>();
            object::rUConnection c
              = object::UConnection::proto
              ->CxxObject::call(SYMBOL(new),
                                object::UVar::fromName(owner_->name), var)
              ->as<object::UConnection>();
            connection_ = c;
            useClosedVar_ = true;
            GD_FINFO_TRACE("UGC %s using UConnection backend %s.", this,
                           connection_.get());
          }
          else
            GD_FWARN("invalid usage of foreign UVar: notifyaccess or USensor"
                       "notifychange on %s from %s",
                     owner_->name, owner_->objname);
        }
        callback_ = new object::Primitive(
            boost::function1<rObject, const objects_type&>
            (boost::bind(&wrap_ucallback_notify, _1, owner_,
                         traceName)));
        callback_->slot_set
          (SYMBOL(target),
           new object::String(&owner_->owner ? owner_->owner.__name:"unknown"));

        if (owner_->type == "varaccess")
          id_ = var->notifyAccess_(callback_);
        else if (owned_)
          id_ = var->notifyChangeOwned_(callback_);
        else
          id_ = var->notifyChange_(callback_);
        GD_FINFO_DUMP("Registered gave id %s on %s(%s)", id_, owner_->name,
                       var.get());
        if (owner_->target)
        {
          KernelUVarImpl* vimpl
            = static_cast<KernelUVarImpl*>(owner_->target->impl_);
          GD_FINFO_DUMP("Registering callback to KernelUVarImpl %s", vimpl);
          vimpl->callbacks_ << this;
        }
      }
      if (&owner_->owner)
        static_cast<KernelUObjectImpl*>(owner_->owner.impl_get())->callbacks_
          .push_back(owner_);
    }

    void
    KernelUGenericCallbackImpl::clear()
    {
    }
  }

  int kernelMajor()
  {
    const std::string& kv = kernelVersion();
    return boost::lexical_cast<int>(kv.substr(0, kv.find_first_of('.')));
  }

  int kernelMinor()
  {
    const std::string& kv = kernelVersion();
    return boost::lexical_cast<int>(kv.substr(kv.find_first_of('.')+1,
                                              kv.npos));
  }

  const std::string& kernelVersion()
  {
    return object::system_class->call(SYMBOL(version))->as<object::String>()
      ->value_get();
  }

  namespace uobjects
  {
    StringPair split_name(const std::string& name)
    {
      size_t p = name.find_last_of(".");
      if (p == name.npos)
      {
        GD_FWARN("invalid argument to split_name: %s", name);
        return StringPair(name, "");
      }
      std::string oname = name.substr(0, p);
      std::string slot = name.substr(p + 1, name.npos);
      return StringPair(oname, slot);
    }

    /** Find an UObject from its name.

    The UObject class expects to know the variable name, i.e. a = new b;
    should pass a to b's corresponding UObject ctor. Since we don't have this
    information, we create a unique string, pass it to the ctor, and store it
    in a.

    But the user can create UVars based on the variable name it knows about,
    i.e. a.val. So get_base must look in its uid map, and if it finds nothing,
    look for an Urbi variable with given name. We expect all UObjects to be
    created in Global.
    */
    rObject
    get_base(const std::string& objname)
    {
      rObject res = libport::find0(uobject_map, objname);
      object::Object::location_type s;
      // The user may be using the Urbi variable name.
      if (!res)
      {
        s = object::global_class->slot_locate(libport::Symbol(objname));
        // Not simplifyable! If the rSlot contains 0, casting to
        // rObject will segv.
        if (s.second)
          res = *s.second;
      }
      if (!res)
      {
        s = where->slot_locate(libport::Symbol(objname));
        if (s.second)
          res = *s.second;
      }
      return res;
    }

    /*! Initialize plugin UObjects.
    \param args object in which the instances will be stored.
    */
    rObject uobject_initialize(const objects_type& args)
    {
      CAPTURE_GLOBAL(Global);
      urbi::setCurrentContext(new urbi::impl::KernelUContextImpl());
      where = args.front();
      where->slot_set(SYMBOL(setTrace), object::primitive(&setTrace));
      uobjects_reload();
      where->slot_set(SYMBOL(getStats),    object::primitive(&Stats::get));
      where->slot_set(SYMBOL(clearStats),  object::primitive(&Stats::clear));
      where->slot_set(SYMBOL(enableStats), object::primitive(&Stats::enable));
      Global->slot_set(SYMBOL(uvalueDeserialize), primitive(&uvalue_deserialize));

      where->bind(SYMBOL(searchPath),    &uobject_uobjectsPath,
                  SYMBOL(searchPathSet), &uobject_uobjectsPathSet);
      uobjects_path
        .push_back(libport::xgetenv("URBI_UOBJECT_PATH", ".:"),
                   kernel::urbiserver->urbi_root_get().uobjects_path(),
                   ":");
      return object::void_class;
    }

    /*! Create the prototype for an UObject class.
    */
    rObject
    uobject_make_proto(const std::string& name)
    {
      rObject res =
        object::Object::proto
        ->slot_get(SYMBOL(UObject))
        ->call(SYMBOL(clone));
      res->call(SYMBOL(uobjectInit));
      res->call(SYMBOL(init));
      res->slot_set(SYMBOL(finalize), new object::Primitive(&uobject_finalize));
      res->slot_set(SYMBOL(__uobject_cname), new object::String(name));
      res->slot_set(SYMBOL(__uobject_base), res);
      res->slot_set(SYMBOL(clone), new object::Primitive(&uobject_clone));
      res->slot_set(SYMBOL(periodicCall), object::primitive(&periodic_call));
      return res;
    }

    /*! Instanciate a new prototype inheriting from a UObject.
    A new instance of UObject is created
    \param proto proto object, created by uobject_make_proto() or uobject_new()
    \param forceName force the reported C++ name to be the class name
    \param instanciate true if the UObject should be instanciated, false if it
    already is.
    */
    rObject
    uobject_new(rObject proto, bool forceName, bool instanciate)
    {
      rObject res = new object::Finalizable(proto->as<object::Finalizable>());

      // Get UObject name.
      rObject rcName = proto->slot_get(SYMBOL(__uobject_cname));
      const std::string& cname = rcName.cast<object::String>()->value_get();

      // Get the name we will pass to uobject.
      std::string name;
      if (forceName)
      {
        res->slot_set(SYMBOL(type), rcName);
        name = cname;
        where->slot_set(libport::Symbol(name), res);
      }
      else
      {
        // boost::lexical_cast does not work on the way back, so dont
        // use it here.
        std::stringstream ss;
        ss << "uob_" << res.get();
        name = ss.str();
        /* We need to make this name accessible in urbi in case the UObject code
        emits urbi code using this name.*/
        where->slot_set(libport::Symbol(name), res);
      }
      uobject_map[name] = res.get();
      res->slot_set(SYMBOL(__uobjectName), object::to_urbi(name));
      res->call(SYMBOL(uobjectInit));
      // Instanciate UObject.
      if (instanciate)
      {
        foreach (urbi::baseURBIStarter* i, urbi::baseURBIStarter::list())
          if (i->name == cname)
          {
            bound_context.push_back(std::make_pair(name, name + ".new"));
            FINALLY(((std::string, name)), bound_context.pop_back());
            i->instanciate(urbi::impl::KernelUContextImpl::instance(), name);
            return res;
          }
      }
      return res;
    }

    std::string processSerializedMessage(int msgType,
                                 libport::serialize::BinaryISerializer& ia)
    {
      switch(msgType)
      {
      case urbi::UEM_ASSIGNVALUE:
        {
          std::string name;
          urbi::UValue val;
          libport::utime_t time;
          unsigned int tlow, thi;
          ia >> name >> val >> tlow >> thi;
          time = tlow + ((libport::utime_t)thi << 32);
          StringPair p = split_name(name);
          GD_FINFO_TRACE("UEM_ASSIGNVALUE %s %s", name, val);
          object::rUValue ov(new object::UValue(val));
          object::global_class
            ->slot_get(Symbol(p.first))
            ->slot_get(Symbol(p.second))
            ->call(SYMBOL(update_timed), ov, object::to_urbi(time));
        }
        break;
      case urbi::UEM_EMITEVENT:
        {
          std::string name;
          int count;
          ia >> name >> count;
          object::objects_type args;
          for (int i=0; i<count; ++i)
          {
            UValue v;
            ia >> v;
            args.push_back(object_cast(v));
          }
          impl::doEmit(name, args);
        }
        break;
      case urbi::UEM_REPLY:
        {
          std::string id;
          UValue val;
          ia >> id >> val;
          object::global_class->slot_get(SYMBOL(UObject))
          ->call(SYMBOL(funCall), object::to_urbi(id),
                 object_cast(val));
        }
        break;
      case urbi::UEM_EVAL:
        {
          std::string code;
          ia >> code;
          return code;
        }
        break;
      default:
        GD_FWARN("Invalid serialized message number %s", (int)msgType);
        break;
      }
      return "";
    }
  }
}
