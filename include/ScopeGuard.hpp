#ifndef SCOPEGUARD_H
#define SCOPEGUARD_H

#include <functional>
#include <memory>

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define SCOPEGUARD(scope) detail::ScopeGuard TOKENPASTE2(TOKENPASTE2(__SCOPE_GUARD_VAR_,__COUNTER__), __LINE__)( [&]() {scope;} )

namespace detail
{
    class ScopeGuard
    {
    public:
        explicit ScopeGuard(std::function<void()> onExitScope) 
            : onExitScope_(onExitScope), dismissed_(false)
        { }

        ~ScopeGuard()
        {
            try
            {
                if(!dismissed_)
                {
                    onExitScope_();
                }
            }
            catch(...){}
        }

        void Dismiss()
        {
            dismissed_ = true;
        }
    private:
        std::function<void()> onExitScope_;
        bool dismissed_;

        // noncopyable
    private:
        ScopeGuard(ScopeGuard const&);
        ScopeGuard& operator=(ScopeGuard const&);
    };
};

inline std::unique_ptr<detail::ScopeGuard> CreateScopeGuard(std::function<void()> onExitScope)
{
    return std::unique_ptr<detail::ScopeGuard>(new detail::ScopeGuard(onExitScope));
}

#endif /*SCOPEGUARD_H*/