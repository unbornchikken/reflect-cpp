/* cast.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 24 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Casting utility.
*/

#include "reflect.h"
#pragma once

namespace reflect {

/******************************************************************************/
/* TARGET REF                                                                 */
/******************************************************************************/

namespace details {

template<typename T>
struct TargetRef
{
    typedef typename std::remove_reference<T>::type CleanT;
    typedef typename std::add_lvalue_reference<CleanT>::type RefT;

    typedef typename std::conditional<
        std::is_lvalue_reference<T>::value, RefT, CleanT>::type
        type;
};

} // namespace details


/******************************************************************************/
/* CAST                                                                       */
/******************************************************************************/

template<typename T, typename Target>
struct Cast
{
    template<typename U>
    static auto cast(U&& value) -> typename details::TargetRef<Target>::type
    {
        return std::forward<U>(value);
    }
};

template<typename Target>
struct Cast<Value, Target>
{
    typedef typename details::TargetRef<Target>::type TargetRef;
    typedef typename std::decay<Target>::type CleanTarget;

    // We make a copy of the argument to ensure that we support const-refs
    // correctly.
    static TargetRef cast(Value value)
    {
        return cast(value,
                std::is_lvalue_reference<Target>(),
                std::is_rvalue_reference<Target>());
    }

private:

    static TargetRef cast(Value& value, std::false_type, std::false_type)
    {
        if (value.refType() == RefType::RValue)
            return move(value);
        return copy(value);
    }

    static TargetRef cast(Value& value, std::true_type, std::false_type)
    {
        return value.cast<Target>();
    }

    static TargetRef cast(Value& value, std::false_type, std::true_type)
    {
        return value.move<Target>();
    }



    static TargetRef copy(Value& value)
    {
        return copy(value,
                typename std::is_copy_constructible<CleanTarget>::type());
    }

    static TargetRef copy(Value& value, std::true_type)
    {
        return value.copy<Target>();
    }

    static TargetRef copy(Value& value, std::false_type)
    {
        reflectError("<%s> cannot be copied to <%s>",
                value.argument().print(), printArgument<Target>);
    }


    static TargetRef move(Value& value)
    {
        return move(value,
                typename std::is_move_constructible<CleanTarget>::type(),
                typename std::is_copy_constructible<CleanTarget>::type());
    }

    template<typename Meh>
    static TargetRef move(Value& value, std::true_type, Meh)
    {
        return value.move<Target>();
    }

    static TargetRef move(Value& value, std::false_type, std::true_type)
    {
        return value.copy<Target>();
    }

    static TargetRef copy(Value& value, std::false_type, std::false_type)
    {
        reflectError("<%s> cannot be moved to <%s>",
                value.argument().print(), printArgument<Target>);
    }

};

template<typename T>
struct Cast<T, Value>
{
    template<typename U>
    static Value cast(U&& value)
    {
        return Value(std::forward<U>(value));
    }
};

template<>
struct Cast<Value, Value>
{
    static       Value& cast(      Value&  value) { return value; }
    static       Value  cast(      Value&& value) { return std::move(value); }
    static const Value& cast(const Value&  value) { return value; }
};

template<typename T>
struct Cast<T, T>
{
    static       T& cast(      T&  value) { return value; }
    static       T  cast(      T&& value) { return std::move(value); }
    static const T& cast(const T&  value) { return value; }
};

template<typename Target, typename T>
auto cast(T&& value) ->
    decltype(Cast<typename std::decay<T>::type, Target>::cast(std::forward<T>(value)))
{
    typedef typename std::decay<T>::type CleanT;
    return Cast<CleanT, Target>::cast(std::forward<T>(value));
}


/******************************************************************************/
/* RET CAST                                                                   */
/******************************************************************************/
/** Used when returning a Value object which may or may-not be void. Since this
    is valid c++ (as far as gcc is concerned anyways):

        void bar() {}
        void foo() { return bar(); }

    which means that if our cast function is able to return void values (doing
    nothing in the cast), then we can use it to avoid having to define two
    seperate call functions. Fun.
 */

template<typename Target, typename T>
Target retCast(T&&, std::true_type) {}

template<typename Target, typename T>
Target retCast(T&& value, std::false_type)
{
    return cast<Target>(std::forward<T>(value));
}

template<typename Target, typename T>
Target retCast(T&& value)
{
    return retCast<Target>(std::forward<T>(value), std::is_same<Target, void>());
}

} // reflect
