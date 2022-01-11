template <typename T> T bits(T val, int start, int end)
{
	return val & ((unsigned long long)(pow(2,end+1)-1) >> start); 
}