
//CRTPs because the templates will be used for all tasks and reductions to avoid the cost of the 
// vtables
template <typename Derived, typename ResultT>
struct Task{
    void run(){
        static_cast<Derived*>(this)->runImpl();
    }
};

template <typename Derived, typename TaskT, typename ResultT>
struct TaskReduction{
    void accumulate(const TaskT& task) {
        static_cast<Derived*>(this)->accumulateImpl(task);
    }
    void accumulate(const std::vector<TaskT>& tasks) {
        for (const auto& task : tasks) accumulate(task);
    }
    ResultT getResult() const {
        return static_cast<const Derived*>(this)->getResultImpl();
};