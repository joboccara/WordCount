#ifndef helpers_hpp
#define helpers_hpp

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
