#pragma once

#include <memory>

namespace exchange::core {

    template< typename _Ty >
    class factory : public std::enable_shared_from_this<_Ty>
    {
    public:
        template <typename ..._Args>
        static std::shared_ptr<_Ty> create(_Args&&... args) {
            return std::shared_ptr<_Ty>(new _Ty(std::forward<_Args>(args)...));
        }
    };
}

