//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// XFAIL: gcc-4

// <utility>

// template <class T1, class T2> struct pair

// template<class U, class V> pair(U&& x, V&& y);


#include <cuda/std/utility>
// cuda/std/memory not supported
// #include <cuda/std/memory>
#include <cuda/std/cassert>

#include "archetypes.h"
#include "test_convertible.h"

#include "test_macros.h"
using namespace ImplicitTypes; // Get implicitly archetypes

template <class T1, class T1Arg,
          bool CanCopy = true, bool CanConvert = CanCopy>
__host__ __device__ void test_sfinae() {
    using P1 = cuda::std::pair<T1, int>;
    using P2 = cuda::std::pair<int, T1>;
    using T2 = int const&;
    static_assert(cuda::std::is_constructible<P1, T1Arg, T2>::value == CanCopy, "");
    static_assert(test_convertible<P1,   T1Arg, T2>() == CanConvert, "");
    static_assert(cuda::std::is_constructible<P2, T2,   T1Arg>::value == CanCopy, "");
    static_assert(test_convertible<P2,   T2,   T1Arg>() == CanConvert, "");
}

struct ExplicitT {
  __host__ __device__ constexpr explicit ExplicitT(int x) : value(x) {}
  int value;
};

struct ImplicitT {
  __host__ __device__ constexpr ImplicitT(int x) : value(x) {}
  int value;
};


int main(int, char**)
{
    // cuda/std/memory not supported
    /*
    {
        typedef cuda::std::pair<cuda::std::unique_ptr<int>, short*> P;
        P p(cuda::std::unique_ptr<int>(new int(3)), nullptr);
        assert(*p.first == 3);
        assert(p.second == nullptr);
    }
    */
    {
        // Test non-const lvalue and rvalue types
        test_sfinae<AllCtors, AllCtors&>();
        test_sfinae<AllCtors, AllCtors&&>();
        test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&, true, false>();
        test_sfinae<ExplicitTypes::AllCtors, ExplicitTypes::AllCtors&&, true, false>();
        test_sfinae<CopyOnly, CopyOnly&>();
        test_sfinae<CopyOnly, CopyOnly&&>();
        test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&, true, false>();
        test_sfinae<ExplicitTypes::CopyOnly, ExplicitTypes::CopyOnly&&, true, false>();
        test_sfinae<MoveOnly, MoveOnly&, false>();
        test_sfinae<MoveOnly, MoveOnly&&>();
        test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&, false>();
        test_sfinae<ExplicitTypes::MoveOnly, ExplicitTypes::MoveOnly&&, true, false>();
        test_sfinae<NonCopyable, NonCopyable&, false>();
        test_sfinae<NonCopyable, NonCopyable&&, false>();
        test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&, false>();
        test_sfinae<ExplicitTypes::NonCopyable, ExplicitTypes::NonCopyable&&, false>();
    }
    {
        // Test converting types
        test_sfinae<ConvertingType, int&>();
        test_sfinae<ConvertingType, const int&>();
        test_sfinae<ConvertingType, int&&>();
        test_sfinae<ConvertingType, const int&&>();
        test_sfinae<ExplicitTypes::ConvertingType, int&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, const int&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, int&&, true, false>();
        test_sfinae<ExplicitTypes::ConvertingType, const int&&, true, false>();
    }
#if TEST_STD_VER > 11
    { // explicit constexpr test
        constexpr cuda::std::pair<ExplicitT, ExplicitT> p(42, 43);
        static_assert(p.first.value == 42, "");
        static_assert(p.second.value == 43, "");
    }
    { // implicit constexpr test
        constexpr cuda::std::pair<ImplicitT, ImplicitT> p = {42, 43};
        static_assert(p.first.value == 42, "");
        static_assert(p.second.value == 43, "");
    }
#endif

  return 0;
}
