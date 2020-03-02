#include "iostream"
#include "cstdlib"
#include "cstdio"
#include "cstring"
#include "algorithm"

int huiwensize(const char* buffer, int header, int bottom, int start, int  end);
int maxHuiwen(const char* buffer);
int maxHuiwenHelper(const char* buffer,  int start, int  end, int header,int bottom);

int maxHuiwenHelper(const char* buffer, int start, int  end, int header, int bottom) {
	if (start == end) {
		return 1;
	}
	if (start > end) {
		return 0;
	}
	auto middle = (end + start) / 2;
	auto frontPart = maxHuiwenHelper(buffer, start, middle, header, bottom);
	auto endPart = maxHuiwenHelper(buffer, middle+1, end, header, bottom);
	auto middlePart = 0;
	if ((end-start)%2 ==1)
		 middlePart = huiwensize(buffer, header, bottom, middle, middle + 1);
	else {
		 middlePart = huiwensize(buffer, header, bottom, middle-1, middle + 1) +1;
	}
	if (frontPart > endPart)
		return std::max(frontPart, middlePart);
	else
		return std::max(endPart, middlePart);
}

int huiwensize(const char* buffer, int header, int bottom, int start, int  end) {
	int count = 0;
	while (buffer[start] == buffer[end])
	{
		count += 2;
		if (start <= header || end >= bottom) break;
		--start;
		++end;
	}
	return count;
}

int maxHuiwen(const char* buffer) {
	int len = std::strlen(buffer);
	if (len <= 1) return len;
	return  maxHuiwenHelper(buffer, 0, len - 1, 0, len - 1);
}

void main() {
	std::cout<<maxHuiwen("ab");
}
/*
abc>1
aa>2
aba>3
abccba>6

*/