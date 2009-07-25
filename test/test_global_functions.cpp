#include "unittest.hpp"
#include "rice/global_function.hpp"

using namespace Rice;

TESTSUITE(GlobalFunction);

SETUP(GlobalFunction)
{
  ruby_init();
}

namespace {

  bool method_to_wrap_called = false;
  void method_to_wrap(Object self) {
    method_to_wrap_called = true;
  }

  int method_with_args_arg0;

  void method_with_args(Object self, int arg) {
    method_with_args_arg0 = arg;
  } 

}

TESTCASE(exposes_global_functions)
{
  define_global_function("method_to_wrap", &method_to_wrap);
  Module m = Module(rb_mKernel);
  m.call("method_to_wrap");

  ASSERT(method_to_wrap_called);
}

TESTCASE(exposes_global_functions_with_arguments)
{
  define_global_function("method_with_args", &method_with_args);
  Module m = Module(rb_mKernel);
  m.call("method_with_args", 10);

  ASSERT_EQUAL(10, method_with_args_arg0);
}

namespace 
{
  int defaults_method_one_arg1;
  int defaults_method_one_arg2;
  bool defaults_method_one_arg3 = false;

  void defaults_method_one(int arg1, int arg2 = 3, bool arg3 = true) 
  {
    defaults_method_one_arg1 = arg1;
    defaults_method_one_arg2 = arg2;
    defaults_method_one_arg3 = arg3;
  }
}

TESTCASE(default_arguments_for_define_global_function)
{
  define_global_function("foo", &defaults_method_one, (Arg("arg1"), Arg("arg2") = 3, Arg("arg3") = true));
  Module m(rb_mKernel);

  m.call("foo", 2);

  ASSERT_EQUAL(2, defaults_method_one_arg1);
  ASSERT_EQUAL(3, defaults_method_one_arg2);
  ASSERT(defaults_method_one_arg3);

  m.call("foo", 11, 10);

  ASSERT_EQUAL(11, defaults_method_one_arg1);
  ASSERT_EQUAL(10, defaults_method_one_arg2);
  ASSERT(defaults_method_one_arg3);

  m.call("foo", 22, 33, false);

  ASSERT_EQUAL(22, defaults_method_one_arg1);
  ASSERT_EQUAL(33, defaults_method_one_arg2);
  ASSERT(!defaults_method_one_arg3);
}
