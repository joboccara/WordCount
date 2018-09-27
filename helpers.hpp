#ifndef helpers_hpp
#define helpers_hpp

#include <vector>

template<typename T, typename Res, typename... Args>
class CallMethod
{
public:
    using MemberFunctionType = Res(T::*)(Args...) const;
    explicit CallMethod(MemberFunctionType memberFunction) : memberFunction_(memberFunction) {}
    decltype(auto) operator()(T const& value)
    {
        return (value.*memberFunction_)();
    }
private:
    MemberFunctionType memberFunction_;
};

template<typename T, typename Res, typename... Args>
CallMethod<T, Res, Args...> callMethod(Res(T::* memberFunction)(Args...) const)
{
    return CallMethod<T, Res, Args...>(memberFunction);
}

template<typename MemberFunction>
auto projectOnMember(MemberFunction memberFunction)
{
    return [memberFunction](auto const& inputs)
    {
        using value_type = decltype(inputs.front());
        using method_return_type = decltype((std::declval<value_type>().*memberFunction)());
        auto results = std::vector<std::decay_t<method_return_type>>{};
        std::transform(begin(inputs), end(inputs), back_inserter(results), callMethod(memberFunction));
        return results;
    };
}

template<typename Function>
auto project(Function function)
{
    return [function](auto const& inputs)
    {
        using value_type = decltype(inputs.front());
        using function_return_type = std::result_of_t<Function(value_type)>;
        auto results = std::vector<std::decay_t<function_return_type>>{};
        std::transform(begin(inputs), end(inputs), back_inserter(results), function);
        return results;
    };
}

template<typename Derived>
struct Comparable
{
    bool operator>=(Derived const& other) const { return !(static_cast<Derived const&>(*this) < other); }
    bool operator<=(Derived const& other) const { return !(other < static_cast<Derived const&>(*this)); }
    bool operator>(Derived const& other) const { return !(*this <= other); }
    bool operator==(Derived const& other) const { return !(static_cast<Derived const&>(*this) < other) && !(other < static_cast<Derived const&>(*this)); }
    bool operator!=(Derived const& other) const { return !(*this == other); }
};


#endif /* helpers_hpp */
