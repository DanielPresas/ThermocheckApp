#ifndef MEMORY_H
#define MEMORY_H

#include <memory>

template<class T>
using Ref = std::shared_ptr<T>;

template<class T>
using Scope = std::unique_ptr<T>;

namespace Memory {
	template<class T, typename ...Args>
	static constexpr Scope<T> createScope(Args&& ...args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<class T, typename ...Args>
	static constexpr Ref<T> createRef(Args&& ...args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif