/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/vm/object-allocator-sizes.h"
#include "hphp/runtime/base/complex-types.h"
#include "hphp/runtime/base/object-data.h"
#include "hphp/runtime/base/smart-allocator.h"

namespace HPHP {

namespace {

template<int Idx> struct AllocIniter {
  static void run() {
    ThreadLocalSingleton<ObjectAllocator<ObjectSizeTable<Idx>::value> > tls;
    ObjectAllocatorCollector::getWrappers()[Idx] =
      (ObjectAllocatorBaseGetter)tls.getCheck;
    GetAllocatorInitList().insert((AllocatorThreadLocalInit)tls.getCheck);

    AllocIniter<Idx + 1>::run();
  }
};

template<> struct AllocIniter<NumObjectSizeClasses> {
  static void run() {}
};

}

int InitializeAllocators() {
  AllocIniter<0>::run();
  return NumObjectSizeClasses;
}


}
