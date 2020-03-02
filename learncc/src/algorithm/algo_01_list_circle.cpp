#include "iostream"

namespace SCircle {
	struct SList {
		SList* next;
		int value;
	};

	inline bool isCircle(SList* header) {
		if (!header)
			return false;
		auto next = header->next;
		if (!next) {
			return false;
		}
		auto nextNext = next->next;
		while (next&&nextNext)
		{
			if (next == nextNext)
				return true;
			next = next->next;
			nextNext = nextNext->next;
			if (nextNext)
				nextNext = nextNext->next;
		}
		return false;
	}
}