#include <span>

template <typename Derived, typename ResultT>
class Task {
public:
    using result_type = ResultT;
    void run() { self().runImpl(); }
private:
    Task() = default;
    friend Derived;

    Derived& self() { 
        return static_cast<Derived&>(*this); 
    }
};

template <typename Derived, typename SourceT, typename ResultT>
class TaskReduction {
public:
    using result_type = ResultT;

    void accumulate(const SourceT& src) { 
        self().accumulateImpl(src); 
    }
    void accumulate(std::span<const SourceT> srcs) {
        for (const auto& s : srcs) accumulate(s);
    }
    [[nodiscard]] ResultT getResult() const { 
        return self().getResultImpl(); 
    }
private:

    TaskReduction() = default;
    friend Derived;

    Derived& self() {
         return static_cast<Derived&>(*this); 
    }
    const Derived& self() const { return static_cast<const Derived&>(*this); }
};